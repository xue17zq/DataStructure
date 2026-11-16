# Lab6：用 C 实现带头哨兵的单链表与循环单链表

> **作业目标**：把课堂上从四个具名结点一路走到的"带头哨兵的 C 链表"补完整：结点从 `malloc` 来，`head` 与 `size` 打包进一个 `LinkedList` 结构体，按秩完成输出、顺序追加、中间插入、删除、按秩读取和按值查找；在同一套结点上做出**循环单链表**，用它解约瑟夫问题；用 CLion 断点、单步和 Memory View 亲眼看到"插入只改指针、删除先断链后释放"。
>
> 本说明给出了新增知识、两个程序的框架、统一测试和期望输出。两个程序各自是一个单独的 `.c` 文件，全部用 C 语言完成。课堂上已经写过的 `newNode`、`insertAfter`、`unlinkAfter` 以及建立、销毁函数由教师提供，学生补的是按秩操作和与"圈"有关的函数。本次不涉及 C++，不涉及 `class`、`new/delete`、构造函数和析构函数，这些放在 Lab7。
>
> **检查说明**：程序和书面题沿用前几次作业的完成度检查口径，内容正确性将在期末统一检测。本次要交三张截图，图片审核会检查是否展示真实的 CLion 调试界面，以及指定时刻的变量、地址和内存数据是否对应。空文件、未作答模板、占位程序和缺失截图不属于完成；自动审核规则以课程仓库发布的 Lab6 配置为准。

---

## 一、配置 CMakeLists.txt

保留 Lab5，在新的 `Lab6` 目录中完成本次作业。两个程序各有一个 `main`，所以要写两个目标，在 CLion 右上角切换运行。

```cmake
cmake_minimum_required(VERSION 3.20)
project(Lab6 C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(lab6_list linkedlist.c)
add_executable(lab6_circ circlist.c)
```

要求：

- 有且只有两个 `add_executable`，目标名与源文件名与上面完全一致。
- `project(...)` 与前几次作业一致，同时声明 `C` 和 `CXX`。本次两个源文件都是 `.c`，按 C11 编译。
- 保存后重新加载 CMake 项目。两个目标互相独立，一个没写完不影响另一个编译。
- 本次仍然需要提交 `CMakeLists.txt`。

---

## 二、完成本次作业所需的新知识

### 2.1 结点：`next` 里存的是地址

```c
typedef struct Node {
    int data;           /* 数据域：保存一个整数 */
    struct Node* next;  /* 指针域：保存下一个结点的地址，NULL 表示没有后继 */
} Node;
```

- 结构体还在定义之中，里面就出现了指向自己的指针 `struct Node* next`。这是允许的，因为指针长度固定；写成 `struct Node next` 会编译错误。
- `p->data` 等价于 `(*p).data`：`p` 是指向结点的指针用 `->`，结点变量本身用 `.`。
- `sizeof(Node)` 是 16 而不是 12：`int` 占 4 字节，指针占 8 字节，编译器为对齐补了 4 字节。3.5 的 Memory View 会亲眼看到这 16 个字节。

### 2.2 结点从 `malloc` 来，用完必须 `free`

```c
static Node* newNode(int value) {
    Node* p = (Node*)malloc(sizeof(Node));
    if (p == NULL) {
        printf("malloc failed, exit\n");
        exit(1);
    }
    p->data = value;
    p->next = NULL;
    return p;
}
```

- 栈上的结点活不过 `return`，所以插入函数里的新结点只能从堆上申请，寿命由 `free` 决定，不由作用域决定。
- 每摘下一个结点都要问一句：它当初是怎么来的，该不该 `free`。本次所有结点都是 `malloc` 来的，摘下之后必须 `free`，否则就是内存泄漏。
- `free(p)` 之后不能再读 `p->data` 或 `p->next`。释放整条链时要**先记住下一个，再 `free` 当前这个**。

### 2.3 头哨兵与 `LinkedList` 结构体

```c
typedef struct {
    Node* head;         /* 头哨兵，data 不使用 */
    int size;           /* 有效结点个数，不含哨兵 */
} LinkedList;
```

- **头哨兵**是一个不存有效数据、永远占着表头位置的结点。有了它，"第一个有效结点的前驱"总是存在，表头插入和删除就不需要单独处理。有效数据从 `head->next` 开始，遍历时不要从 `head` 开始，否则会多打一个 0。
- 课堂上的 `head` 是 `main` 里的局部变量。本次把它和 `size` 打包进 `LinkedList`，所有函数通过 `LinkedList*` 参数访问它们，这和 Lab3 把 `data` 与 `size` 打包进 `SeqList` 是同一件事。
- `size` 必须单独记录：链表不像向量那样能用"首地址加秩"算出有效范围。`listInsert` 成功后 `size` 加一，`listRemove` 成功后减一，忘了任何一处 `listPrint` 里的计数就对不上。
- `LinkedList list;` 只是一个局部变量，里面还没有任何结点，必须先 `listInit(&list)`。用完必须 `listDestroy(&list)`，C 语言没有析构函数，释放要自己写在 `main` 里。

### 2.4 插入两句与删除一句

```c
/* 在 prev 后面插入 p：先让新结点接住后面，再让前面接住新结点。顺序不能反 */
p->next = prev->next;
prev->next = p;

/* 摘掉 prev 后面的结点：只改前驱的一根指针 */
prev->next = prev->next->next;
```

插入和删除各自只改指针，没有一个 `data` 被移动过。写反插入两句会让新结点指向自己，后半条链再也走不到。删除之后必须 `free` 被摘下的结点；`free` 之前必须先断链，否则链上会留下一个指向已释放内存的指针。

### 2.5 按秩操作：从前驱出发

链表没有下标，秩为 `rank` 的结点只能从哨兵出发走 `rank` 步才能到。本次两个程序里都有一个私有的定位函数 `prevOf`，学生不用写，但要看懂：

| 调用 | 返回什么 | `rank` 的范围 |
| :--- | :--- | :--- |
| `prevOf(list, rank)` | 秩为 `rank` 的结点的**前驱**；`rank == 0` 时是哨兵 | `0 <= rank <= size` |

单链表要拿前驱，是因为插入和删除改的都是前驱的 `next`。`prevOf(list, size)` 返回最后一个有效结点，正好是追加时需要的前驱。

