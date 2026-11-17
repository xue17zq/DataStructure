# Lab7：从 Lab6 的 C 链表到课本版 List<T>

> **作业目标**：把 Lab6 里那份纯 C 的带头哨兵单链表，按课堂上讲的路线改成课本 LinkedList 那一章的样子。本次只要抓住两块核心知识：**① 双向链表**——结点从一根指针变成 `pred` + `succ`，头哨兵旁边再加一个尾哨兵；**② C++ 基础语法**——`template` 模板、头文件拆分与 `#include`、`private` / `protected` / `public` 权限分离、构造与析构函数。八个自由函数装进 `List<T>` 类，代码拆成两个头文件加一个测试程序。课堂上已经写过的 `insertAsPred`、`insertAsSucc`、构造与析构的骨架由教师提供，学生补的是接链、断链、查找、按秩那一批核心代码。本次不涉及迭代器、拷贝构造、排序和模板特化。
>
> **检查说明**：程序和书面题沿用前几次作业的完成度检查口径，内容正确性将在期末统一检测。空文件、未作答模板和占位程序不属于完成；自动审核规则以课程仓库发布的 Lab7 配置为准。

---

## 一、配置 CMakeLists.txt

保留 Lab6，在新的 `Lab7` 目录中完成本次作业。本次只有一个程序，所以只有一个目标，在 CLion 右上角选择 `lab7_list` 运行。

```cmake
cmake_minimum_required(VERSION 3.20)
project(Lab7 C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(lab7_list main.cpp)
```

要求：

- 有且只有一个 `add_executable`，目标名 `lab7_list`，源文件 `main.cpp`。
- `project(...)` 与前几次作业一致，同时声明 `C` 和 `CXX`，并按 C++17 编译。
- 三个文件 `listNode.h`、`list.h`、`main.cpp` 与 `CMakeLists.txt` 放在**同一个目录**里。`list.h` 用 `#include "listNode.h"` 引入结点，`main.cpp` 用 `#include "list.h"` 引入表；头文件不要列进 `add_executable`，它们由 `#include` 引进来。
- 保存后重新加载 CMake 项目，本次仍然需要提交 `CMakeLists.txt`。

---

## 二、完成本次作业所需的新知识

### 2.1 五处结构性变化，一次说清

Lab6 的 `linkedlist.c` 是一条"表 + 八个自由函数"的路子。课堂上把它改成课本的样子，一共动五处：

| | Lab6 的 C 版 | 本次的课本版 |
| :--- | :--- | :--- |
| ① 结点 | `typedef struct Node { int data; struct Node* next; } Node;` 一根向后的绳子 | `template <typename T> struct ListNode { T data; ListNode<T>* pred; ListNode<T>* succ; };` 前后各一根 |
| ② 哨兵 | 只有一个头哨兵 `head`，表尾是 `NULL` | 头哨兵 `header` 加尾哨兵 `trailer`，表尾也是一个真结点 |
| ③ 代码组织 | 结构体 + 八个自由函数，每个都要收一个 `LinkedList*` | 一个 `class List`，函数变成成员函数，`this` 就是 C 版的 `list` |
| ④ 类型 | `int` 写死在 22 行里，想存 `double` 就得复制一份文件 | `T` 先空着，`List<int>`、`List<double>` 各自动生成一份 |
| ⑤ 文件 | 全部塞在一个 `.c` 里 | 拆成 `listNode.h`、`list.h` 加一个 `main.cpp` |

函数名字的对应关系，本次要逐条记住（4.7 填空题要用到）：

| Lab6 的 C 版 | 本次的课本版 |
| :--- | :--- |
| `LinkedList list; listInit(&list);` | `List<int> list;`（构造函数自动跑） |
| `listDestroy(&list);`（`main` 结尾自己写） | 不用写（析构函数自动跑） |
| `listSize(&list)` | `list.size()` |
| `listEmpty(&list)` | `list.empty()` |
| `listPrint(&list)` | `list.print()` |
| `listPushBack(&list, v)` | `list.insertAsLast(v)` |
| `listPushFront(&list, v)` | `list.insertAsFirst(v)` |
| `listInsert(&list, r, v)` | `list.insert(r, v)` |
| `listRemove(&list, r, &v)` | `list.remove(r, v)`（`v` 是引用） |
| `listGet(&list, r, &v)` | `list.get(r, v)`（`v` 是引用） |
| `listFind(&list, v)` 返回秩 | `list.find(v)` 返回位置，再用 `list.rankOf(p)` 折算成秩 |

### 2.2 双向链表：两个指针、两个哨兵

```cpp
template <typename T>
struct ListNode {
    T data;

    ListNode<T>* pred;      /* 前驱：Lab6 的 C 版没有这一项 */
    ListNode<T>* succ;      /* 后继：Lab6 里叫 next */
};
```

`next` 改名 `succ`，是为了和 `pred` 配对（`succ` 是 successor）。多出来的 `pred` 就是那根向前的绳子：

```text
Lab6 的结点：只有一根向后的绳子

    head --> [18] --> [-1] --> [42] --> NULL
    每个结点只知道"后面是谁"，不知道"前面是谁"

课本的结点：前后各一根，并且两头都站着哨兵

    header <--> [18] <--> [-1] <--> [42] <--> trailer
    任何一个有效结点，往前一步、往后一步都走得了
```

这笔改动要花掉 8 个字节，`main.cpp` 里第一句就把它打出来：`sizeof(ListNode<int>)` 是 **24**（`int data` 4 字节 + 对齐补 4 字节 + `pred` 8 字节 + `succ` 8 字节），`sizeof(List<int>)` 也是 **24**（`List` 自己只存两个指针和一个计数）。C 版的 `sizeof(Node)` 是 16，多的那 8 个字节就是 `pred`。

换来三件事：

