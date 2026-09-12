# Lab3：C++ 初体验与固定容量顺序表

> **作业目标**：能够看懂并修改最简单的 `CMakeLists.txt`，在一个项目里同时配置 C 和 C++ 两个可执行文件；能够看懂并使用 `std::cout`、`bool` 和引用参数；理解"数据 + 对数据的操作"共同构成数据结构，并用 C 语言把顺序表的遍历、插入、删除写成代码，说清楚元素移动的顺序和下标合法性。
>
> 本说明已包含完成本次作业所需的全部语法和概念，不需要参考其他材料。本次作业涉及的 C++ 语法只要求"看懂、会照着用"，不要求解释底层原理。顺序表的完整接口（初始化、查找、清空等）将在后续课程中实现，本次只做遍历、插入、删除三个操作。
>
> **审核说明**：本次 PR 自动审核会检查作业是否完整，包括必交文件是否存在、`CMakeLists.txt` 是否配置了两个可执行文件、`hello.cpp` 和 `seqlist.c` 是否是非空的完整程序、`Lab3.md` 中的选择题和填空是否已经作答。自动审核不判断选项、程序输出和文字回答是否正确，内容正确性将在期末统一检测。空文件、未作答的模板或明显未完成的程序不能通过本次审核。

---

## 一、配置 CMakeLists.txt：一个项目两个程序

在 CLion 中新建一个项目（C 或 C++ Executable 均可），项目名称建议使用 `Lab3`。将自动生成的 `CMakeLists.txt` 改为下面的内容，让同一个项目里同时生成一个 C 程序和一个 C++ 程序：

```cmake
cmake_minimum_required(VERSION 3.20)
project(Lab3 C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(lab3_cpp hello.cpp)
add_executable(lab3_c seqlist.c)
```

要求：

- `project(...)` 必须同时声明 `C` 和 `CXX` 两种语言，否则 `.c` 或 `.cpp` 中的一种无法编译。
- 必须有且只有两个 `add_executable`，目标名分别为 `lab3_cpp` 和 `lab3_c`，源文件分别为 `hello.cpp` 和 `seqlist.c`。
- 目标名不要和源文件同名，也不要带 `.c`/`.cpp` 后缀。
- 修改并保存后，在 CLion 中重新加载 CMake 项目（`File → Reload CMake Project`），确认运行按钮旁的下拉框中出现 `lab3_cpp` 和 `lab3_c` 两个目标，并且两个目标都能分别运行。

> 与 Lab1、Lab2 不同，本次作业**需要提交 `CMakeLists.txt`**，因为它本身就是本次作业的学习内容。

---

## 二、hello.cpp：第一个 C++ 程序

新建 `hello.cpp`，程序必须完成以下三件事，并且按顺序输出：

1. 用 `std::cout` 输出自己的姓名和学号，各占一行；
2. 定义一个 `bool` 变量 `isStudent`，赋值为 `true`，并输出它的值；
3. 实现下面的引用参数函数 `bool getDoubled(int value, int& result)`，在 `main` 中用自己学号的**最后两位**作为 `value` 调用它，并输出结果。

### 完成本节所需的 C++ 知识（共 3 点，会用即可）

**1. `std::cout` 输出**：C++ 用 `std::cout` 配合 `<<` 把内容"推"到屏幕上，可以连续拼接，不需要像 `printf` 那样写 `%d` 格式符；`std::endl` 用来换行，效果类似 `\n`。示例：

```cpp
#include <iostream>

int main() {
    int score = 90;
    std::cout << "成绩是 " << score << std::endl;
    return 0;
}
```

`std::` 前缀现阶段不需要理解，只需照抄。

**2. `bool` 类型**：C++ 的 `bool` 只有 `true` / `false` 两个取值，语义是"是/否"，比"用 `int` 的 `1` / `0` 表示成功/失败"更直接。用 `std::cout` 直接输出 `bool` 时，`true` 显示为 `1`，`false` 显示为 `0`。示例：

```cpp
#include <iostream>

bool isPositive(int value) {
    if (value > 0) {
        return true;
    }
    return false;
}

int main() {
    int x = 5;
    bool ok = isPositive(x);
    std::cout << x << " 是正数吗? " << ok << std::endl;
    return 0;
}
```

**3. 引用参数（本次唯一的"新"语法）**：函数有时需要把计算结果"传回去"给调用者。C 语言的做法是用指针做输出参数，调用时传地址、函数内解引用赋值；C++ 里可以用引用做同样的事，写法更简洁。对照如下：

```c
// C 版本：用指针把结果"传回去"
int getDoubled(int value, int *result) {
    if (result == NULL) {
        return 0;  // 失败
    }
    *result = value * 2;  // 必须解引用才能写回
    return 1;  // 成功
}

// 调用时必须取地址：getDoubled(input, &output);
```

