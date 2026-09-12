# Lab4：从 C 顺序表到 C++ 固定容量向量类

> **作业目标**：把 Lab3 中用 C 语言实现的固定容量顺序表，改写为一个 C++ 类 `IntVector`；能够看懂并使用 `class`、`public/private`、构造函数、成员函数和成员函数末尾的 `const`；亲手验证"算法一个字不变，只是代码组织方式变了"——插入、删除的移动过程和 Lab3 完全相同，C 中直接改 `list.size` 的写法在 C++ 中会被编译器拒绝。
>
> 本说明已包含完成本次作业所需的全部语法和概念，不需要参考其他材料。本次涉及的 C++ 语法只要求"看懂、会照着用"，不要求解释底层原理。本次仍是固定容量版本（数组大小为 `MAX_SIZE`），不涉及动态内存、`new`/`delete` 和扩容，这些将在后续课程和作业中出现。
>
> **审核说明**：本次 PR 自动审核会检查作业是否完整，包括必交文件是否存在、`CMakeLists.txt` 是否配置了 `lab4_cpp` 目标、`intvector.cpp` 是否是非空的完整程序并包含 `class IntVector` 和全部要求的成员函数、`Lab4.md` 中的选择题和填空是否已经作答。自动审核不判断选项、程序输出和文字回答是否正确，内容正确性将在期末统一检测。空文件、未作答的模板、只有框架没有实现的程序不能通过本次审核。

---

## 一、配置 CMakeLists.txt

在 CLion 中新建一个 C++ 项目，项目名称建议使用 `Lab4`。将自动生成的 `CMakeLists.txt` 改为下面的内容：

```cmake
cmake_minimum_required(VERSION 3.20)
project(Lab4 C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(lab4_cpp intvector.cpp)
```

要求：

- 有且只有一个 `add_executable`，目标名为 `lab4_cpp`，源文件为 `intvector.cpp`。
- `project(...)` 与 Lab3 保持一致，同时声明 `C` 和 `CXX`。
- 修改并保存后，在 CLion 中重新加载 CMake 项目（`File → Reload CMake Project`），确认运行按钮旁的下拉框中出现 `lab4_cpp` 并能运行。

> 与 Lab3 一样，本次需要提交 `CMakeLists.txt`。

---

## 二、完成本次作业所需的 C++ 知识（共 6 点，会用即可）

**1. `class` 与 `public` / `private`**：C 里用 `typedef struct { ... } SeqList;` 定义类型，数据放在里面、操作它的函数写在外面，任何代码都能直接改 `list.size`。C++ 的 `class` 把数据和操作写进同一个类型里：数据写在 `private:` 下面，叫**数据成员**，只有类自己的函数能读写；操作写在 `public:` 下面，叫**成员函数**，是外界使用这个类型的唯一入口。类定义末尾和结构体一样有分号。

```cpp
class IntVector {
private:
    int data[MAX_SIZE];   // 数据成员
    int size;

public:
    int getSize() const { // 成员函数
        return size;
    }
};
```

在类外面写 `v.size = 1000;` 会得到类似 `'size' is a private member of 'IntVector'` 的编译错误（不同编译器措辞略有差别）。这正是把 `size` 设为 `private` 的目的：所有对 `size` 的修改只能经过带边界检查的成员函数。

**2. 对象与成员函数调用**：`IntVector v;` 创建一个**对象**，`v` 和 `SeqList list;` 里的 `list` 一样是这个类型的变量。调用成员函数写 `对象.函数名(参数)`，点号左边的对象就是被操作的那个表，取代了 C 函数的第一个参数 `SeqList *list`。在成员函数内部，直接写 `size`、`data[i]` 就是当前对象的 `size`、`data[i]`，不再写 `list->`。

| C | C++ |
| :--- | :--- |
| `insertAt(&list, 2, 25)` | `v.insert(2, 25)` |
| `list->size`、`list->data[i]`（函数体内） | `size`、`data[i]` |

**3. 构造函数**：与类同名、没有返回类型（连 `void` 也不写）的成员函数，在 `IntVector v;` 创建对象时自动执行一次，不需要也不能手动调用。它的作用与 C 里的 `initList(&list)` 或手写 `list.size = 0` 相同，但不可能被忘记。

```cpp
IntVector() {
    size = 0;
}
```

**4. 成员函数末尾的 `const`**：只读不改的成员函数，在参数列表的右括号后面写 `const`，表示"本函数不修改本对象的 `data` 和 `size`"。如果在这样的函数里写了 `size++`，编译器会报错。本次作业中 `getSize`、`empty`、`get`、`find`、`print` 只读，带 `const`；`insert`、`remove` 会修改对象，不带。框架里已经写好，照用即可。