1. **删得快**。`remove(p)` 里断链要读 `p->pred`，一读就有，不用再从头找前驱——C 版删第 8 个得先走 7 步。
2. **能往前插**。`insertAsPred` 能存在，就是因为有 `pred`；C 版只有 `insertAfter`，只能往后插。
3. **表尾一句话**。`last()` 就是 `trailer->pred`，`insertAsLast` 是 `O(1)`；C 版的 `listPushBack` 要先把哨兵走到表尾，是 `O(n)`。

**在 `p` 之前插入 `q`**（`p` 可以是任何有效结点，也可以是 `trailer`），三步：

```cpp
ListNode<T>* x = new ListNode<T>(e, pred, this);   /* 新结点先抓住两边 */
pred->succ = x;                                    /* 老前驱的后继改成 x */
pred = x;                                          /* this 的前驱改成 x */
```

判据只有一条：**后一句会不会改掉前一句要读的那个成员。** 第一句 `pred->succ = x` 要读 `pred`，最后一句 `pred = x` 恰好把 `pred` 改掉，所以"新结点先抓住两边"必须在前。把后两句颠倒，第二句先跑，`pred` 已经变成 `x`，第一句就成了 `x->succ = x`，新结点自己指向自己。

**摘掉有效结点 `p`**，两句断链，然后 `delete`：

```cpp
p->pred->succ = p->succ;        /* p 的前驱直接指向 p 的后继 */
p->succ->pred = p->pred;        /* p 的后继直接指向 p 的前驱 */
delete p;                       /* new 出来的必须 delete，不能用 free */
```

这两句**可以**颠倒，因为两句都只读 `p->pred`、`p->succ`，谁都不去改它们。接链两句不能颠倒、断链两句可以颠倒，两处互为反例，判据是同一条。4.3 题考的就是这个判据。

### 2.3 模板：把 `int` 换成一个名字 `T`

Lab6 的 `linkedlist.c` 把 `int` 写死在 22 行里。想存 `double`，就得把整个文件复制一份、把这 22 行一个个换掉。C 里没有语法能把这个 `int` 变成"先空着，用的时候再填"。

```cpp
template <typename T>      /* 读作：下面要用的这个 T，代表一个「类型」 */
struct ListNode { T data; ListNode<T>* pred; ListNode<T>* succ; };

template <typename T>
class List { ... };
```

用的时候把名字填进去，编译器顺手各生成一份：

```cpp
List<int> list;        /* 把 T 换成 int */
List<double> nums;     /* 同一个类，源码一行都不用改 */
```

三条规矩要记住。

**第一，`template <typename T>` 这一行只管紧跟它的那一个定义，不是管到文件末尾。** 文件里有几个类外定义的成员函数，就要写几遍。数一下本次的两个头文件：`list.h` 里类外定义的成员函数一共 16 个，所以这一行在 `list.h` 里一共出现 **17 次**，多出来的那一次是给 `class List` 自己的；`listNode.h` 里出现 **3 次**，一次给 `struct ListNode`，两次给 `insertAsPred`、`insertAsSucc`。

**第二，类外定义一个模板的成员函数 = 前面一行 `template <typename T>` 加名字里的 `List<T>::`，少一样都编不过。** 漏写 `<T>` 报 `use of class template 'List' requires template arguments`；漏写 `template` 那一行报 `use of undeclared identifier 'T'`。两条报错的箭头都指在 `List` 后面那一截，位置一模一样，所以看到这两句先回头看上一行。

**第三，模板的定义（那些函数体）必须整个写在头文件里。** 不能像普通函数那样"声明放 `.h`、定义放 `.cpp`"。真机原话是**链接错误**，不是语法错误：

```text
Undefined symbols for architecture arm64:
  "List<int>::print() const", referenced from:
      _main in main.o
```

原因就是模板是编译期的模子：编译器得先看见完整的模子，才能在你写 `List<int>` 的时候生成 `int` 版的代码。看不见模子，它就什么都没生成，最后链接时凑不齐这个函数。4.5 题考的就是这件事。

顺带说清 `typename` 这个词。`typename` 不是唯一写法，`template <class T>` 在模板参数表里和它**一模一样**，编出来的结果也一模一样；`class` 在这里只是历史上的一个用词不当，学生在别人的代码里看到不用慌。本次一律用 `typename T`。

### 2.4 头文件拆分与引入：`listNode.h`、`list.h`、`main.cpp`

课本把结点和表分成两个文件，有两个理由：

1. 两样东西改动频率差得远。结点是最后一块砖，以后栈、队列、树、图都要拿它去搭，它自己就能单独用；`List` 是要一直长的，还要往里加东西。
2. 2.3 第三条把话说死了：模板的定义必须让使用它的地方看得见，所以函数体只能待在头文件里。既然都在头文件里，就没道理把几百行塞进一个文件。

依赖方向只能是单向的：

```text
main.cpp --include--> list.h --include--> listNode.h
(测试程序)             (表 List)          (结点 ListNode)

反过来：listNode.h 完全不知道世上有 List 这个东西。
```

三个文件里**没有 `list.cpp`**。原因还是 2.3 第三条：全部函数体都在头文件里，没有东西可以留给 `.cpp` 去放。

**头文件必须加卫士。**

```cpp
#ifndef LISTNODE_H
#define LISTNODE_H

/* 全部内容 */

#endif
```

把它去掉，在 `main.cpp` 里把 `#include "listNode.h"` 写两遍就会报 `error: redefinition of 'ListNode'`。有卫士的时候，第二次包含被跳过，编译运行一切照常。真实项目里 `a.h` 包含 `b.h`、`b.h` 又包含 `a.h` 很常见，所以每个头文件都要有。

**`#include` 的两种写法不是风格问题。**

```cpp
#include <iostream>      /* 尖括号：去系统目录里找，用来包含标准库 */
#include "listNode.h"    /* 引号：先到我自己的目录里找，用来包含自己写的头文件 */
```

写反了，换台机器、换个目录就编不过。