```cpp
// C++ 版本：用引用做同样的事（本次作业要求实现这个版本）
bool getDoubled(int value, int& result) {
    result = value * 2;  // 直接赋值即可写回调用者的变量
    return true;
}

// 调用时直接传变量名：getDoubled(input, output);
```

两者的对应关系：参数写法 `int *result` 对应 `int& result`；调用时 `&output` 对应 `output`；函数内 `*result = ...` 对应 `result = ...`。引用一定绑定到一个有效变量，不会是"空的"，所以不需要判空；它可以直接修改调用者的变量。现阶段只要求看懂并会照着调用，不要求解释底层实现。

本次作业中 `getDoubled` 的规定：计算 `value * 2` 存入 `result`，返回 `true`；第二个参数必须是引用 `int&`。

程序输出应类似（以姓名张三、学号 20230001 为例）：

```text
姓名: 张三
学号: 20230001
是否为学生: 1
1 的两倍是 2
```

程序要求：

- 文件名必须是 `hello.cpp`，全部小写。
- 只能使用 `#include <iostream>`，不能使用 `printf`。
- 输出成功/失败的类型必须是 `bool`，不能用 `int` 代替。
- `getDoubled` 的第二个参数必须是引用 `int&`，不能改成指针，调用时直接传变量名，不取地址。
- 姓名和学号必须是自己的真实信息，不能照抄示例。

---

## 三、seqlist.c：顺序表的遍历、插入、删除

新建 `seqlist.c`，用 C 语言实现一个**固定容量**顺序表，容量为 `MAX_SIZE`。

> 背景：C 语言的数组只是一块连续内存，它自己不知道"装了几个有效元素"，元素个数只能由程序员另用一个变量（如 `count`）手动维护，容易忘记更新、容易传错。把"存放数据的数组"和"记录元素个数的变量"打包进同一个结构体统一管理，就是顺序表的基本思路：**数据 + 对数据的操作，合起来才是一个完整的数据结构**。打包之后，任何操作顺序表的函数都通过 `SeqList*` 参数来访问和修改它们，而不再零散地传两个变量；当前有多少个有效元素，直接读 `size` 字段即可。

### 3.1 结构体定义

必须使用下面给出的结构体，不得增删字段：

```c
#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int size;
} SeqList;
```

### 3.2 需要实现的三个函数

```c
// 练习 1：从下标 0 到 size - 1 依次输出所有有效元素，元素之间用一个空格分隔，末尾换行
void printList(const SeqList *list);

// 练习 2：在下标 index 处插入 value，成功返回 1，失败返回 0
// 失败情形：顺序表已满（size == MAX_SIZE），或 index 不在 [0, size] 范围内
int insertAt(SeqList *list, int index, int value);

// 练习 3：删除下标 index 处的元素，并通过指针参数 removed 把被删除的值传回调用者
// 成功返回 1，失败返回 0；失败情形：index 不在 [0, size - 1] 范围内
int removeAt(SeqList *list, int index, int *removed);
```

实现要求：

1. 三个函数**必须通过 `SeqList*` 参数**访问和修改 `data` 与 `size`，不能使用全局变量，也不能把 `data` 和 `size` 拆成两个参数分开传。
2. `insertAt` 必须先判断"是否已满"和"下标是否合法"，再移动元素；元素移动必须**从最后一个有效元素开始、从后向前**逐个后移，最后再写入新值，并让 `size` 加 `1`。
3. `removeAt` 必须先判断下标是否合法，再让被删元素后面的元素**从前向后**逐个前移，并让 `size` 减 `1`；被删除的值必须先保存到 `*removed`，再移动元素。
4. `removeAt` 的第三个参数是 C 语言的指针输出参数：函数把被删除的值写入 `*removed` 传回给调用者（用法与第二节 C 版本 `getDoubled` 中的 `int *result` 相同），调用时要传地址（如 `&removed`），函数内先判空再解引用赋值。
5. 三个函数在 C 语言中用 `int` 表示成功（`1`）和失败（`0`），这与 `hello.cpp` 中用 `bool` 的写法形成对照。
6. 只能使用 `stdio.h`，不得使用 C++ 的 `iostream`、`cout`、`bool`（`<stdbool.h>` 也不要用，本次故意和 C++ 版本对照）。

### 3.3 main 函数

`main` 函数必须按顺序完成以下操作，并在每一步之后调用 `printList` 输出当前内容：

