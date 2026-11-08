# ppt.h 接口文档

基于 `LinkList<slide>` 的 PPT 数据层，每张幻灯片对应一个结点。
最终导出 Python 脚本，运行后通过 python-pptx 生成真正的 `.pptx` 文件。

## class slide

| 接口 | 说明 |
|---|---|
| `slide()` | 默认构造，html 和 prompt 为空 |
| `setprompt(string &s)` | 设置单页自然语言要求，参数需为字符串变量 |
| `sethtml(string &s)` | 保存 AI 返回的 Python 代码，html 是保留的旧字段名 |
| `getprompt()` | 返回要求的 `string&` |
| `gethtml()` | 返回代码的 `string&` |

## class ppt

页面下标从 0 开始。

| 接口 | 说明 |
|---|---|
| `ppt(string name1="", string s="")` | 创建空 PPT，参数为名字和整体要求 |
| `ppt(const ppt &b)` | 深拷贝页面，同时复制名字和整体要求 |
| `getname()` | 返回 PPT 名字 |
| `size()` | 返回页数 |
| `getslide(int pos)` | 返回 `slide*`，越界返回 `nullptr` |
| `setslide(int pos, slide s)` | 替换指定页，越界时直接返回 |
| `addslide(slide s)` | 尾部追加一页 |
| `delslide(int n)` | 删除指定页，调用方需保证下标有效 |
| `generate(int n)` | 调用 DeepSeek，生成指定页的代码并写回该页 |
| `generateall()` | 按顺序重新生成所有页的代码，并显示进度 |

## 工具函数

| 接口 | 所在文件 | 说明 |
|---|---|---|
| `model(const string& system, const string& user)` | `LinkList/ai.h` | 从环境变量读取密钥，调用 DeepSeek 并返回文本 |
| `exportppt(ppt& p, string filename="generate_ppt.py")` | `LinkList/tool.h` | 合并每页代码并输出 Python 脚本，未生成的页导出为空白页 |
| `testppt()` | `LinkList/testdata.h` | 返回包含十页要求的测试 PPT，不调用 API |

每页代码应定义 `add_slide(prs)` 函数，只向已有的 `prs` 添加一页。
导出工具会逐页调用该函数，最后保存脚本同目录下的 `output.pptx`。

## 典型用法

```cpp
#include "testdata.h"
#include "tool.h"

int main()
{
    ppt deck = testppt();
    string prompt = "制作一页蓝白配色的课程汇报封面";
    deck.getslide(0)->setprompt(prompt);
    deck.generateall();
    exportppt(deck);
}
```

设置 `DEEPSEEK_API_KEY` 后运行上述程序，再执行 `py -3 generate_ppt.py`。

## 当前限制

- 请传入有效的页面下标；生成和删除没有完整的越界处理。
- 批量生成会覆盖已有代码，API 出错时中断，没有重试。
- 当前没有项目保存和加载功能；退出后内存数据丢失。
- `slide` 尚未实现流运算符或查找用的转换，不能直接用 `LinkList<slide>` 的 `Save/Load/Locate`。
