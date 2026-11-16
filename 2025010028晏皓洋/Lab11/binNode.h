#ifndef BINNODE_H
#define BINNODE_H

/* 仿照课堂示例，定义二叉树节点的模板。
 * 整个文件只出现这一个模板 struct：三个数据成员加两个构造函数。 */
template <typename T>
struct BinNode {
    T data;          /* 元素本身 */
    BinNode<T>* lc;  /* 左孩子，无左孩子时为 nullptr */
    BinNode<T>* rc;  /* 右孩子，无右孩子时为 nullptr */

    /* 默认构造函数：把两个孩子指针置为 nullptr，data 保持默认值。 */
    BinNode() : lc(nullptr), rc(nullptr) {}

    /* 由元素 e 构造的构造函数，同样把两个孩子指针置为 nullptr。 */
    BinNode(const T& e) : data(e), lc(nullptr), rc(nullptr) {}
};

#endif