1. 定义 `SeqList list;`，手动把 `size` 设为 `4`，`data[0..3]` 依次设为 `10, 20, 30, 40`，然后输出；
2. 调用 `insertAt(&list, 2, 25)`，输出返回值和顺序表内容；
3. 定义 `int removed;`，调用 `removeAt(&list, 1, &removed)`，输出返回值、被删除的值和顺序表内容；
4. 调用 `insertAt(&list, 10, 99)`（下标不合法），输出返回值和顺序表内容，确认顺序表没有被改动；
5. 调用 `removeAt(&list, 4, &removed)`（此时 `size` 为 `4`，下标 `4` 不合法），输出返回值和顺序表内容，确认顺序表没有被改动。

程序输出必须与下面完全一致：

```text
初始: 10 20 30 40
insertAt(2, 25) 返回 1
10 20 25 30 40
removeAt(1) 返回 1, 删除的值是 20
10 25 30 40
insertAt(10, 99) 返回 0
10 25 30 40
removeAt(4) 返回 0
10 25 30 40
```

程序要求：

- 文件名必须是 `seqlist.c`，全部小写。
- 输出内容必须由 `printList` 和函数返回值生成，不得把结果写死在 `printf` 中。
- 程序必须能使用 C11 或更高版本独立编译运行。
- 下标不合法或顺序表已满时，函数只返回 `0`，不能让程序崩溃，也不能修改 `data` 和 `size`。

---

## 四、选择题与填空

本部分除标明为填空题外均为单项选择题。作答时将所选选项前的 `[ ]` 改为 `[x]`，每题只能勾选一项，不要删除其他选项。

### 4.1 CMakeLists.txt 中最关键的一行

在本次作业的 `CMakeLists.txt` 中，`add_executable(lab3_c seqlist.c)` 这一行的含义是：

- [ ] A. 声明本项目至少需要 CMake 3.20 版本
- [x] B. 用源文件 `seqlist.c` 生成一个名为 `lab3_c` 的可执行程序
- [ ] C. 用源文件 `lab3_c` 生成一个名为 `seqlist.c` 的可执行程序
- [ ] D. 把 `seqlist.c` 编译为 C++ 程序

### 4.2 两个可执行文件之间的关系

一个 `CMakeLists.txt` 中写了两个 `add_executable`，以下说法正确的是：

- [ ] A. 两个程序会合并成一个程序，`main` 函数会执行两次
- [x] B. 每个 `add_executable` 对应一个可以单独运行的程序，内部逻辑互不影响
- [ ] C. 只有第一个 `add_executable` 有效，第二个会被忽略
- [ ] D. 两个源文件必须是同一种语言，不能一个 `.c` 一个 `.cpp`

### 4.3 `bool` 的输出

在 `hello.cpp` 中执行 `bool isStudent = true; std::cout << isStudent;`，屏幕上显示的是：

- [ ] A. `true`
- [x] B. `1`
- [ ] C. `yes`
- [ ] D. 编译错误，`bool` 不能直接输出

### 4.4 指针参数与引用参数

比较 `int getDoubled(int value, int *result)`（C 版本）和 `bool getDoubled(int value, int& result)`（C++ 版本），以下说法正确的是：

- [ ] A. 两个版本调用时都必须写成 `getDoubled(input, &output)`
- [x] B. C 版本函数内写 `*result = value * 2`，C++ 版本函数内写 `result = value * 2`
- [ ] C. 引用参数可能是空的，所以 C++ 版本也需要判断 `result == NULL`
- [ ] D. 引用参数不能修改调用者的变量，只能读取

### 4.5 容量与元素个数

对于本次作业的 `SeqList`，初始化后 `size` 为 `4`，以下说法正确的是：

- [ ] A. 容量为 `4`，元素个数为 `100`
- [x] B. 容量为 `100`，元素个数为 `4`，`data[4]` 到 `data[99]` 中的内容是无效的
- [ ] C. 容量和元素个数都是 `4`
- [ ] D. 容量和元素个数都是 `100`

### 4.6 插入时的移动顺序

在 `10 20 30 40`（`size = 4`）的下标 `2` 处插入 `25`，正确的元素移动顺序是：

- [ ] A. 先 `data[2] = data[3]`，再 `data[3] = data[4]`，最后 `data[2] = 25`
- [ ] B. 先 `data[3] = data[2]`，再 `data[4] = data[3]`，最后 `data[2] = 25`
- [x] C. 先 `data[4] = data[3]`，再 `data[3] = data[2]`，最后 `data[2] = 25`
- [ ] D. 直接 `data[2] = 25`，不需要移动其他元素

### 4.7 删除时的移动顺序

在 `10 20 25 30 40`（`size = 5`）中删除下标 `1` 处的元素，正确的元素移动顺序是：