**5. 引用输出参数**：Lab3 的 `hello.cpp` 已经用过。C 版 `removeAt(SeqList *list, int index, int *removed)` 通过指针传回被删除的值，调用时传 `&removed`，函数内写 `*removed = ...`；C++ 版 `remove(int index, int& removed)` 用引用做同样的事，调用时直接传 `removed`，函数内直接写 `removed = ...`。引用一定绑定到一个真实变量，不需要判空。成功/失败用 `bool` 的 `true`/`false`，对应 C 的 `1`/`0`。

**6. 函数体写在哪里**：本次作业所有成员函数的函数体都直接写在类的大括号里面（框架已经给出位置）。把函数体写到类外面需要 `IntVector::` 前缀，课程尚未讲到，本次不要使用。

---

## 三、intvector.cpp：把 seqlist.c 改写为类

新建 `intvector.cpp`。Lab3 的 `seqlist.c` 里已经有 `printList`、`insertAt`、`removeAt` 三个函数，本次把它们连同课堂上讲过的初始化、按位置读取、按值查找一起，改写为 `IntVector` 类的成员函数。

> 改写的固定套路：去掉第一个参数 `SeqList *list`；函数体里的 `list->size`、`list->data[i]` 改为 `size`、`data[i]`；`int *` 输出参数改为 `int&`，`*value = ...` 改为 `value = ...`；返回 `int` 的 `1`/`0` 改为 `bool` 的 `true`/`false`。边界检查和元素移动的顺序一个字不改。

### 3.1 必须使用的类框架

下面的框架可以直接编译运行（输出全是错的），把每一处 `TODO` 换成正确实现即可。不得修改类名、数据成员、成员函数的名字、参数、返回类型和末尾的 `const`，不得增加数据成员。

```cpp
#include <iostream>

#define MAX_SIZE 100

class IntVector {
private:
    int data[MAX_SIZE];
    int size;

public:
    // 构造函数：建立空表，对应课堂上的 initList
    IntVector() {
        // TODO：把 size 设为 0
    }

    // 当前元素个数。size 是 private 的，类外只能通过它读取
    int getSize() const {
        return 0;   // TODO：改为返回 size
    }

    // 表是否为空
    bool empty() const {
        return false;   // TODO
    }

    // 练习 1：按位置读取，对应课堂上的 getAt
    // 成功返回 true 并把元素写入 value；index 不在 [0, size - 1] 内返回 false，且不修改 value
    bool get(int index, int& value) const {
        return false;   // TODO
    }

    // 练习 2：按值查找，对应课堂上的 findValue
    // 返回第一次出现的下标，未找到返回 -1
    int find(int value) const {
        return -1;   // TODO
    }

    // 练习 3：插入，对应 Lab3 的 insertAt
    // 成功返回 true；表已满（size == MAX_SIZE）或 index 不在 [0, size] 内返回 false
    bool insert(int index, int value) {
        return false;   // TODO
    }

    // 练习 4：删除，对应 Lab3 的 removeAt
    // 成功返回 true 并把被删除的值写入 removed；index 不在 [0, size - 1] 内返回 false，且不修改 removed
    bool remove(int index, int& removed) {
        return false;   // TODO
    }

    // 练习 5：输出，对应 Lab3 的 printList
    // 从下标 0 到 size - 1 依次输出所有有效元素，元素之间用一个空格分隔，末尾换行
    void print() const {
        // TODO
    }
};
```

### 3.2 实现要求

1. 八个成员函数全部实现，函数体直接写在类内。
2. `data` 和 `size` 必须保持 `private`，不得移到 `public`，不得删除 `private:`；类外（包括 `main`）不得直接访问 `data` 和 `size`（编译器也不会允许）。
3. `insert` 必须先判断"是否已满"和"下标是否合法"，再**从后向前**移动元素，最后写入新值并让 `size` 加 `1`；`remove` 必须先判断下标是否合法，先把被删除的值保存到 `removed`，再**从前向后**移动元素，最后让 `size` 减 `1`。移动方向与 Lab3 相同。
4. 失败时只返回 `false`，不能让程序崩溃，也不能修改 `data`、`size` 和输出参数。
5. 只能使用 `#include <iostream>`，输出用 `std::cout`；不得使用 `printf`、`std::vector`、`new`、`delete`、全局变量。
6. 成功/失败一律用 `bool`，输出参数一律用引用 `int&`，不得改成指针。

### 3.3 main 函数

`main` 函数必须**原样**使用下面的代码，放在类定义之后：

