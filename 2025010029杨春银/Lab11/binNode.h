#ifndef BINNODE_H
#define BINNODE_H

/* 学生完成：仿照课堂示例，定义二叉树节点的模板。
 * 整个文件只允许出现这一个模板 struct：三个数据成员加两个构造函数。 */
template <typename T>
struct BinNode {
    /* TODO 1：三个数据成员：元素本身（类型 T）、左孩子指针、右孩子指针。 */
    T data;
    BinNode<T> *lc;
    BinNode<T> *rc;


    /* TODO 2：默认构造函数，把两个孩子指针置为 nullptr，data 保持默认值。 */
    BinNode() : data(), lc(nullptr), rc(nullptr) {}


    /* TODO 3：由元素 e 构造的构造函数，同样把两个孩子指针置为 nullptr。 */
    BinNode(T data) : data(data), lc(nullptr), rc(nullptr) {}


};

#endif