**头文件里绝对不要写 `using namespace std;`。** 这句写在 `.cpp` 里，影响范围只有那一个文件；写在头文件里，凡是 `#include` 了它的文件全被连坐，而且报错报在别人的代码上——比如有人在自己代码里定义了一个 `struct pair`，报错会指向**用** `pair` 的那一行，第二条候选还来自标准库。所以本次 `list.h` 里输出一律写 `std::cout`，一个字都不省：多敲五个字符，换的是"谁包含我，谁就安全"。

### 2.5 权限分离与构造析构

C 版的 `LinkedList` 里"别动 `head`"只能写在注释里，编译器一个字都不说：任何一个拿到 `list` 指针的地方都能顺手写 `list->head = ...`。到了类里，这句话变成编译器认的规矩。

本次的 `List` 按三档分开：

| 分区 | 本次有什么 | 谁能碰 |
| :--- | :--- | :--- |
| `private:` | `_size`、`header`、`trailer` | 只有 `List` 自己的成员函数。类外面的代码写 `list.header` 或 `list._size` 直接编译不过 |
| `protected:` | `init()`、`clear()` | 自己，以及将来继承 `List` 的派生类。它们是内部维护动作，用 `List` 的人不需要、也不该直接调用 |
| `public:` | `List()`、`~List()`、`size()`、`empty()`、`first()`、`last()`、`valid()`、`insertAsFirst()`、`insertAsLast()`、`insertA()`、`insertB()`、`remove()`、`find()`、`operator[]()`、`rankOf()`、`insert(r,e)`、`remove(r,e)`、`get(r,e)`、`print()`、`printReverse()` | 任何人 |

为什么 `init()` 和 `clear()` 放 `protected` 而不是 `private`：前者被构造函数调用，后者被析构函数调用，都在类内部；但如果将来有"清空但保留对象"的派生需求（比如一个会自动复用空间的表），派生类也该能用它们，所以留一档而不是关死。

两句和 C 版对照的话要记住：

- `size() const` 末尾那个 `const`，等于 C 版的 `const LinkedList* list`：意思是"这个函数不会改对象"。C 版靠调用方在参数上写 `const`，现在是函数自己声明。
- `_size` 前面那个下划线不是装饰。同一个类里不能既有数据成员 `size`、又有成员函数 `size()`，`size()` 这个名字要留给外面调用，所以数据成员只能改名。`header`、`trailer` 跟任何成员函数都不重名，带不带下划线都能编过，所以两档要分清：`_size` 是**必须**，`header` / `trailer` 是**风格**。

**构造与析构：C 版记得调用的两件事，现在不用记得。**

C 版是两个自由函数，一个负责开头，一个负责结尾，`main` 里要自己写、自己排顺序（`listInit(&list);` … `listDestroy(&list);`）。本次这两件事交给对象自己：

```cpp
List() { init(); }      /* 构造函数：C 版的 listInit */
~List();                /* 析构函数：C 版的 listDestroy，里面先 clear() 再删两个哨兵 */
```

`List<int> list;` 这一句里构造函数自动跑，两个哨兵就立好了；`list` 离开作用域时析构函数自动跑，中间的有效结点被 `clear()` 一个个删掉，最后才释放两个哨兵。**调用方再也没有"记得调用"这个义务。**

两处细节要认真看：

- `~List()` 里第一句 `clear()` 不能省。它是循环 `remove(header->succ)`，把中间的结点一个个删掉；只写后面那两句 `delete`，末端的两个哨兵是还回去了，中间那些结点一个都没还。
- 顺序是**后构造的先析构**。`main` 里先定义 `list`、后定义 `empty`，离开函数时 `empty` 先析构，`list` 后析构。C 版这个顺序是你自己写的，本次是编译器安排的。

`clear()` 是本课唯一一处"两个函数互相咬着"的地方：`clear()` 靠 `_size > 0` 决定什么时候停，而 `_size--` 写在 `remove()` 里。**如果 `remove()` 忘了 `_size--`，程序不会报错，会在析构函数里死循环**——这不是编译错误，是逻辑错误，写代码时格外注意。

本次不要求：迭代器、拷贝构造、`operator=`、排序、去重、有序插入、模板特化、可变参数模板、`new[]/delete[]`、`.cpp` 与头文件分离。模板用一个 `template <typename T>` 就够。

---

## 三、三个文件：必须使用的框架与要补的代码

下面三个文件就是本次的交付物。**所有 `TODO` 换成正确实现即可。** 框架能直接编译通过（会有几条 `unused parameter` 警告，那是因为占位函数没用到参数，不用管）；但在没补完之前，程序跑起来会崩、会卡或者输出错乱，这都属于正常现象，不要据此怀疑框架本身有问题。不得修改结点与类的定义、数据成员、函数名、参数、返回类型和参数/函数末尾的 `const`，不得增加或删除数据成员，不得修改教师提供的函数（已注明）。

### 3.1 `listNode.h`

```cpp
#ifndef LISTNODE_H
#define LISTNODE_H

/* ==========================================================================
 * 邓俊辉《数据结构（C++ 语言版）》第三章：列表结点 ListNode
 *
 * 本文件与 list.h 合起来，就是课本里链表的那一对文件。
 * Lab6 的 C 版把 Node 和 LinkedList 写在一个 .c 里；课本把它们拆成两个 .h。
 * ========================================================================== */

template <typename T>
struct ListNode {
    T data;                 /* 数据域：Lab6 的 int data 换成了名字 T */

    ListNode<T>* pred;      /* 前驱指针：Lab6 的 C 版没有这一项 */
    ListNode<T>* succ;      /* 后继指针：Lab6 里叫 next */

    /* 教师提供：默认构造函数，两个指针都置空。头哨兵、尾哨兵用它建立 */
    ListNode()
        : data(), pred(nullptr), succ(nullptr) {}

    /* 教师提供：带数据的构造函数，一次把数据、前驱、后继都设定好 */
    ListNode(const T& e,
             ListNode<T>* p = nullptr,
             ListNode<T>* s = nullptr)
        : data(e), pred(p), succ(s) {}

    ListNode<T>* insertAsPred(const T& e);   /* 练习 1：插到自己前面 */
    ListNode<T>* insertAsSucc(const T& e);   /* 练习 2：插到自己后面 */
};

/* 练习 1：在自己前面插入 e，返回新结点 x。this 就是"自己"，pred 是当前的前驱。
 * 用局部变量 x 接住 new 出来的结点（构造函数里把 pred、succ 一次设好：pred 是 pred，succ 是 this）；
 * 再把"老前驱的后继"和"this 的前驱"改成 x。三句各占一行，顺序要自己想清楚。 */
template <typename T>
ListNode<T>* ListNode<T>::insertAsPred(const T& e) {
    return nullptr;   /* TODO */
}

/* 练习 2：在自己后面插入 e，返回新结点 x。与练习 1 左右对称，
 * 把 pred 换成 succ、this 换成 succ 的关系想一遍即可。 */
template <typename T>
ListNode<T>* ListNode<T>::insertAsSucc(const T& e) {
    return nullptr;   /* TODO */
}

#endif
```