```cpp
int main() {
    IntVector v;
    std::cout << "空表: size = " << v.getSize() << ", empty = " << v.empty() << std::endl;

    v.insert(0, 10);
    v.insert(1, 20);
    v.insert(2, 30);
    v.insert(3, 40);
    std::cout << "初始: ";
    v.print();

    bool ok = v.insert(2, 25);
    std::cout << "insert(2, 25) 返回 " << ok << std::endl;
    v.print();

    int removed = 0;
    ok = v.remove(1, removed);
    std::cout << "remove(1) 返回 " << ok << ", 删除的值是 " << removed << std::endl;
    v.print();

    ok = v.insert(10, 99);
    std::cout << "insert(10, 99) 返回 " << ok << std::endl;
    v.print();

    ok = v.remove(4, removed);
    std::cout << "remove(4) 返回 " << ok << std::endl;
    v.print();

    int value = 0;
    ok = v.get(2, value);
    std::cout << "get(2) 返回 " << ok << ", value = " << value << std::endl;
    ok = v.get(4, value);
    std::cout << "get(4) 返回 " << ok << std::endl;

    std::cout << "find(30) = " << v.find(30) << std::endl;
    std::cout << "find(99) = " << v.find(99) << std::endl;
    std::cout << "最终: size = " << v.getSize() << ", empty = " << v.empty() << std::endl;

    IntVector other;
    other.insert(0, 7);
    std::cout << "other: ";
    other.print();
    std::cout << "两个对象的 size: " << v.getSize() << " " << other.getSize() << std::endl;

    return 0;
}
```

程序输出必须与下面完全一致（`bool` 输出时 `true` 显示为 `1`，`false` 显示为 `0`）：

```text
空表: size = 0, empty = 1
初始: 10 20 30 40
insert(2, 25) 返回 1
10 20 25 30 40
remove(1) 返回 1, 删除的值是 20
10 25 30 40
insert(10, 99) 返回 0
10 25 30 40
remove(4) 返回 0
10 25 30 40
get(2) 返回 1, value = 30
get(4) 返回 0
find(30) = 2
find(99) = -1
最终: size = 4, empty = 0
other: 7
两个对象的 size: 4 1
```

中间五行与 Lab3 的输出完全相同，这不是巧合：插入和删除的算法没有变。最后两行验证两个对象各自拥有独立的 `data` 和 `size`。

程序要求：

- 文件名必须是 `intvector.cpp`，全部小写。
- 输出内容必须由成员函数和返回值生成，不得把结果写死在 `std::cout` 中。
- 程序必须能使用 C++17 独立编译运行。

---

## 四、选择题与填空

本部分除标明为填空题外均为单项选择题。作答时将所选选项前的 `[ ]` 改为 `[x]`，每题只能勾选一项，不要删除其他选项。

### 4.1 `private` 的作用

在 `main` 中写 `IntVector v; v.size = 3;`，结果是：

- [ ] A. 编译通过，`size` 变为 `3`
- [ ] B. 编译错误，因为 `size` 是 `private` 成员，类外不能访问
- [ ] C. 编译通过，运行时报错
- [ ] D. 编译通过，但 `size` 保持不变

### 4.2 构造函数

关于框架中的 `IntVector()`，以下说法正确的是：

- [ ] A. 它必须写成 `void IntVector()`
- [ ] B. 需要在 `main` 中写 `v.IntVector();` 才会执行
- [ ] C. 它与类同名、没有返回类型，在 `IntVector v;` 创建对象时自动执行
- [ ] D. 它的作用与 Lab3 的 `printList` 相同

### 4.3 成员函数的调用

C 语言中的 `insertAt(&list, 2, 25)` 对应 C++ 中的：

- [ ] A. `insert(&v, 2, 25)`
- [ ] B. `v.insert(2, 25)`
- [ ] C. `v->insert(2, 25)`
- [ ] D. `IntVector.insert(2, 25)`

### 4.4 引用参数的调用

对于 `bool remove(int index, int& removed)`，正确的调用是：

- [ ] A. `v.remove(1, &removed)`
- [ ] B. `v.remove(1, removed)`
- [ ] C. `v.remove(1, *removed)`
- [ ] D. `v.remove(1)`

### 4.5 成员函数末尾的 `const`

`int find(int value) const` 末尾的 `const` 表示：

- [ ] A. 参数 `value` 不能被修改
- [ ] B. 返回值是常量
- [ ] C. 这个函数不修改当前对象的 `data` 和 `size`
- [ ] D. 这个函数只能被调用一次

### 4.6 为什么需要 `getSize()`

C 版本可以直接读 `list.size`，C++ 版本却要写 `getSize()`，原因是：

- [ ] A. C++ 不允许类里有 `int` 成员
- [ ] B. `size` 是 `private` 的，类外读不到，`getSize()` 提供一个只读的入口
- [ ] C. `getSize()` 比直接读更快
- [ ] D. 没有原因，只是习惯

