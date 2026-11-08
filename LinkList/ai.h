#pragma once
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

using namespace std;
using json = nlohmann::json;

// libcurl 回调，收集响应体
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t real_size = size * nmemb;
    ((string*)userp)->append((char*)contents, real_size);
    return real_size;
}

string model(const string& system, const string& user) {
    const char* key = getenv("DEEPSEEK_API_KEY");
    if (!key || !*key) {
        throw runtime_error("请先设置 DEEPSEEK_API_KEY 环境变量");
    }
    const string api_key = key;
    json request_body;
    request_body["model"] = "deepseek-flash";
    request_body["messages"] = json::array({
        {{"role", "system"}, {"content", system}},
        {{"role", "user"}, {"content", user}}
    });
    request_body["thinking"] = {{"type", "enabled"}};
    request_body["reasoning_effort"] = "high";
    request_body["stream"] = false;

    string body_str = request_body.dump();
    const string url = "https://api.deepseek.com/chat/completions";
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw runtime_error("CURL 初始化失败");
    }
    string read_buffer;
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_str.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)body_str.size());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 120L);
    CURLcode res = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_global_cleanup();
    if (res != CURLE_OK) {
        throw runtime_error(string("请求失败: ") + curl_easy_strerror(res));
    }
    if (http_code != 200) {
        throw runtime_error("HTTP " + to_string(http_code) + "，响应体:\n" + read_buffer);
    }
    try {
        auto j = json::parse(read_buffer);
        return j["choices"][0]["message"]["content"].get<string>();
    } catch (const exception& e) {
        throw runtime_error(string("解析响应失败: ") + e.what() + "\n原始响应:\n" + read_buffer);
    }
}