### 3.2 `list.h`

```cpp
#ifndef LIST_H
#define LIST_H

/* ==========================================================================
 * 邓俊辉《数据结构（C++ 语言版）》第三章：列表 List
 *
 * 与 Lab6 的 C 版相比，结构性差别有五处：
 *   1. 单链表改成双向链表：结点多了 pred，删除不再需要"找前驱"；
 *   2. 一个头哨兵改成两个哨兵 header / trailer：
 *      空表是 header <--> trailer，任何有效结点都夹在两个哨兵之间；
 *   3. 自由函数 + 结构体指针参数  改成  类 + 成员函数（this 就是 C 版的 list）；
 *   4. 把 int 换成模板参数 T：同一个类支持 List<int>、List<double>；
 *   5. 拆成两个头文件：ListNode 与 List 各自独立。
 *
 * 空表的样子（也是本类的初始状态）：
 *
 *      header  <-->  trailer
 *
 * 装入 18、-1 之后：
 *
 *      header  <-->  [18]  <-->  [-1]  <-->  trailer
 * ========================================================================== */

#include <iostream>
#include "listNode.h"

typedef int Rank;               /* 秩：与课本一致，一个整数 */

template <typename T>
class List {
private:
    int _size;                  /* 有效元素个数，不含两个哨兵 */
    ListNode<T>* header;        /* 头哨兵：C 版的 head */
    ListNode<T>* trailer;       /* 尾哨兵：C 版没有 */

protected:
    void init();                /* 教师提供：建立两个哨兵，连成空表。对应 Lab6 的 listInit */
    int clear();                /* 练习 3：删除所有有效结点，保留两个哨兵，返回删掉的个数 */

public:
    List() { init(); }          /* 教师提供：C 版的 listInit(&list) */
    ~List();                    /* 教师提供：C 版的 listDestroy(&list)，末尾的输出原样保留 */

    /* 只读接口，全部由教师提供 */
    Rank size() const { return _size; }
    bool empty() const { return _size == 0; }
    ListNode<T>* first() const { return header->succ; }
    ListNode<T>* last() const { return trailer->pred; }
    bool valid(ListNode<T>* p) const {
        return p != nullptr && p != header && p != trailer;
    }

    /* 插入：课本的名字。四个入口，最后都汇到同一个地方 */
    ListNode<T>* insertAsFirst(const T& e);   /* 教师提供：作为首元素插入 */
    ListNode<T>* insertAsLast(const T& e);    /* 教师提供：作为末元素插入 */
    ListNode<T>* insertA(ListNode<T>* p, const T& e);   /* 练习 4：作为 p 的后继插入 */
    ListNode<T>* insertB(ListNode<T>* p, const T& e);   /* 练习 4：作为 p 的前驱插入 */

    /* 删除 */
    T remove(ListNode<T>* p);   /* 练习 5：删除结点 p，返回它保存的数据 */

    /* 查找与秩 */
    ListNode<T>* find(const T& e) const;        /* 练习 6：按值查找，返回位置（不是秩） */
    ListNode<T>* operator[](Rank r) const;      /* 练习 7：按秩定位，课本写法 L[r] */
    Rank rankOf(ListNode<T>* p) const;          /* 教师提供：位置折算成秩，查不到给 -1 */

    /* 下面三个课本没有，是为了让 Lab6 的 C 版能一行一行对上才补的 */
    bool insert(Rank r, const T& e);            /* 练习 8：对应 listInsert */
    bool remove(Rank r, T& e);                  /* 练习 8：对应 listRemove */
    bool get(Rank r, T& e) const;               /* 练习 8：对应 listGet */

    void print() const;                         /* 教师提供：对应 listPrint */
    void printReverse() const;                  /* 练习 9：反向遍历，只有双向链表做得到 */
};

/* --------------------------------------------------------------------------
 * 保护接口
 * -------------------------------------------------------------------------- */

/* 教师提供：建立空表。申请两个哨兵，互相指向，形成「哨兵夹住一段空区间」的初始状态。
 * 注意 header->pred 与 trailer->succ 由构造函数置为 nullptr，课本把它们留给越界位置使用。
 * 这是全篇第一次出现「template 那一行 + List<T>::」这种写法，看清楚它长什么样。 */
template <typename T>
void List<T>::init() {
    header = new ListNode<T>();
    trailer = new ListNode<T>();
    header->succ = trailer;     /* 头哨兵的后继是尾哨兵 */
    trailer->pred = header;     /* 尾哨兵的前驱是头哨兵 */
    _size = 0;
}

/* 练习 3：删除全部有效结点，保留两个哨兵。
 * 反复删除"第一个有效结点"，删到 _size 为 0 为止；返回删掉之前有多少个。
 * 两个哨兵不在这里释放，交给析构函数。记住它和练习 5 是互相咬着的两个函数。 */
template <typename T>
int List<T>::clear() {
    return 0;   /* TODO */
}

/* --------------------------------------------------------------------------
 * 构造与析构
 * -------------------------------------------------------------------------- */

/* 教师提供：析构函数。先清掉全部有效结点，再释放两个哨兵。
 * 如果只写 delete header; delete trailer; 中间那些结点就全部泄漏了。 */
template <typename T>
List<T>::~List() {
    int n = clear();
    delete header;
    delete trailer;
    std::cout << "(destructor: cleared " << n << " data nodes, freed 2 sentinels)\n";
}

/* --------------------------------------------------------------------------
 * 插入：四个入口，一条通路
 * -------------------------------------------------------------------------- */

/* 教师提供：作为首元素插入，插到「当前首结点」之前。
 * 空表时 first() 就是 trailer，所以空表插入也自动成立，不需要 if。 */
template <typename T>
ListNode<T>* List<T>::insertAsFirst(const T& e) {
    return insertB(first(), e);
}

/* 教师提供：作为末元素插入，插到尾哨兵之前。
 * 这正是尾哨兵存在的理由——表尾和表中间不再有区别。 */
template <typename T>
ListNode<T>* List<T>::insertAsLast(const T& e) {
    return insertB(trailer, e);
}

/* 练习 4a：作为 p 的后继插入。_size 由 List 维护，ListNode 只顾接链 */
template <typename T>
ListNode<T>* List<T>::insertA(ListNode<T>* p, const T& e) {
    return nullptr;   /* TODO */
}

/* 练习 4b：作为 p 的前驱插入。注意 p 允许是 trailer，也允许是首结点 */
template <typename T>
ListNode<T>* List<T>::insertB(ListNode<T>* p, const T& e) {
    return nullptr;   /* TODO */
}

/* --------------------------------------------------------------------------
 * 删除
 * -------------------------------------------------------------------------- */

/* 练习 5：删除结点 p，返回它保存的数据。p 一定是有效结点（不是两个哨兵）。
 * 先取出 data（马上要失去这个结点了），再断链，最后单独一行 delete p; 并让 _size 减一。 */
template <typename T>
T List<T>::remove(ListNode<T>* p) {
    return T();   /* TODO */
}

/* --------------------------------------------------------------------------
 * 查找与按秩访问
 * -------------------------------------------------------------------------- */

/* 练习 6：按值查找，返回结点的"位置"（ListNode<T>*），不是秩。
 * 课本从最后一个有效结点出发，顺着 pred 往前走，遇到头哨兵就说明没找到。
 * 同一元素出现多次时，它命中"最后一个"，与 C 版 listFind 的方向相反。 */
template <typename T>
ListNode<T>* List<T>::find(const T& e) const {
    return nullptr;   /* TODO */
}

/* 练习 7：按秩定位，返回秩为 r 的结点。
 * 从首结点一步一步走 r 步；r 越界时返回 nullptr。
 * 链表算不出地址，这就是"按秩访问 O(n)"的全部来源。 */
template <typename T>
ListNode<T>* List<T>::operator[](Rank r) const {
    return nullptr;   /* TODO */
}

/* 教师提供：位置折算成秩。查不到（含 nullptr）给 -1 */
template <typename T>
Rank List<T>::rankOf(ListNode<T>* p) const {
    if (!valid(p)) {
        return -1;
    }
    Rank r = 0;
    for (ListNode<T>* q = first(); q != p; q = q->succ) {
        r++;
    }
    return r;
}

/* --------------------------------------------------------------------------
 * 为与 Lab6 的 C 版逐条对照而补的三个接口
 * -------------------------------------------------------------------------- */

/* 练习 8a：对应 C 版 listInsert，在秩 r 处插入，合法范围 0 <= r <= size。
 * r == _size 表示插到末尾，此时目标位置就是尾哨兵 trailer，不是 (*this)[r]。 */
template <typename T>
bool List<T>::insert(Rank r, const T& e) {
    return false;   /* TODO */
}

/* 练习 8b：对应 C 版 listRemove，删除秩 r 处的结点，数据经引用参数 e 带回。
 * 越界时返回 false 且不修改 e。 */
template <typename T>
bool List<T>::remove(Rank r, T& e) {
    return false;   /* TODO */
}

/* 练习 8c：对应 C 版 listGet，按秩读取，失败时不修改 e */
template <typename T>
bool List<T>::get(Rank r, T& e) const {
    return false;   /* TODO */
}

/* --------------------------------------------------------------------------
 * 输出
 * -------------------------------------------------------------------------- */

/* 教师提供：遍历链表的标准写法，从首结点出发，遇到尾哨兵就停 */
template <typename T>
void List<T>::print() const {
    std::cout << "[size = " << _size << "] ";
    for (ListNode<T>* p = header->succ; p != trailer; p = p->succ) {
        std::cout << p->data << ' ';
    }
    std::cout << '\n';
}

/* 练习 9：反向遍历。把 print() 的方向掉过来：从 trailer->pred 出发，
 * 顺着 pred 走，遇到 header 停。前缀 "reverse: " 与末尾换行已经写好，你只补中间那两句。 */
template <typename T>
void List<T>::printReverse() const {
    std::cout << "[size = " << _size << "] reverse: ";
    /* TODO */
    std::cout << '\n';
}

#endif
```