按秩操作的合法范围与 Lab4 的向量完全一致：`listInsert` 允许 `rank == size`（追加到末尾），`listRemove` 和 `listGet` 不允许。

### 2.6 循环单链表：最后一个结点指回哨兵

```text
单链表：  head → [1] → [2] → [3] → NULL
循环链表：head → [1] → [2] → [3] ┐
          ↑____________________┘
```

只改一个约定：最后一个结点的 `next` 不再是 `NULL`，而是指回 `head`。由此带来三处变化，写循环链表时每一处都要过一遍脑子：

| 单链表 | 循环链表 |
| :--- | :--- |
| 空表：`head->next == NULL` | 空表：`head->next == head`，哨兵指向自己 |
| 遍历停止条件：`p != NULL` | 遍历停止条件：`p != head`。写成 `p != NULL` 会**死循环** |
| 找最后一个结点：`p->next == NULL` | 找最后一个结点：`p->next == head` |

插入两句和删除一句**一字不差**，只是 `unlinkAfter` 判断边界时从"后面是不是 `NULL`"改成"后面是不是 `head`"。好处是从任何一个结点出发都能走遍整条链，这正是约瑟夫问题需要的。**哨兵也在圈上**，绕圈报数时走到哨兵要跳过它。

### 2.7 约瑟夫问题

`n` 个人围成一圈，从第一个人开始报数，报到 `k` 的人出列，从他的下一个人重新从 1 报起，直到所有人出列。用循环链表做：

1. 维护一个指针 `prev`，它始终是"下一个报 1 的结点"的**前驱**，初始为 `head`。
2. 让 `prev` 向前走 `k - 1` 步，走的过程中如果下一个是哨兵就跳过哨兵。
3. 此时 `prev` 后面那个结点就是报到 `k` 的人，用 `unlinkAfter` 摘下、输出、`free`。
4. 摘下之后 `prev->next` 自动变成出列者的下一个人，正好是下一轮报 1 的人；如果它是哨兵，让 `prev = head`。
5. 重复直到 `size == 0`。

手工模拟 `n = 5, k = 3`：`1 2 3 4 5` → 出 3，剩 `1 2 4 5` 从 4 报起 → 出 1，剩 `2 4 5` 从 2 报起 → 出 5，剩 `2 4` 从 2 报起 → 出 2 → 出 4。出列顺序 `3 1 5 2 4`。

为什么维护"前驱"而不是"当前正在报数的人"：出列的动作是摘链，摘链改的是前驱的 `next`。手里握着前驱，摘一个结点只要一句；握着当前结点，单链表上回头找前驱是走不通的。

### 2.8 本次不要求

不要求 C++、类、`new/delete`、构造函数、析构函数、模板、头文件拆分、尾指针、排序或有序插入。C++ 版本的 `IntList` 类和双向链表放在 Lab7。

---

## 三、linkedlist.c：补完课堂上的带头哨兵单链表

### 3.1 必须使用的框架

下面的框架可以直接编译运行（输出全是错的），把每一处 `TODO` 换成正确实现即可。不得修改结点与 `LinkedList` 的定义，不得修改函数的名字、参数、返回类型和参数里的 `const`，不得修改教师提供的函数。

```c
#include <stdio.h>
#include <stdlib.h>

/* 结点：与课堂上写的完全相同，不得修改 */
typedef struct Node {
    int data;           /* 数据域：保存一个整数 */
    struct Node* next;  /* 指针域：保存下一个结点的地址，NULL 表示没有后继 */
} Node;

/* 链表：头哨兵的地址和有效结点个数打包在一起。
 * 与 Lab3 的 SeqList 一样，所有操作都通过 LinkedList* 参数访问它们 */
typedef struct {
    Node* head;         /* 头哨兵，data 不使用 */
    int size;           /* 有效结点个数，不含哨兵 */
} LinkedList;

/* --------------------------------------------------------------------------
 * 教师提供：结点级操作，课堂代码原样搬来，不得修改
 * -------------------------------------------------------------------------- */

/* 申请一个存放 value 的新结点。它的寿命不由作用域决定，而由 free 决定 */
static Node* newNode(int value) {
    Node* p = (Node*)malloc(sizeof(Node));
    if (p == NULL) {
        printf("malloc failed, exit\n");
        exit(1);
    }
    p->data = value;
    p->next = NULL;
    return p;
}

/* 在 prev 后面插入一个存 value 的新结点，返回新结点。课堂上的两句 */
static Node* insertAfter(Node* prev, int value) {
    Node* p = newNode(value);
    p->next = prev->next;
    prev->next = p;
    return p;
}

/* 摘下 prev 后面的结点并返回它，只改指针，不释放 */
static Node* unlinkAfter(Node* prev) {
    Node* p = prev->next;
    if (p != NULL) {
        prev->next = p->next;
    }
    return p;
}

/* 秩为 rank 的结点的前驱；rank == 0 时是哨兵本身。从哨兵出发走 rank 步 */
static Node* prevOf(const LinkedList* list, int rank) {
    Node* p = list->head;
    for (int i = 0; i < rank; i++) {
        p = p->next;
    }
    return p;
}

/* --------------------------------------------------------------------------
 * 教师提供：建立与销毁
 * -------------------------------------------------------------------------- */

/* 建立空表：立哨兵。对应课堂上 main 开头的 head = newNode(0) */
void listInit(LinkedList* list) {
    list->head = newNode(0);
    list->size = 0;
}

/* 释放整条链，哨兵也在链上。末尾的输出是验证手段，原样保留 */
void listDestroy(LinkedList* list) {
    int count = 0;
    Node* p = list->head;
    while (p != NULL) {
        Node* next = p->next;   /* 先记住下一个，再 free 当前这个 */
        free(p);
        p = next;
        count++;
    }
    list->head = NULL;
    list->size = 0;
    printf("(listDestroy: freed %d nodes, including the sentinel)\n", count);
}

int listSize(const LinkedList* list) {
    return list->size;
}

int listEmpty(const LinkedList* list) {
    return list->size == 0;
}

/* --------------------------------------------------------------------------
 * 学生练习
 * -------------------------------------------------------------------------- */

/* 练习 1：输出。从第一个有效结点开始输出，哨兵不输出，每个数后面一个空格，末尾换行 */
void listPrint(const LinkedList* list) {
    printf("[size = %d] ", list->size);
    /* TODO：for (const Node* p = list->head->next; p != NULL; p = p->next) */
    printf("\n");
}

/* 练习 2：在秩 rank 处插入 value，合法范围 0 <= rank <= size
 * 成功返回 1 并让 size 加一；rank 非法返回 0，链表不变 */
int listInsert(LinkedList* list, int rank, int value) {
    return 0;   /* TODO：检查 rank；insertAfter(prevOf(list, rank), value)；size 加一 */
}

/* 练习 3：顺序追加与表头插入，都复用 listInsert，不得另写接链代码 */
int listPushBack(LinkedList* list, int value) {
    return 0;   /* TODO：listInsert(list, list->size, value) */
}

int listPushFront(LinkedList* list, int value) {
    return 0;   /* TODO：listInsert(list, 0, value) */
}

/* 练习 4：删除秩 rank 处的结点，被删的值写入 *value，合法范围 0 <= rank < size
 * 成功返回 1 并让 size 减一；失败返回 0 且不修改 *value
 * 必须用局部变量 p 接住 unlinkAfter 的返回值，并单独一行写 free(p);（3.5 的断点设在这一行） */
int listRemove(LinkedList* list, int rank, int* value) {
    return 0;   /* TODO：检查 rank；Node* p = unlinkAfter(prevOf(list, rank))；取出 data；free(p)；size 减一 */
}

/* 练习 5：按秩读取，合法范围 0 <= rank < size；失败时不修改 *value */
int listGet(const LinkedList* list, int rank, int* value) {
    return 0;   /* TODO */
}

/* 练习 6：按值查找，返回第一次出现的秩，找不到返回 -1。从 head->next 开始数，秩从 0 起 */
int listFind(const LinkedList* list, int value) {
    return -1;  /* TODO */
}
```

