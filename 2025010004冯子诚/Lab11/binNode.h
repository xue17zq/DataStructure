#ifndef BINNODE_H
#define BINNODE_H

/* 仿照课堂示例定义的二叉树节点模板：三个数据成员加两个构造函数。 */
template <typename T>
struct BinNode {
    T data;          /* 元素本身 */
    BinNode<T>* lc;  /* 左孩子指针，无左孩子时为 nullptr */
    BinNode<T>* rc;  /* 右孩子指针，无右孩子时为 nullptr */

    /* 默认构造函数：两个孩子指针置空，data 保持默认值。 */
    BinNode() : data(), lc(nullptr), rc(nullptr) {}

    /* 由元素 e 构造：同样把两个孩子指针置空。 */
    BinNode(T const& e) : data(e), lc(nullptr), rc(nullptr) {}
};

#endif
