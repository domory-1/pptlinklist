# LinkList.h 接口文档

单向链表类模板，`#include "LinkList.h"` 即可使用，无需额外编译单元。

```cpp
LinkList<Slide> ppt;          // 例：每个结点存一页幻灯片
```

## 构造与赋值

| 接口 | 说明 |
|---|---|
| `LinkList(int n=0, const T *array=NULL)` | 构造 n 个结点的链表；array 非空时按数组初始化 |
| `LinkList(const LinkList &list)` | 深拷贝构造 |
| `~LinkList()` | 析构，自动释放所有结点 |
| `operator=(const LinkList &list)` | 深赋值，会同步 cur_node 位置 |

## 插入 / 追加（都会把 cur_node 移到新结点）

| 接口 | 说明 |
|---|---|
| `Insert(const T &t)` | 头插，成为新首结点 |
| `InsertBefore(const T &t)` | 插到当前结点之前（空表/无当前结点返回 NULL） |
| `InsertAfter(const T &t)` | 插到当前结点之后 |
| `Append(const T &t)` | 尾插，成为新尾结点 |

返回值均为 `Node<T> *`（新结点地址，失败为 NULL）。

## 删除 / 清空 / 修改

| 接口 | 说明 |
|---|---|
| `DeleteCurNode()` | 删当前结点，下一结点成为新当前结点，返回被删数据；空表 `throw -1` |
| `FreeList()` | 释放全部结点，head/cur_node 置 NULL，num 归 0 |
| `ModifyData(const T &t)` | 替换当前结点数据，返回旧值；无当前结点 `throw -1` |
| `CurData()` | 引用返回当前结点数据，可做左值；无当前结点 `throw -1` |

## 当前结点定位

| 接口 | 说明 |
|---|---|
| `GoTop()` | 当前结点 → 首结点 |
| `Go(int n)` | 当前结点 → 第 n 个结点（0 起），越界返回 NULL |
| `GoBottom()` | 当前结点 → 尾结点（⚠ 空链表调用会崩溃，见注意事项） |
| `Skip(int n=1)` | 相对移动 n 步，n 可为负；cur_node 为 NULL 时 `throw -1` |
| `CurPos()` | 当前结点序号（0 起），无当前结点返回 -1 |
| `CurNode()` | 当前结点地址，无则 NULL |

## 查找 / 排序 / 倒置

| 接口 | 说明 |
|---|---|
| `Locate<TYPE>(const TYPE &x, bool newsearch=false)` | 按条件定位：`TYPE(p->data)==x` 的结点；`newsearch=true` 从头找，否则接着上次继续找 |
| `Sort<TYPE>(const TYPE &x, bool ascending=true)` | 按 `TYPE(data)` 插入排序，ascending 升/降序 |
| `Reverse()` | 链表倒置，cur_node 指向不变 |

`TYPE` 用法：给元素类写类型转换运算符，例如 `Slide::operator string()` 返回标题，即可 `ppt.Locate(string("封面"), true)` 按标题查、`ppt.Sort(string(""))` 按标题排。

## 信息 / 输出 / 文件

| 接口 | 说明 |
|---|---|
| `NumNodes()` | 结点总数 |
| `ShowList()` | 单行打印：`head -> *cur -> ... -> NULL`（`*` 标当前结点） |
| `PutList(ostream &out)` | 输出 `(n)` + 每行一个数据到任意 ostream |
| `GetList(istream &in)` | 从 istream 读入（⚠ 追加模式，见注意事项） |
| `Save(const char *filename)` | 写入文件，返回结点数，失败返回 0 |
| `Load(const char *filename)` | 从文件读入，返回新增结点数，失败返回 0 |
| `operator<<` / `operator>>` | 流运算符，内部调 PutList/GetList |

## 对元素类型 T 的要求

- `operator<<`、`operator>>`：用 `Save/Load/PutList/GetList` 必需
- 默认构造 + 赋值：`new Node<T>` 需要
- 类型转换运算符（如 `operator string()`）：用 `Locate/Sort` 必需

## 注意事项（已实测的坑）

| 问题 | 位置 | 规避 |
|---|---|---|
| `GoBottom()` 空链表崩溃（NULL->next） | LinkList.h:365 | 调用前判 `NumNodes()>0` |
| `Locate` 的 static 变量在所有链表实例间共享 | LinkList.h:385 | 只用一条链表；多条需改成成员变量 |
| `GetList/Load` 是追加不清空（`FreeList()` 被注释） | LinkList.h:132 | 重新加载前先 `FreeList()` |
| 异常抛的是 `int`（`throw -1`） | 多处 | `catch(int)` 或避免空表调用 |
| `Go(n)` 边界写的 `n>num`（实际 n==num 也返回 NULL） | LinkList.h:351 | 行为正确，仅判断不严谨 |

## 典型用法（PPT 翻页模型）

```cpp
ppt.Append(Slide("封面"));          // 加页
ppt.Go(i);                          // 跳到第 i 页
ppt.Skip(1); / ppt.Skip(-1);        // 下/上一页
ppt.InsertAfter(Slide("新页"));     // 当前页后插页
ppt.DeleteCurNode();                // 删当前页
ppt.CurData() = newSlide;           // 改当前页
ppt.Save("ppt.txt");                // 存盘
```