### 3.2 实现要求

1. `Node`、`LinkedList` 的定义和四个 `static` 工具函数不得修改；`listInit`、`listDestroy`、`listSize`、`listEmpty` 原样保留。
2. `listInsert()` 必须复用 `insertAfter(prevOf(list, rank), value)`，不得另写一套接链代码；成功后 `size` 加一。
3. `listRemove()` 必须复用 `unlinkAfter(prevOf(list, rank))`，用局部变量 `Node* p` 接住返回值，取出 `data` 之后**单独一行**写 `free(p);`，然后 `size` 减一。3.5 的断点要设在这一行。
4. `listGet()` 与 `listFind()` 只读，参数里的 `const` 不得删除；`listGet()` 失败时不修改 `*value`。`*value` 是 C 语言的指针输出参数，用法与 Lab3 的 `removeAt` 相同，写入前要先判空。
5. `listFind()` 的秩从第一个有效结点算起为 0，哨兵不计入；重复元素返回第一次出现的秩。
6. `listPushBack()`、`listPushFront()` 必须复用 `listInsert`，它们是本次的检查点之一。
7. 只能包含 `<stdio.h>` 和 `<stdlib.h>`；不得使用 C++ 的 `iostream`、`cout`、`bool`、`new`、`delete`，不得使用数组代替链表，不得使用全局变量。
8. `listDestroy` 末尾那行输出是教师提供的验证手段，原样保留，不得在其他函数里增加调试输出。

### 3.3 统一 main 函数

把下面代码**原样**放在所有函数之后。测试顺序与讲义附录一、Lab4 的向量一致：空表、追加五个样例、中间插入、删除并带回值、表头插入、合法与越界读取、重复值查找、非法位置、空表删除、释放。

```c
/* 测试顺序与讲义附录一、Lab4 的向量完全一致，方便逐行对照 */
int main(void) {
    LinkedList list;
    listInit(&list);
    printf("after init: ");
    listPrint(&list);
    printf("listEmpty = %d, listSize = %d\n\n", listEmpty(&list), listSize(&list));

    /* 1. 依次追加五个样例元素 */
    int samples[] = {18, -1, 42, 18, 65};
    for (int i = 0; i < 5; i++) {
        listPushBack(&list, samples[i]);
    }
    printf("after listPushBack 18, -1, 42, 18, 65: ");
    listPrint(&list);
    printf("\n");

    /* 2. 在秩 2 处插入 25 */
    int ok = listInsert(&list, 2, 25);
    printf("listInsert(2, 25): ok = %d, ", ok);
    listPrint(&list);

    /* 3. 删除秩 1 处的元素，并输出被删除的值 */
    int removed = 777;
    ok = listRemove(&list, 1, &removed);
    printf("listRemove(1): ok = %d, removed = %d, ", ok, removed);
    listPrint(&list);

    /* 4. 在表头插入 7 */
    ok = listPushFront(&list, 7);
    printf("listPushFront(7): ok = %d, ", ok);
    listPrint(&list);
    printf("\n");

    /* 5. 按秩读取：合法的秩与越界的秩 */
    int value = 777;
    ok = listGet(&list, 0, &value);
    printf("listGet(0): ok = %d, value = %d\n", ok, value);
    value = 777;
    ok = listGet(&list, 6, &value);     /* 此刻 size = 6，秩 6 已越界 */
    printf("listGet(6): ok = %d, value = %d\n\n", ok, value);

    /* 6. 按值查找：重复元素只返回第一次出现的秩 */
    printf("listFind(42) = %d\n", listFind(&list, 42));
    printf("listFind(18) = %d\n", listFind(&list, 18));
    printf("listFind(99) = %d\n\n", listFind(&list, 99));

    /* 7. 非法位置：负秩、超过 size 的秩、删除空表 */
    ok = listInsert(&list, -1, 100);
    printf("listInsert(-1, 100): ok = %d\n", ok);
    ok = listInsert(&list, 100, 100);
    printf("listInsert(100, 100): ok = %d\n", ok);

    LinkedList empty;
    listInit(&empty);
    ok = listRemove(&empty, 0, &removed);
    printf("empty listRemove(0): ok = %d\n\n", ok);

    /* 8. 收尾：C 语言没有析构函数，释放必须自己写 */
    printf("before destroy: ");
    listPrint(&list);
    printf("destroy by hand before main ends, empty first, then list:\n");
    listDestroy(&empty);
    listDestroy(&list);
    return 0;
}
```