### 3.3 `main.cpp`

把下面代码**原样**放在同一目录里（这就是本次唯一的程序入口，不要改）。测试顺序与 Lab6 的 `linkedlist.c` 一致，方便逐行对照；末尾多出几段，是单链表做不到的事。

```cpp
/* ==========================================================================
 * Lab7 测试程序：课本版 List<T>（listNode.h + list.h）的统一测试
 *
 * 编译运行：clang++ -std=c++17 -Wall -Wextra main.cpp -o lab7_list && ./lab7_list
 * ========================================================================== */

#include <iostream>
#include "list.h"

/* 第一部分：List<int>，测试顺序与 Lab6 的 linkedlist.c 一致 */
static void testIntList() {
    List<int> list;

    std::cout << "after init: ";
    list.print();
    std::cout << "empty = " << list.empty() << ", size = " << list.size() << "\n\n";

    /* 1. 依次在表尾插入五个样例元素。C 版：listPushBack(&list, samples[i]) */
    int samples[] = {18, -1, 42, 18, 65};
    for (int i = 0; i < 5; i++) {
        list.insertAsLast(samples[i]);
    }
    std::cout << "after insertAsLast 18, -1, 42, 18, 65: ";
    list.print();
    std::cout << '\n';

    /* 2. 按秩插入。C 版：listInsert(&list, 2, 25) */
    bool ok = list.insert(2, 25);
    std::cout << "insert(2, 25): ok = " << ok << ", ";
    list.print();

    /* 3. 按秩删除，被删的值由引用参数带回。C 版：listRemove(&list, 1, &removed) */
    int removed = 777;
    ok = list.remove(1, removed);
    std::cout << "remove(1): ok = " << ok << ", removed = " << removed << ", ";
    list.print();

    /* 4. 在表头插入。C 版：listPushFront(&list, 7) */
    list.insertAsFirst(7);
    std::cout << "insertAsFirst(7): ";
    list.print();
    std::cout << '\n';

    /* 5. 按秩读取：合法的秩与越界的秩。C 版：listGet(&list, 0, &value) */
    int value = 777;
    ok = list.get(0, value);
    std::cout << "get(0): ok = " << ok << ", value = " << value << '\n';

    value = 777;
    ok = list.get(6, value);            /* 此刻 size = 6，秩 6 已越界 */
    std::cout << "get(6): ok = " << ok << ", value = " << value << "\n\n";

    /* 6. 按值查找：课本返回的是"位置"，用 rankOf 折算成秩 */
    std::cout << "rankOf(find(42)) = " << list.rankOf(list.find(42)) << '\n';
    std::cout << "rankOf(find(18)) = " << list.rankOf(list.find(18)) << '\n';
    std::cout << "rankOf(find(99)) = " << list.rankOf(list.find(99)) << "\n\n";

    /* 7. 拿着位置直接插到前后，不用先算秩：单链表做不到这件事 */
    ListNode<int>* p = list.first();
    list.insertB(p, 2000);
    list.insertA(p, 1000);
    std::cout << "insertB(first(), 2000) and insertA(first(), 1000): ";
    list.print();
    std::cout << '\n';

    /* 8. 课本的按秩访问写法，以及双向链表才有的反向遍历 */
    std::cout << "list[2]->data = " << list[2]->data << '\n';
    list.printReverse();
    std::cout << '\n';

    /* 9. 非法位置：负秩、超过 size 的秩、删除空表 */
    ok = list.insert(-1, 100);
    std::cout << "insert(-1, 100): ok = " << ok << '\n';
    ok = list.insert(100, 100);
    std::cout << "insert(100, 100): ok = " << ok << '\n';

    List<int> empty;
    ok = empty.remove(0, removed);
    std::cout << "empty remove(0): ok = " << ok << "\n\n";

    /* 10. 从表头删掉三个，观察 size 回落；剩下的交给析构函数 */
    std::cout << "remove one by one: ";
    for (int i = 0; i < 3; i++) {
        list.remove(0, removed);
        std::cout << removed << "(size = " << list.size() << ") ";
    }
    std::cout << "\nbefore leaving testIntList: ";
    list.print();
    std::cout << "no listDestroy written by hand: both objects clean themselves up ->\n";
}

/* 第二部分：List<double>，同一个类换一种数据类型 */
static void testDoubleList() {
    List<double> nums;

    double values[] = {1.5, 2.25, -3.75};
    for (int i = 0; i < 3; i++) {
        nums.insertAsLast(values[i]);
    }
    std::cout << "List<double>: ";
    nums.print();

    nums.insert(1, 0.5);
    std::cout << "after insert(1, 0.5): ";
    nums.print();

    double firstValue = 0.0;
    nums.get(0, firstValue);
    std::cout << "get(0) = " << firstValue << ", size = " << nums.size() << '\n';
}

int main() {
    std::cout << "sizeof(ListNode<int>) = " << sizeof(ListNode<int>) << '\n';
    std::cout << "sizeof(List<int>) = " << sizeof(List<int>) << "\n\n";

    testIntList();
    std::cout << '\n';
    testDoubleList();
    return 0;
}
```

