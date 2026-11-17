//
// Created by 联想电
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
    ListNode<T>* x = new ListNode<T>(e, pred, this);
    pred->succ = x;
    pred = x;
    return x;
}

/* 练习 2：在自己后面插入 e，返回新结点 x。与练习 1 左右对称，
 * 把 pred 换成 succ、this 换成 succ 的关系想一遍即可。 */
template <typename T>
ListNode<T>* ListNode<T>::insertAsSucc(const T& e) {
    ListNode<T>* x = new ListNode<T>(e, this, succ);
    succ->pred = x;
    succ = x;
    return x;
}

#endif