- [x] A. 先 `data[1] = data[2]`，再 `data[2] = data[3]`，最后 `data[3] = data[4]`，然后 `size` 减 `1`
- [ ] B. 先 `data[3] = data[4]`，再 `data[2] = data[3]`，最后 `data[1] = data[2]`，然后 `size` 减 `1`
- [ ] C. 把 `data[1]` 设为 `0`，`size` 不变
- [ ] D. 把 `data[4]` 移到 `data[1]`，然后 `size` 减 `1`

### 4.8 下标合法性

当前顺序表 `size = 4`，以下说法正确的是：

- [ ] A. 访问 `data[4]` 是合法的，因为数组容量是 `100`
- [x] B. 访问时合法下标是 `0` 到 `3`；插入时合法下标是 `0` 到 `4`，因为可以插到末尾
- [ ] C. 插入和访问的合法下标都是 `0` 到 `3`
- [ ] D. 下标 `-1` 表示最后一个元素，是合法的

### 4.9 时间复杂度

以元素的赋值（移动）作为基本操作，对元素个数为 `n` 的顺序表，以下说法正确的是：

- [x] A. 按下标访问一个元素是 **`O(1)`**，在中间位置插入一个元素最坏情况是 **`O(n)`**
- [ ] B. 按下标访问一个元素是 **`O(n)`**，在中间位置插入一个元素是 **`O(1)`**
- [ ] C. 两者都是 **`O(1)`**
- [ ] D. 两者都是 **`O(n²)`**

### 4.10 为什么要把 `data` 和 `size` 打包

如果只用一个裸数组 `int data[100]` 加一个散落在外的计数变量 `count` 来管理数据，容易忘记更新 `count`、传错或传漏参数。把 `data` 和 `size` 放进同一个结构体的主要目的是：

- [ ] A. 让数组可以自动扩容
- [ ] B. 让程序运行更快
- [x] C. 把"数据"和"元素个数"绑定在一起统一管理，避免 `count` 散落在各处、忘记更新
- [ ] D. 让数组可以存放不同类型的元素

### 4.11 填空：程序运行验证

`seqlist.c` 实际运行输出是否与第三部分给出的期望输出完全一致？

- [x] A. 一致
- [ ] B. 不一致

如果选择"不一致"，请说明哪一行不同以及原因：

> 在此填写原因，选择"一致"时填写"无"。

无

### 4.12 填空：一句话总结

用一句话说明：为什么在 `insertAt` 中元素要**从后向前**移动，而在 `removeAt` 中要**从前向后**移动？如果方向反了会发生什么？

> 在此填写你的回答（不少于 20 字）。

insertAt从后向前移动是为了不覆盖后面尚未移动的元素；removeAt从前向后移动是为了不覆盖前面尚未移动的元素。方向反了会导致数据被覆盖丢失。

---

## 五、提交要求

将本文件复制到自己的 `学号姓名/Lab3/Lab3.md`，填写选择题答案和填空。最终只提交以下 4 个文件：

```text
学号姓名/
└── Lab3/
    ├── CMakeLists.txt
    ├── hello.cpp
    ├── seqlist.c
    └── Lab3.md
```

特别注意：

- `CMakeLists.txt` 必须包含 `lab3_cpp` 和 `lab3_c` 两个可执行目标，且 `project` 同时声明 `C` 和 `CXX`。
- `hello.cpp` 和 `seqlist.c` 必须是能够独立编译运行的完整程序。
- `Lab3.md` 必须保留题目结构，填写选择题答案和填空。
- 每道单项选择题必须且只能将一个选项的 `[ ]` 改为 `[x]`，不能全部不选，也不能同时选择多项。
- 两处填空位置不能留空；4.11 选择"一致"时填写"无"。
- 自动审核只检查是否完成，不判断答案和程序结果是否正确；内容正确性将在期末统一检测。
- 不需要提交运行截图。
- 不要提交整个 CLion 项目，不要提交 `cmake-build-*`、`.idea/`、`.exe` 或其他编译产物。
- `Lab3`、`Lab3.md`、`CMakeLists.txt`、`hello.cpp` 和 `seqlist.c` 的大小写必须完全一致。
- PR 标题必须严格使用 `[学号姓名]Lab3作业提交`，右方括号后不能有空格。
- 一个 PR 只能包含本次 Lab3 的文件，不得修改其他同学、`homework/`、README 或仓库配置。

---

## 六、截止时间

**2026 年 9 月 14 日 24:00（即 2026 年 9 月 15 日 00:00，北京时间）**

以 GitHub 记录的最后一次向 PR 推送代码的时间为准。不晚于上述时刻创建 PR 并完成最后一次推送不算超时；超过该时刻新建 PR，或向已有 PR 推送任何修改，均算作超时。审核未通过的同学请务必在截止前完成修改。