### 3.4 `CMakeLists.txt`

按第一节给出的那份写，与 `main.cpp` 同目录。

### 3.5 实现要求

1. `listNode.h` 里不得包含任何头文件（它不需要输出）；`list.h` 只包含 `<iostream>` 与 `"listNode.h"`；`main.cpp` 只包含 `<iostream>` 与 `"list.h"`。
2. 两个头文件都必须有 `#ifndef/#define/#endif` 卫士，宏名与文件名对应。两个头文件里都**不得**出现 `using namespace std;`，输出一律写 `std::cout`。
3. 不得修改教师提供的函数：两个构造函数、`init()`、`List()`、`~List()`、`size()`、`empty()`、`first()`、`last()`、`valid()`、`insertAsFirst()`、`insertAsLast()`、`rankOf()`、`print()`，以及 `~List()` 末尾那行输出（它是验证手段，原样保留，不得在其他函数里增加调试输出）。
4. `insertA()`、`insertB()` 必须复用 `p->insertAsSucc(e)`、`p->insertAsPred(e)`，各自先让 `_size` 加一，不得另写一套接链代码。
5. `remove(ListNode<T>* p)` 必须先取出 `data`，再写两句断链，最后**单独一行** `delete p;`，然后 `_size--`。两句断链的顺序不限，但必须在 `delete` 之前。
6. `insert()`、`remove(r, e)`、`get(r, e)` 三个按秩接口必须复用已有的接口，不得另写遍历或接链代码：`insert()` 用 `insertB()`；`remove(r, e)` 用 `(r)` 取位置再交给 `remove(p)`；`get(r, e)` 用 `operator[]()`。三个接口失败时都不得修改 `e`。
7. `printReverse()` 的停止条件必须是 `p != header`，与 `print()` 的 `p != trailer` 对称；不得用 `_size` 或计数控制。
8. `clear()` 与 `remove(p)` 互相咬着：`clear()` 靠 `_size > 0` 决定何时停，`_size--` 在 `remove()` 里。两个都要写对，否则析构时会死循环。
9. 只能用 `new` / `delete`，不得出现 `malloc` / `free`；不得使用数组、`std::list`、`std::vector`、`std::deque` 或全局变量；不得把 `private` 里的数据成员改成 `public`，或在类外直接访问 `header`、`trailer`、`_size`。
10. `main.cpp` 必须与 3.3 给出的一致，不得为了调试改写、增删输出。