### 3.4 期望输出与核对重点

```text
after init: [size = 0] 
listEmpty = 1, listSize = 0

after listPushBack 18, -1, 42, 18, 65: [size = 5] 18 -1 42 18 65 

listInsert(2, 25): ok = 1, [size = 6] 18 -1 25 42 18 65 
listRemove(1): ok = 1, removed = -1, [size = 5] 18 25 42 18 65 
listPushFront(7): ok = 1, [size = 6] 7 18 25 42 18 65 

listGet(0): ok = 1, value = 7
listGet(6): ok = 0, value = 777

listFind(42) = 3
listFind(18) = 1
listFind(99) = -1

listInsert(-1, 100): ok = 0
listInsert(100, 100): ok = 0
empty listRemove(0): ok = 0

before destroy: [size = 6] 7 18 25 42 18 65 
destroy by hand before main ends, empty first, then list:
(listDestroy: freed 1 nodes, including the sentinel)
(listDestroy: freed 7 nodes, including the sentinel)
```

核对重点：

- `listInsert(2, 25)` 之后 `25` 在 `-1` 和 `42` 之间；写反插入两句会让链表从 `25` 开始自己指向自己，`listPrint` 停不下来。
- `listRemove(1)` 带回的是 `-1`，不是 `18`。
- `listFind(18)` 是 `1`，不是 `4`：重复元素只返回第一次出现的秩。
- `listGet(6)` 失败后 `value` 仍是 `777`。
- 最后两行是 `listDestroy` 打印的：`empty` 释放 1 个结点（只有哨兵），`list` 释放 7 个（6 个数据加哨兵）。如果第二个数不是 7，说明 `listRemove` 没有 `free` 或 `listInsert` 多建了结点。C 语言里这两句释放要自己写、自己排顺序，Lab7 会看到 C++ 的析构函数怎样把它们变成自动的。

### 3.5 必做：CLion 断点、单步与 Memory View 截图

