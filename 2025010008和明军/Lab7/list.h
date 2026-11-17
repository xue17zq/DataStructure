//
// Created by 联想电脑 on 2026/9/27.
//

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
    int oldSize = _size;
    while (_size > 0) {
        remove(first());
    }
    return oldSize;
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
    _size++;
    return p->insertAsSucc(e);
}

/* 练习 4b：作为 p 的前驱插入。注意 p 允许是 trailer，也允许是首结点 */
template <typename T>
ListNode<T>* List<T>::insertB(ListNode<T>* p, const T& e) {
    _size++;
    return p->insertAsPred(e);
}

/* --------------------------------------------------------------------------
 * 删除
 * -------------------------------------------------------------------------- */

/* 练习 5：删除结点 p，返回它保存的数据。p 一定是有效结点（不是两个哨兵）。
 * 先取出 data（马上要失去这个结点了），再断链，最后单独一行 delete p; 并让 _size 减一。 */
template <typename T>
T List<T>::remove(ListNode<T>* p) {
    T val = p->data;
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    delete p;
    _size--;
    return val;
}

/* --------------------------------------------------------------------------
 * 查找与按秩访问
 * -------------------------------------------------------------------------- */

/* 练习 6：按值查找，返回结点的"位置"（ListNode<T>*），不是秩。
 * 课本从最后一个有效结点出发，顺着 pred 往前走，遇到头哨兵就说明没找到。
 * 同一元素出现多次时，它命中"最后一个"，与 C 版 listFind 的方向相反。 */
template <typename T>
ListNode<T>* List<T>::find(const T& e) const {
    for (ListNode<T>* p = last(); p != header; p = p->pred) {
        if (p->data == e) {
            return p;
        }
    }
    return nullptr;
}

/* 练习 7：按秩定位，返回秩为 r 的结点。
 * 从首结点一步一步走 r 步；r 越界时返回 nullptr。
 * 链表算不出地址，这就是"按秩访问 O(n)"的全部来源。 */
template <typename T>
ListNode<T>* List<T>::operator[](Rank r) const {
    if (r < 0 || r >= _size) {
        return nullptr;
    }
    ListNode<T>* p = first();
    for (Rank i = 0; i < r; i++) {
        p = p->succ;
    }
    return p;
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
    if (r < 0 || r > _size) {
        return false;
    }
    ListNode<T>* p = (*this)[r];
    if (p == nullptr) {
        p = trailer;
    }
    insertB(p, e);
    return true;
}

/* 练习 8b：对应 C 版 listRemove，删除秩 r 处的结点，数据经引用参数 e 带回。
 * 越界时返回 false 且不修改 e。 */
template <typename T>
bool List<T>::remove(Rank r, T& e) {
    ListNode<T>* p = (*this)[r];
    if (!valid(p)) {
        return false;
    }
    e = remove(p);
    return true;
}

/* 练习 8c：对应 C 版 listGet，按秩读取，失败时不修改 e */
template <typename T>
bool List<T>::get(Rank r, T& e) const {
    ListNode<T>* p = (*this)[r];
    if (!valid(p)) {
        return false;
    }
    e = p->data;
    return true;
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
    for (ListNode<T>* p = trailer->pred; p != header; p = p->pred) {
        std::cout << p->data << ' ';
    }
    std::cout << '\n';
}

#endif