### 3.6 期望输出与核对重点

```text
sizeof(ListNode<int>) = 24
sizeof(List<int>) = 24

after init: [size = 0] 
empty = 1, size = 0

after insertAsLast 18, -1, 42, 18, 65: [size = 5] 18 -1 42 18 65 

insert(2, 25): ok = 1, [size = 6] 18 -1 25 42 18 65 
remove(1): ok = 1, removed = -1, [size = 5] 18 25 42 18 65 
insertAsFirst(7): [size = 6] 7 18 25 42 18 65 

get(0): ok = 1, value = 7
get(6): ok = 0, value = 777

rankOf(find(42)) = 3
rankOf(find(18)) = 4
rankOf(find(99)) = -1

insertB(first(), 2000) and insertA(first(), 1000): [size = 8] 2000 7 1000 18 25 42 18 65 

list[2]->data = 1000
[size = 8] reverse: 65 18 42 25 18 1000 7 2000 

insert(-1, 100): ok = 0
insert(100, 100): ok = 0
empty remove(0): ok = 0

remove one by one: 2000(size = 7) 7(size = 6) 1000(size = 5) 
before leaving testIntList: [size = 5] 18 25 42 18 65 
no listDestroy written by hand: both objects clean themselves up ->
(destructor: cleared 0 data nodes, freed 2 sentinels)
(destructor: cleared 5 data nodes, freed 2 sentinels)

List<double>: [size = 3] 1.5 2.25 -3.75 
after insert(1, 0.5): [size = 4] 1.5 0.5 2.25 -3.75 
get(0) = 1.5, size = 4
(destructor: cleared 4 data nodes, freed 2 sentinels)
```

把程序跑通、输出与上面完全一致之后再往下做题。核对重点：

- 把中间那段数据行与 Lab6 的 3.4 并排看：`insert(2, 25)`、`remove(1)` 之后的表完全一样，只是函数名去掉了 `list` 前缀。**同一个数据结构，两种语言、两种存储，算法没有变。**
- `get(6)` 失败后 `value` 仍然是 `777`，说明失败时不修改输出参数。
- `rankOf(find(18)) = 4`，而 Lab6 的 `listFind(18) = 1`。这不是谁的 bug：课本的 `find` 从**末结点**顺着 `pred` 往前走，先命中的是最后一个 `18`；C 版的 `listFind` 从表头顺着 `next` 走，先命中的是第一个。两版方向相反，是课本故意这么写的，不要为了"让输出和 Lab6 一样"去改它。
- 反向遍历是 `65 18 42 25 18 1000 7 2000`。如果这一行是空的、少几个数或者程序在这里崩掉，检查 `insertAsPred`、`insertAsSucc` 有没有把 `pred` 那一侧接上：只接 `succ` 不接 `pred`，正向遍历也会跟着错乱，但反向是最先暴露的地方。
- 最后两行是析构函数自动打印的。`empty` 先析构（`cleared 0`），`list` 后析构（`cleared 5`，就是第 10 步剩下那五个）。**后构造的先析构。** 如果第二个数不是 5，说明 `remove(p)` 少删了或在多删。
- 最后一行 `(destructor: cleared 4 data nodes, freed 2 sentinels)` 来自 `List<double>`。同一个类，换一种类型，源码一行都没改——这是模板换来的。
- **如果程序卡住不返回**，多半是 `clear()` 与 `remove(p)` 没配合好：`clear()` 的 `while (_size > 0)` 等的是 `_size--`，而 `_size--` 写在 `remove(p)` 里。这不是编译错误，是逻辑错误。

---

## 四、选择题与填空

本部分除填空外均为单项选择题。将所选选项前的 `[ ]` 改为 `[x]`，每题只能勾选一项，不要删除其他选项。

### 4.1 双向链表的结点与两个哨兵

`sizeof(ListNode<int>)` 是 24，而 Lab6 的 `sizeof(Node)` 是 16。多出来的东西和两个哨兵带来的变化，正确的是：

- [x] A. 多出来的是 `pred`；`succ` 就是 Lab6 的 `next` 改了名字，两个哨兵让表头、表尾的处理完全对称，`insertAsLast` 变成 `O(1)`
- [ ] B. 多出来的是两个哨兵指针，各占 4 字节
- [ ] C. `int data` 从 4 字节变成了 12 字节
- [ ] D. `pred`、`succ` 各占 4 字节，构造函数再占 8 字节

### 4.2 接链三句的顺序

`insertAsPred` 的三句是：先 `ListNode<T>* x = new ListNode<T>(e, pred, this);`，再 `pred->succ = x;`，最后 `pred = x;`。如果把后两句颠倒（先写 `pred = x;`）重新编译运行，结果是：

- [ ] A. 编译错误
- [ ] B. 两句互不影响，结果一样
- [x] C. 第二句先跑，`pred` 已经变成 `x`，第一句等于执行 `x->succ = x`，新结点自己指向自己；输出打到 `[size = 5]` 就停住，紧接着退出码 139
- [ ] D. 新结点被插到了表头

### 4.3 删除结点为什么不用找前驱