本项检查你能否把"插入只改指针、删除先断链后释放"与实际内存对应起来。先把程序写完并让输出与 3.4 完全一致，再调试统一 `main`，不为截图改写程序逻辑。操作方法与 Lab5 相同，见课堂发的《CLion 调试与 Memory View 使用讲义》，官方入口参考：[CLion 官方文档：Memory view](https://www.jetbrains.com/help/clion/memory-view.html)。本次只用到设断点、Resume、Step Over 和 Memory View 四个动作。

#### 3.5.1 准备调试

1. CMake 配置选择 **Debug**，运行目标选择 `lab6_list`，重新构建后用 **Debug** 启动。
2. 在 `main` 中 `int ok = listInsert(&list, 2, 25);` 这一行设第一个断点，运行到这里停下。此时五个样例已经追加完，`listInsert(2, 25)` 还没执行。
3. **停在第一个断点之后**，再在 `insertAfter` 里的 `prev->next = p;` 这一行设第二个断点，在 `listRemove` 里的 `free(p);` 这一行设第三个断点。如果一开始就设第二个断点，第一次 `listPushBack(18)` 就会停下来，看到的不是要求的时刻。三张图分别在这三个位置完成，用 **Resume** 在断点之间移动。
4. 三张图必须来自**同一次 Debug 运行**。重新启动程序后地址会变，要重新做整组。

#### 3.5.2 三张截图的固定位置

**A．新结点已接住后面、前驱还没改：`imgs/insert_before_link.png`**

从第一个断点 Resume，程序会停在 `insertAfter` 的 `prev->next = p;` 上。这一行**还没执行**，所以此刻：新结点 `p` 的 `data` 是 25，`p->next` 已经指向 42 所在的结点；`prev` 是 `-1` 所在的结点，`prev->next` **仍然**指向 42 所在的结点，还没有改成 `p`。

在 Variables 中展开 `p` 和 `prev`，各自看到 `data` 和 `next`。右键 `p`，选择 **Show in Memory View**，看到 `p` 指向的 16 个字节：前 4 个字节是 `19 00 00 00`（25），后 8 个字节是 `p->next` 的地址（小端序，低字节在前）。

截图要能看清：`p` 和 `prev` 的 `next` 此刻是**同一个地址**，以及 Memory View 里 `p` 的 16 个字节。

![图 A：新结点已接住后面，前驱还没改](imgs/insert_before_link.png)

**B．前驱改完之后：`imgs/insert_after_link.png`**

按一次 **Step Over**，`prev->next = p;` 执行完。此刻 `prev->next` 变成了 `p` 的地址。在 Memory View 的 Go to 里输入 `prev`，看 `prev` 指向的 16 个字节：前 4 个字节是 `ff ff ff ff`（-1 的补码），后 8 个字节已经从图 A 里 42 所在结点的地址变成了 `p` 的地址。

截图要能看清：`prev->next` 现在等于 `p`，以及 Memory View 里 `prev` 的 16 个字节中后 8 个字节的变化。

![图 B：前驱改完之后](imgs/insert_after_link.png)

**C．已断链、尚未释放：`imgs/remove_before_free.png`**

Resume 到第三个断点，程序停在 `listRemove` 的 `free(p);` 上，这一行还没执行。此时 `unlinkAfter` 已经把 `-1` 所在的结点摘下来了：`list->head->next` 直接指向 18 所在的结点，`list->head->next->next` 指向 25 所在的结点；但 `p`（`-1` 所在结点）**还在内存里**，它的 `next` 还指着 25 所在的结点，`list->size` 仍是 6。

在 Variables 中展开 `list`（它是指向 `main` 里那个结构体的指针）、`head`、`p`。用 Memory View 看 `p` 的 16 个字节：`ff ff ff ff` 加上一个地址。

截图要能看清：`list->head->next` 已经不再是 `p`，但 `p` 的 `data` 和 `next` 都还完整。

![图 C：已断链、尚未释放](imgs/remove_before_free.png)

#### 3.5.3 怎样读结点的 16 个字节

`sizeof(Node)` 是 16：`int data` 占 4 字节，编译器为对齐补 4 字节，`Node* next` 占 8 字节。在 Memory View 里把每行字节数调成 16，一个结点正好一行：

| 位置 | 字节数 | 内容 |
| :--- | :---: | :--- |
| 第 0～3 字节 | 4 | `data`，小端序：25 是 `19 00 00 00`，-1 是 `ff ff ff ff`，42 是 `2a 00 00 00` |
| 第 4～7 字节 | 4 | 对齐填充，内容不定，不要去读 |
| 第 8～15 字节 | 8 | `next` 的地址，小端序：地址 `0x6000012340a0` 显示为 `a0 40 23 01 00 60 00 00` |

把后 8 个字节倒过来读，就是 Variables 里显示的 `next` 的值。这是本次要亲眼验证的事：**结点里存的"下一个"就是一个地址，改指针就是改这 8 个字节。**

结点是一个个 `malloc` 出来的，地址不保证连续，也不保证按大小排，Memory View 一次显示的 256 字节里可能夹着别的结点或与本程序无关的内容，不要试图从窗口里"数出"整条链。

#### 3.5.4 必填：调试观察记录

完成三张截图后，立即把自己环境中的观察结果填入下表。地址记录为实际看到的十六进制值，不能留空或保留"（填写）"字样。

| 记录项目 | 图 A：前驱还没改 | 图 B：前驱改完 | 图 C：已断链、尚未释放 |
| :--- | :--- | :--- | :--- |
| `p` 的地址 | 0xb01840 | 0xb01840 | 0xb01740 |
| `p->data` | 25 | 25 | 18 |
| `p->next` 的地址 | 0xb01740| 0xb01740 | 0xb01780|
| `prev->next` 的地址（图 C 填 `list->head->next`） | 0xb01740| 0xb01840 | 0xb01780 |
| `list->size` | 5 | 6 | 5 |

回答下面两个问题：

1. 图 A 到图 B 之间，`p` 这个结点的 16 个字节有没有变化？`prev` 这个结点的 16 个字节里哪几个变了？

> 在此填写你的回答（不少于 20 字）。
从图A到图B之间，p结点的16个字节没有变化；prev结点存储next指针的8个字节发生了改变，由指向原来后继结点修改为指向新结点p，data数据部分保持不变。
2. 图 C 里 `p` 的 `next` 还指着 25 所在的结点，为什么这不会导致 25 被重复释放？如果把 `listRemove` 里的 `free(p);` 删掉，程序输出会不会变？会少掉什么？

> 在此填写你的回答（不少于 30 字）。
不会造成重复释放，此时的p已经是被摘下的独立结点，它的next只是普通指针，不会自动释放结点。如果删掉 free(p); ，程序输出不会变化，但是内存没有释放，会产生内存泄漏，堆内存得不到回收。
#### 3.5.5 截图检查标准

| 检查项目 | 要求 |
| :--- | :--- |
| 真实界面 | 来自本人电脑的真实 CLion Debug 窗口，使用系统截图功能；不使用手机拍屏、他人图片、示意图或拼接修改的结果 |
| 必要内容 | 每张图能看清 Variables 里指定的指针值，以及 Memory View 里指定结点的 16 个字节；无需展示整个源文件 |
| 三个状态 | 图 A 中 `p->next` 与 `prev->next` 相同；图 B 中 `prev->next` 等于 `p`；图 C 中 `list->head->next` 不等于 `p` 而 `p->next` 仍有效 |
| 地址关系 | 图 A 与图 B 中 `p` 的地址相同；图 C 中 `p` 的地址等于图 A 中 `prev` 的地址（都是 -1 所在的结点） |
| 文件 | 三张真实 PNG，分别保存为指定文件名；每张不超过 5 MB，三张合计不超过 12 MB，最长边不超过 4096 像素 |
| 不作为错误的差异 | CLion 版本、操作系统、主题、地址具体数值、对齐填充字节的内容、窗口排列方式、Variables 面板的展开层次 |

---

## 四、circlist.c：循环单链表与约瑟夫问题

### 4.1 必须使用的框架

结点与 `linkedlist.c` 完全相同，`insertAfter` 一字未改，`unlinkAfter` 只把边界判断从 `NULL` 换成了 `head`。学生补的是遍历、追加、按秩插入删除、查找和与"圈"有关的三个函数。

```c
#include <stdio.h>
#include <stdlib.h>

/* 结点与单链表完全相同 */
typedef struct Node {
    int data;
    struct Node* next;
} Node;

/* 循环链表的约定：最后一个结点的 next 指回 head；空表时 head->next == head */
typedef struct {
    Node* head;         /* 头哨兵，它也在圈上 */
    int size;           /* 有效结点个数，不含哨兵 */
} CircList;

/* --------------------------------------------------------------------------
 * 教师提供：结点级操作，与 linkedlist.c 对照
 * -------------------------------------------------------------------------- */

static Node* newNode(int value) {
    Node* p = (Node*)malloc(sizeof(Node));
    if (p == NULL) {
        printf("malloc failed, exit\n");
        exit(1);
    }
    p->data = value;
    p->next = NULL;
    return p;
}

/* 与单链表完全相同的两句 */
static Node* insertAfter(Node* prev, int value) {
    Node* p = newNode(value);
    p->next = prev->next;
    prev->next = p;
    return p;
}

/* 摘下 prev 后面的结点并返回它；prev 后面是哨兵时返回 NULL。只改指针，不释放。
 * 与单链表的差别只有一处：判断的不再是 NULL，而是 head */
static Node* unlinkAfter(const CircList* list, Node* prev) {
    Node* p = prev->next;
    if (p == list->head) {
        return NULL;
    }
    prev->next = p->next;
    return p;
}

/* 秩为 rank 的结点的前驱，与单链表一字不差 */
static Node* prevOf(const CircList* list, int rank) {
    Node* p = list->head;
    for (int i = 0; i < rank; i++) {
        p = p->next;
    }
    return p;
}

/* 练习 1：最后一个有效结点。从哨兵出发，走到"next 是哨兵"的那个结点为止；空表时返回 head 本身 */
static Node* circLast(const CircList* list) {
    return list->head;  /* TODO */
}

/* --------------------------------------------------------------------------
 * 教师提供：建立与销毁
 * -------------------------------------------------------------------------- */

/* 建立空表：立哨兵，并让它指向自己，圈就成立了 */
void circInit(CircList* list) {
    list->head = newNode(0);
    list->head->next = list->head;
    list->size = 0;
}

/* 逐个摘下并释放，最后释放哨兵。末尾的输出原样保留 */
void circDestroy(CircList* list) {
    int count = 0;
    while (list->size > 0) {
        Node* p = unlinkAfter(list, list->head);
        free(p);
        list->size--;
        count++;
    }
    free(list->head);
    list->head = NULL;
    printf("(circDestroy: freed %d nodes, including the sentinel)\n", count + 1);
}

int circSize(const CircList* list) {
    return list->size;
}

/* --------------------------------------------------------------------------
 * 学生练习
 * -------------------------------------------------------------------------- */

/* 练习 2：空表判断。不许用 size，用指针关系判断 */
int circEmpty(const CircList* list) {
    return 1;   /* TODO：head->next 是否等于 head */
}

/* 练习 3：遍历。从 head->next 出发，回到 head 就停；写成 p != NULL 会死循环 */
void circPrint(const CircList* list) {
    printf("[size = %d] ", list->size);
    /* TODO */
    printf("\n");
}

/* 练习 4：追加到表尾。在 circLast 后面插入，size 加一，返回 1 */
int circPushBack(CircList* list, int value) {
    return 0;   /* TODO：insertAfter(circLast(list), value)；size 加一 */
}

/* 练习 5：按秩插入，合法范围 0 <= rank <= size。与 listInsert 一字不差 */
int circInsert(CircList* list, int rank, int value) {
    return 0;   /* TODO */
}

/* 练习 6：按秩删除，合法范围 0 <= rank < size。与 listRemove 只差 unlinkAfter 多传一个 list */
int circRemove(CircList* list, int rank, int* value) {
    return 0;   /* TODO */
}

/* 练习 7：按值查找，返回第一次出现的秩，找不到返回 -1。停止条件是 head，不是 NULL */
int circFind(const CircList* list, int value) {
    return -1;  /* TODO */
}

/* 练习 8：从第一个结点出发连续走 steps 步，每到一个结点输出它的 data；走到哨兵就跳过哨兵
 * 走的步数可以超过 size，用来证明这是一个圈。空表时输出 (empty) */
void circPrintAround(const CircList* list, int steps) {
    printf("walk %d steps around: ", steps);
    /* TODO */
    printf("\n");
}

/* 练习 9：约瑟夫问题。从第一个结点开始报 1，报到 k 的结点出列（摘下、输出、释放），
 * 从它的下一个结点重新报 1，直到全部出列。哨兵不参与报数，走到它就跳过
 * 提示：维护"下一个报 1 的结点的前驱"prev，初始为 head；出列用 unlinkAfter(list, prev) */
void circJosephus(CircList* list, int k) {
    printf("josephus n = %d, k = %d, out order: ", list->size, k);
    /* TODO */
    printf("\n");
}
```

### 4.2 实现要求

1. `circLast()` 从 `head` 出发循环，停止条件用 `p->next != list->head`，空表时返回 `head` 本身。
2. `circEmpty()` 必须用指针关系判断，不许写 `return list->size == 0`。
3. `circPushBack()` 复用 `insertAfter(circLast(list), value)`，`size` 加一。它是 `O(n)` 的，书面题会问为什么。
4. `circInsert()`、`circRemove()` 分别复用 `insertAfter(prevOf(...), value)` 与 `unlinkAfter(list, prevOf(...))`，写法与 `linkedlist.c` 对照；出列或删除的结点必须 `free`。
5. `circPrintAround(steps)` 走的步数可以超过 `size`；每走到哨兵就跳到哨兵的下一个，哨兵本身不输出。空表时只输出 `(empty)`。
6. `circJosephus(k)` 按 2.7 的五步实现，出列的结点必须 `free`，出列一个 `size` 减一；结束时表为空，哨兵仍在。不得先把数据拷进数组再模拟。
7. 遍历条件一律用 `!= list->head`，不得出现 `!= NULL`。
8. 只能包含 `<stdio.h>` 和 `<stdlib.h>`；不得使用数组、C++ 语法或全局变量。

### 4.3 统一 main 函数

```c
int main(void) {
    CircList ring;
    circInit(&ring);
    printf("after init: ");
    circPrint(&ring);
    printf("circEmpty = %d\n\n", circEmpty(&ring));

    /* 1. 依次追加 1 到 5 */
    for (int i = 1; i <= 5; i++) {
        circPushBack(&ring, i);
    }
    printf("after circPushBack 1 to 5: ");
    circPrint(&ring);

    /* 2. 绕圈走 12 步：单链表到第 5 个就停，循环链表会转回 1 */
    circPrintAround(&ring, 12);
    printf("\n");

    /* 3. 中间插入、删除，再删除表尾并追加：圈必须始终闭合 */
    int ok = circInsert(&ring, 2, 25);
    printf("circInsert(2, 25): ok = %d, ", ok);
    circPrint(&ring);
    int removed = 777;
    ok = circRemove(&ring, 2, &removed);
    printf("circRemove(2): ok = %d, removed = %d, ", ok, removed);
    circPrint(&ring);
    ok = circRemove(&ring, 4, &removed);
    printf("circRemove(4): ok = %d, removed = %d, ", ok, removed);
    circPrint(&ring);
    ok = circPushBack(&ring, 5);
    printf("circPushBack(5): ok = %d, ", ok);
    circPrint(&ring);
    circPrintAround(&ring, 7);
    printf("\n");

    /* 4. 查找与非法位置 */
    printf("circFind(4) = %d\n", circFind(&ring, 4));
    printf("circFind(99) = %d\n", circFind(&ring, 99));
    ok = circInsert(&ring, 6, 100);
    printf("circInsert(6, 100): ok = %d\n", ok);
    ok = circRemove(&ring, 5, &removed);
    printf("circRemove(5): ok = %d, removed = %d\n\n", ok, removed);

    /* 5. 约瑟夫问题 n = 5, k = 3 */
    circJosephus(&ring, 3);
    printf("after josephus: ");
    circPrint(&ring);
    printf("circEmpty = %d\n\n", circEmpty(&ring));

    /* 6. 再来两组：n = 7, k = 2 与 n = 1, k = 1 */
    CircList ring2;
    circInit(&ring2);
    for (int i = 1; i <= 7; i++) {
        circPushBack(&ring2, i);
    }
    circJosephus(&ring2, 2);

    CircList ring3;
    circInit(&ring3);
    circPushBack(&ring3, 1);
    circJosephus(&ring3, 1);

    /* 7. 书面题用：n = 6, k = 2 的结果不在这里给出，请先手工模拟再填写 */
    printf("destroy by hand before main ends:\n");
    circDestroy(&ring3);
    circDestroy(&ring2);
    circDestroy(&ring);
    return 0;
}
```

### 4.4 期望输出与核对重点

```text
after init: [size = 0] 
circEmpty = 1

after circPushBack 1 to 5: [size = 5] 1 2 3 4 5 
walk 12 steps around: 1 2 3 4 5 1 2 3 4 5 1 2 

circInsert(2, 25): ok = 1, [size = 6] 1 2 25 3 4 5 
circRemove(2): ok = 1, removed = 25, [size = 5] 1 2 3 4 5 
circRemove(4): ok = 1, removed = 5, [size = 4] 1 2 3 4 
circPushBack(5): ok = 1, [size = 5] 1 2 3 4 5 
walk 7 steps around: 1 2 3 4 5 1 2 

circFind(4) = 3
circFind(99) = -1
circInsert(6, 100): ok = 0
circRemove(5): ok = 0, removed = 5

josephus n = 5, k = 3, out order: 3 1 5 2 4 
after josephus: [size = 0] 
circEmpty = 1

josephus n = 7, k = 2, out order: 2 4 6 1 5 3 7 
josephus n = 1, k = 1, out order: 1 
destroy by hand before main ends:
(circDestroy: freed 1 nodes, including the sentinel)
(circDestroy: freed 1 nodes, including the sentinel)
(circDestroy: freed 1 nodes, including the sentinel)
```

核对重点：

- `walk 12 steps around` 输出 12 个数而不是 5 个，走到 5 之后回到 1，这是循环链表与单链表唯一的可见区别。
- `circRemove(4)` 删掉表尾的 5 之后，`walk 7 steps around` 仍能从 4 转回 1：删除表尾时前驱的 `next` 必须指回 `head`，圈不能断。`circRemove(5)` 失败后 `removed` 仍是上一次的 5。
- `n = 5, k = 3` 出列顺序 `3 1 5 2 4`，与 2.7 的手工模拟一致。
- 三行释放输出都是 `freed 1 nodes`：约瑟夫已经把数据结点全部出列并释放，销毁时只剩哨兵。如果有一行是 6 或 8，说明 `circJosephus` 没有 `free` 出列者。

---

## 五、选择题与填空

本部分除填空外均为单项选择题。将所选选项前的 `[ ]` 改为 `[x]`，每题只能勾选一项，不要删除其他选项。

### 5.1 插入的顺序

在 `prev` 后面插入新结点 `p`，正确的两句顺序是：

- [ ] A. `prev->next = p; p->next = prev->next;`
- [x] B. `p->next = prev->next; prev->next = p;`
- [ ] C. 两句顺序无所谓，结果一样
- [ ] D. `p->next = prev; prev->next = p;`

### 5.2 写反了会怎样

如果把 5.1 的两句写反，执行之后：

- [ ] A. 编译错误
- [ ] B. 新结点插不进去，链表不变
- [x] C. 新结点的 `next` 指向它自己，原来 `prev` 后面的结点再也走不到
- [ ] D. 新结点插到了表头

### 5.3 为什么先断链后释放

`listRemove` 必须先 `unlinkAfter` 再 `free`，原因是：

- [x] A. `free` 之后 `p->next` 不能再读，前驱就接不上后面了
- [ ] B. 先 `free` 会导致编译错误
- [ ] C. 顺序无所谓，只要两件事都做
- [ ] D. `unlinkAfter` 内部会自动 `free`

### 5.4 释放整条链

`listDestroy` 释放整条链的循环体，正确的写法是：

- [ ] A. `free(p); p = p->next;`
- [x] B. `Node* next = p->next; free(p); p = next;`
- [ ] C. 只写一句 `free(list->head);`，后面的结点会跟着一起释放
- [ ] D. 不用释放，`main` 结束时局部变量自动消失

### 5.5 头哨兵

带头哨兵的单链表，空表的判断条件是：

- [ ] A. `head == NULL`
- [x] B. `head->next == NULL`
- [ ] C. `head->data == 0`
- [ ] D. `size == -1`

### 5.6 为什么 `size` 要单独记录

`LinkedList` 里除了 `head` 还要有一个 `size` 成员，原因是：

- [ ] A. C 语言的结构体至少要有两个成员
- [x] B. 链表无法像向量那样用"首地址加秩"算出有效范围，元素个数要么单独维护，要么每次从头数一遍
- [ ] C. `malloc` 需要知道 `size` 才能申请结点
- [ ] D. 没有 `size` 就无法遍历链表

### 5.7 按秩访问的代价

对带头哨兵的单链表，`listGet(list, rank, &value)` 的时间复杂度是：

- [ ] A. `O(1)`，和向量一样
- [x] B. `O(rank)`，最坏 `O(n)`，因为要从哨兵走 `rank` 步
- [ ] C. `O(log n)`
- [ ] D. `O(n²)`

### 5.8 "链表插入是 O(1)"的前提

下列哪种说法是对的：

- [ ] A. 链表在任何位置插入都是 `O(1)`
- [x] B. 已经拿到前驱结点的地址时，接链是 `O(1)`；按秩找到前驱本身是 `O(n)`
- [ ] C. 只有在表头插入是 `O(1)`，其他位置都是 `O(n²)`
- [ ] D. 链表插入是 `O(1)`，因为不用申请内存

### 5.9 结点的字节

`sizeof(Node)` 是 16 而不是 12，原因是：

- [ ] A. `int` 占 8 个字节
- [ ] B. 指针占 4 个字节，`int` 占 12 个字节
- [x] C. `int` 占 4 字节、指针占 8 字节，编译器为让指针对齐到 8 字节边界补了 4 个字节
- [ ] D. `struct` 关键字占 4 个字节

### 5.10 循环链表的空表

带头哨兵的循环单链表，空表的判断条件是：

- [ ] A. `head->next == NULL`
- [ ] B. `head == NULL`
- [x] C. `head->next == head`，哨兵指向自己
- [ ] D. `head->next->next == head`

### 5.11 循环链表的遍历

对循环单链表写 `for (Node* p = list->head->next; p != NULL; p = p->next)`，结果是：

- [ ] A. 正常输出全部元素
- [ ] B. 只输出第一个元素
- [x] C. 死循环，因为链上没有任何一个 `next` 是 `NULL`
- [ ] D. 编译错误

### 5.12 循环链表的表尾追加

本次 `circPushBack` 的复杂度是 `O(n)`，如果给 `CircList` 加一个 `Node* tail` 成员，`circPushBack` 会变成：

- [ ] A. 仍是 `O(n)`，因为还要遍历一遍检查
- [x] B. `O(1)`，直接 `insertAfter(tail, value)` 再更新 `tail`
- [ ] C. `O(log n)`
- [ ] D. 无法实现，循环链表没有"尾"

### 5.13 填空：程序运行验证

两个程序的实际运行输出是否与 3.4、4.4 的期望输出完全一致？

- [x] A. 两个都一致
- [ ] B. 有不一致

如果选择"有不一致"，请说明哪个程序、哪一行不同以及原因：

> 在此填写原因，选择"两个都一致"时填写"无"。
无
### 5.14 填空：手工模拟约瑟夫

不运行程序，手工模拟 `n = 6, k = 2`（六个人编号 1 到 6，从 1 开始报数，报到 2 的出列），把每一轮的状态填进表里：

| 轮次 | 出列前的圈（从下一个报 1 的人开始写） | 出列者 |
| :---: | :--- | :---: |
| 1 | `1 2 3 4 5 6` | 2 |
| 2 | `3 4 5 1 6 `| 4 |
| 3 | `5 6 1 3 `| 6 |
| 4 | `1 3 5` | 1 |
| 5 | `3 5` | 5 |
| 6 | `3` | 3 |

最终出列顺序：（填写）

### 5.15 填空：单链表与循环单链表的对照

把右列补全，至少填写四行：

| 项目 | 单链表 `LinkedList` | 循环单链表 `CircList` |
| :--- | :--- | :--- |
| 空表条件 | `head->next == NULL` | head->next==head |
| 遍历停止条件 | `p != NULL` | p!=head |
| 最后一个结点的特征 | `p->next == NULL` | p->next==head |
| `unlinkAfter` 里"后面没有结点"的判断 | `p == NULL` | p==head|
| 表尾追加复杂度 | `O(n)` | O(1) |
| 从最后一个结点再走一步到哪里 | 无处可走 | 回到头哨兵head |

### 5.16 填空：向量与链表

用三到五句话回答：本次 `listPushBack` 是 `O(n)`，Lab5 的 `IntVector::pushBack` 是均摊 `O(1)`；但 `listInsert(list, 0, v)` 是 `O(1)`，`IntVector` 在下标 0 处插入是 `O(n)`。两者各自"快"在哪一步、"慢"在哪一步？什么场合该选链表？

> 在此填写你的回答（不少于 60 字）。
单链表 listPushBack 需要从头遍历到链表尾部，因此时间复杂度为O(n)；而它在表头插入结点仅修改指针，不需要移动元素，可以做到O(1)。IntVector向量尾部追加元素多数情况直接写入，均摊复杂度O(1)，但在下标0处插入时，需要把全部原有元素向后挪动，复杂度为O(n)。链表更适合频繁做头部、中间插入删除、随机访问操作较少的场景。
---

## 六、提交要求

将本文件复制到自己的 `学号姓名/Lab6/Lab6.md`，填写选择题答案、填空和 3.5.4 的观察记录。最终只提交以下 **7 个文件，其中包含三张必交截图**：

```text
学号姓名/
└── Lab6/
    ├── CMakeLists.txt
    ├── linkedlist.c
    ├── circlist.c
    ├── Lab6.md
    └── imgs/
        ├── insert_before_link.png
        ├── insert_after_link.png
        └── remove_before_free.png
```

特别注意：

- `CMakeLists.txt` 必须包含 `lab6_list`、`lab6_circ` 两个目标，源文件分别为 `linkedlist.c`、`circlist.c`。
- 两个 `.c` 都必须是能独立编译运行的完整 C 程序，包含框架里的全部函数和本文给出的原样 `main`，不能只提交增量片段。
- `Lab6.md` 必须保留题目结构，填写选择题答案、填空和 3.5.4 的观察记录；3.5.2 中三处 Markdown 图片引用必须原样保留，不得删除或改写路径。
- 每道单项选择题必须且只能将一个 `[ ]` 改为 `[x]`，包括 5.13 的运行验证。
- 填写内容不能留空：5.13 选择"两个都一致"时填写"无"；5.14 六轮和最终顺序全部填写；5.15 至少四行；5.16 填写解释；3.5.4 的表格和两个问题填写完整。
- 三张截图必须满足 3.5 的状态、界面和地址关系要求，文件名与上面的目录树一致，并能在 `Lab6.md` 的 3.5.2 对应位置正常显示；不另交普通运行结果截图。
- 程序和书面题的完成度检查不代替正确性验证；截图会按 3.5 的要求检查。
- 不要提交整个 CLion 项目、`cmake-build-*`、`.idea/`、可执行文件或其他编译产物。
- `Lab6`、`Lab6.md`、`CMakeLists.txt` 和两个 `.c` 文件名的大小写必须完全一致。
- PR 标题必须严格使用 `[学号姓名]Lab6作业提交`，右方括号后不能有空格。
- 一个 PR 只能包含本次 Lab6 的文件，不得修改 Lab3、Lab4、Lab5、其他同学的文件、`homework/`、README 或仓库配置。

---

## 七、截止时间

**2026 年 9 月 28 日 24:00（即 2026 年 9 月 29 日 00:00，北京时间）**

以 GitHub 记录的最后一次向 PR 推送代码的时间为准。不晚于上述时刻创建 PR 并完成最后一次推送不算超时；超过该时刻新建 PR，或向已有 PR 推送任何修改，均算作超时。审核未通过的同学请在截止前完成修改。
