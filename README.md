# pptlinklist
a ppt agent that use linklist.

用 `LinkList<ppt>` 管理多个 PPT，每个 PPT 用 `LinkList<slide>` 管理页面。
`slide` 里的 `prompt` 存本页要求，`html` 暂时保留原名，实际存 DeepSeek 返回的 Python PPT 生成代码。

## 运行

使用 MSYS2 UCRT64 的 g++，需要 libcurl 和 nlohmann-json：

调用 DeepSeek 前设置 `DEEPSEEK_API_KEY` 环境变量，不要把密钥写入源码。
如果已经配置 Windows 用户环境变量，在当前 PowerShell 中读取：

```powershell
$env:DEEPSEEK_API_KEY = [Environment]::GetEnvironmentVariable("DEEPSEEK_API_KEY", "User")
```

也可以仅为当前终端设置 `$env:DEEPSEEK_API_KEY = "你的 API Key"`。
然后编译并运行：

```powershell
E:/msys64/ucrt64/bin/g++.exe -std=c++17 LinkList/Main.cpp -lcurl -o Main.exe
$env:PATH = "E:\msys64\ucrt64\bin;" + $env:PATH
.\Main.exe
```

生成 PPT 还需要 Python 和 python-pptx：

```powershell
py -3 -m pip install python-pptx
```

## 使用

1. 主菜单输入 `-1`，填写 PPT 名字和整体要求。
2. 输入 PPT 编号进入，输入 `-1` 添加一页，填写本页要求。
3. 输入页号进入单页菜单：`1` 修改要求，`2` 调用 DeepSeek 生成代码，`3` 查看代码，`0` 返回。
4. 所有页准备好后，在 PPT 菜单输入 `-3`，导出当前目录下的 `generate_ppt.py`。
5. 运行 `py -3 generate_ppt.py`，在脚本所在目录得到 `output.pptx`。

编号从 0 开始。未生成代码的页导出为空白页；修改要求会清空本页旧代码，需要重新生成。
当前只做最简单的流程，请输入有效编号。退出程序不会保存编辑数据，导出会覆盖同名脚本和 PPT。

## 十页测试数据与批量生成

`LinkList/testdata.h` 内置一份十页课程汇报：封面、项目背景、项目目标、整体架构、
双层链表、单页数据、编辑流程、AI 生成流程、批量生成与导出、总结与展望。
可以直接修改里面的 `prompts` 数组来换测试内容。

1. 主菜单输入 `-4`，加载十页测试 PPT（只添加数据，不调用 AI）。
2. 输入显示的 PPT 编号进入；刚启动时第一次加载的编号为 `0`。
3. 输入 `-5`，依次生成全部页面代码，界面会显示当前进度。
4. 生成完输入 `-3` 导出脚本，再运行 `py -3 generate_ppt.py`。

批量生成会按顺序调用十次 DeepSeek，并覆盖各页原来的代码；它不会自动导出文件。
当前版本没有失败重试，API 出错时会中断。

## 代码如何变成 PPT

`ppt::generate()` 要求 DeepSeek 返回一个 `add_slide(prs)` 函数，向已有的演示文稿添加一页。
`tool.h` 的 `exportppt()` 按链表顺序拼接每页代码并调用这个函数，最后保存 `.pptx`。
生成的 `.py` 是会实际执行的 Python 代码，可以先查看和修改再运行。
API 的模型名和地址在 `LinkList/ai.h` 中，认证密钥从 `DEEPSEEK_API_KEY` 环境变量读取。