本次 `remove(p)` 不需要像 Lab6 那样先走一遍 `prevOf` 找前驱，而且它的两句断链可以颠倒。原因是：

- [x] A. 因为 `pred` 直接存着前驱；两句断链都只读 `p->pred`、`p->succ`，谁都不去改它们，所以能颠倒。而接链的第二句恰好改掉第一句要读的 `pred`，所以不能颠倒——判据是同一条：后一句会不会改掉前一句要读的成员
- [ ] B. 因为断链改的是指针，接链改的是数据
- [ ] C. 因为 `delete` 只能写在最后一句
- [ ] D. 其实两处都不能颠倒，只是断链写反了不容易被发现

### 4.4 `template` 那一行的作用范围

`list.h` 里每个类外定义的成员函数前面都有一行 `template <typename T>`，一共 16 行。原因是：

- [ ] A. 这是语法装饰，写不写都能编过
- [x] B. 这一行只管紧跟它的那一个定义，下一个定义要重新写；类外定义必须写成"`template <typename T>` 一行 + 名字里的 `List<T>::`"，少一样都编不过
- [ ] C. 写一次就够，重复写只是为了排版
- [ ] D. 因为文件里有两个模板，所以每段代码都要写一行

### 4.5 模板的定义为什么必须写在头文件里

如果把 `List<T>::print()` 的定义从 `list.h` 搬到一个 `list.cpp` 里，只留声明在头文件：

- [ ] A. 编译就报错，因为 `.cpp` 里看不到模板参数
- [x] B. 每个文件单独编译都能通过，最后链接时报 `Undefined symbols`，因为编译器看不见完整的模板定义，就不会为 `List<int>` 生成 `print()` 的代码
- [ ] C. 一切正常，和普通类完全一样
- [ ] D. 可以编译运行，只在调用 `print()` 时抛异常

### 4.6 `private` / `protected` 的作用

`_size`、`header`、`trailer` 放在 `private:`，`init()`、`clear()` 放在 `protected:`。这样安排的效果是：

- [ ] A. 只是注释里的约定，编译器仍然不管
- [x] B. 编译器真的会拦：类外面的代码写 `list.header` 或 `list._size` 直接编译不过（C 版只能靠注释提醒）；而 `protected` 允许自己以及将来继承 `List` 的派生类调用，用 `List` 的人不需要也不该直接调
- [ ] C. `protected` 下的函数执行更快
- [ ] D. 对象占的内存会变少

### 4.7 填空：C 版与课本版逐条对照

把右列补全，至少填写四行：

| Lab6 的 C 版 | 本次的课本版                             |
| :--- |:-----------------------------------------|
| `LinkedList list; listInit(&list);` | `List<int>list;`                         |
| `listPushBack(&list, v)` | `list.insertAsLast(v)`                   |
| `listInsert(&list, 2, 25)` | `list.insert(2,25)`                      |
| `listRemove(&list, 1, &removed)` | `list.remove(1,removed)`                 |
| `listFind(&list, 18)`（返回秩） | `list.rankOf(list.find(18)`              |
| `main` 结尾的 `listDestroy(&list);` | 不需要手动写，离开作用域自动调用析构函数 |

### 4.8 填空：三句话解释

用三到五句话回答：为什么模板的成员函数定义必须写在头文件里，而不能像普通函数那样"声明放 `.h`、定义放 `.cpp`"？把代码拆成 `listNode.h` 与 `list.h` 两个文件，又解决了什么？

> 在此填写你的回答（不少于 60 字）。
原因：模板属于编译期实例化，编译器必须看到完整的函数定义，才能根据 List<int> 这类类型生成对应代码。如果模板只在头文件写声明、把实现放在 .cpp ，编译时无法完成实例化，链接会出现未定义符号报错。拆分目的：拆分为 listNode.h 和 list.h 两个文件，将结点结构与链表类分离，实现代码解耦，逻辑层次更加清晰
---

## 五、提交要求

将本文件复制到自己的 `学号姓名/Lab7/Lab7.md`，填写选择题答案和填空。最终只提交以下 **5 个文件**：

```text
学号姓名/
└── Lab7/
    ├── CMakeLists.txt
    ├── listNode.h
    ├── list.h
    ├── main.cpp
    └── Lab7.md
```

特别注意：

- `CMakeLists.txt` 必须有且只有一个 `add_executable`，目标 `lab7_list` 对应 `main.cpp`；三个源文件与它放在同一目录、同级引用。
- 三个代码文件必须能一起编译运行，跑出的输出与 3.6 完全一致：`listNode.h`、`list.h`、`main.cpp` 缺一不可，不能只提交增量片段，也不能把三个文件的内容合并成一个 `.cpp`。
- `Lab7.md` 必须保留题目结构，填写选择题答案和两个填空。
- 每道单项选择题必须且只能将一个 `[ ]` 改为 `[x]`。
- 填写内容不能留空：4.7 至少填写四行；4.8 填写解释，不少于 60 字。
- 程序和书面题的完成度检查不代替正确性验证。
- 不要提交整个 CLion 项目、`cmake-build-*`、`.idea/`、可执行文件或其他编译产物。
- `Lab7`、`Lab7.md`、`CMakeLists.txt`、`listNode.h`、`list.h`、`main.cpp` 文件名的大小写必须完全一致。
- PR 标题必须严格使用 `[学号姓名]Lab7作业提交`，右方括号后不能有空格。
- 一个 PR 只能包含本次 Lab7 的文件，不得修改 Lab3、Lab4、Lab5、Lab6、其他同学的文件、`homework/`、README 或仓库配置。

---

## 六、截止时间

**2026 年 10 月 1 日 24:00（即 2026 年 10 月 2 日 00:00，北京时间）**

以 GitHub 记录的最后一次向 PR 推送代码的时间为准。不晚于上述时刻创建 PR 并完成最后一次推送不算超时；超过该时刻新建 PR，或向已有 PR 推送任何修改，均算作超时。审核未通过的同学请在截止前完成修改。