### 4.7 两个对象

执行下面的代码后，`b.getSize()` 的值是：

```cpp
IntVector a;
IntVector b;
a.insert(0, 10);
```

- [ ] A. `1`
- [ ] B. `0`
- [ ] C. 不确定，取决于内存里原来的值
- [ ] D. 编译错误

### 4.8 算法与复杂度

C 版本的 `insertAt` 和 C++ 版本的 `insert` 相比，以下说法正确的是：

- [ ] A. C++ 版本插入不需要移动元素
- [ ] B. 两者的元素移动过程和时间复杂度完全相同，只是代码组织方式不同
- [ ] C. C++ 版本是 **`O(1)`**，C 版本是 **`O(n)`**
- [ ] D. C++ 版本必须从前向后移动

### 4.9 `get` 的返回值设计

`get` 用返回值表示成功或失败、用引用参数传回元素值，而不是"直接返回元素值，失败时返回 `-1`"，原因是：

- [ ] A. `-1` 本身可能就是表中的一个元素，无法区分"读到了 `-1`"和"读取失败"
- [ ] B. C++ 不允许函数返回负数
- [ ] C. 引用参数比返回值运行更快
- [ ] D. 这样可以少写一个 `if`

### 4.10 `find` 的返回值

执行 `v.find(30)` 返回 `0`，含义是：

- [ ] A. 没有找到 `30`
- [ ] B. `30` 在下标 `0` 处
- [ ] C. 读取失败
- [ ] D. 表为空

### 4.11 填空：程序运行验证

`intvector.cpp` 实际运行输出是否与第三部分给出的期望输出完全一致？

- [ ] A. 一致
- [ ] B. 不一致

如果选择"不一致"，请说明哪一行不同以及原因：

> 在此填写原因，选择"一致"时填写"无"。

### 4.12 填空：C 与 C++ 写法的对应关系

把右列补全，至少填写三行（可以全部填写）：

| C（Lab3 / 课堂） | C++（本次作业） |
| :--- | :--- |
| `SeqList list; list.size = 0;` | （在此填写） |
| `insertAt(&list, 2, 25)` | （在此填写） |
| `removeAt(&list, 1, &removed)` | （在此填写） |
| 函数体内的 `list->data[i]` | （在此填写） |
| 返回 `int` 的 `1` / `0` | （在此填写） |

### 4.13 填空：一句话总结

Lab3 的 `main` 里可以直接写 `list.size = 4;`。用一句话说明：为什么把 `size` 设为 `private` 之后，`insert`、`remove` 里的边界检查才真正"管用"？

> 在此填写你的回答（不少于 20 字）。

---

## 五、提交要求

将本文件复制到自己的 `学号姓名/Lab4/Lab4.md`，填写选择题答案和填空。最终只提交以下 3 个文件：

```text
学号姓名/
└── Lab4/
    ├── CMakeLists.txt
    ├── intvector.cpp
    └── Lab4.md
```

特别注意：

- `CMakeLists.txt` 必须包含 `lab4_cpp` 目标，源文件为 `intvector.cpp`。
- `intvector.cpp` 必须是能够独立编译运行的完整程序，包含 `class IntVector` 的完整实现和第三部分给出的 `main`。
- `Lab4.md` 必须保留题目结构，填写选择题答案和填空。
- 每道单项选择题必须且只能将一个选项的 `[ ]` 改为 `[x]`，不能全部不选，也不能同时选择多项。
- 三处填空位置不能留空；4.11 选择"一致"时填写"无"；4.12 至少填写三行。
- 自动审核只检查是否完成，不判断答案和程序结果是否正确；内容正确性将在期末统一检测。
- 不需要提交运行截图。
- 不要提交整个 CLion 项目，不要提交 `cmake-build-*`、`.idea/`、`.exe` 或其他编译产物。
- `Lab4`、`Lab4.md`、`CMakeLists.txt` 和 `intvector.cpp` 的大小写必须完全一致。
- PR 标题必须严格使用 `[学号姓名]Lab4作业提交`，右方括号后不能有空格。
- 一个 PR 只能包含本次 Lab4 的文件，不得修改其他同学、`homework/`、README 或仓库配置。

---

## 六、截止时间

**2026 年 9 月 17 日 24:00（即 2026 年 9 月 18 日 00:00，北京时间）**

以 GitHub 记录的最后一次向 PR 推送代码的时间为准。不晚于上述时刻创建 PR 并完成最后一次推送不算超时；超过该时刻新建 PR，或向已有 PR 推送任何修改，均算作超时。审核未通过的同学请务必在截止前完成修改。
