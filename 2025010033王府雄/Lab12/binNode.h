#ifndef BINNODE_H
#define BINNODE_H

/* 按邓俊辉《数据结构（C++ 语言版）》BinNode / BinNodePosi 的命名与组织方式
 * 编写的教学子集。
 *
 * 本次只用 char，所以节点不是模板：data 的类型直接写死为 char。
 * 课本用模板是为了让同一套代码服务多种元素类型；本实验只需要一种，
 * 去掉模板能让代码短一截，也少一层 “<T>” 的语法负担。
 *
 * 节点仍然只有 data、lc、rc 三个数据成员；parent、height、npl、color、
 * 构造函数、succ、size 等一律没有。
 * 课本用 binnode_macro_basic.h 下的宏做结构判定，本次给出其中不依赖 parent 的五个，
 * 附在文件末尾（教师提供，不要求修改）。
 * 由此产生的一个直接后果：只能自根沿孩子指针向下移动，无法在 O(1) 时间内回到父节点；
 * 因此本次删除时用“指针槽的引用”代替 parent 完成接链，见 Lab12.md 第二、三节。
 */

struct BinNode;                     /* 先声明一下，好让下面的别名能用到它 */

using BinNodePosi = BinNode*;       /* 课本记法：元素类型为 char 的节点指针 */

struct BinNode {
    /* TODO 1：只填写三个数据成员，不要添加其他成员：
     *   1) 元素本身，类型为 char；
     *   2) 左孩子，类型用上面定义的 BinNodePosi；
     *   3) 右孩子，类型同上。
     * 左、右两条的书写先后可以由你决定，只要与这里的注释保持一致即可。
     * 没有孩子的节点，对应的孩子指针置为 nullptr（由建树操作建立节点时设置）。
     * 这里只写三条数据成员声明：没有成员函数，也没有构造函数，
     * 于是 BinNode 是聚合体，可以直接用花括号依次写出三个成员来建立节点。
     */
    char data;              /* 元素本身 */
    BinNodePosi lc;         /* 左孩子，没有时为 nullptr */
    BinNodePosi rc;         /* 右孩子，没有时为 nullptr */
};

/* 与课本 binnode_macro_basic.h 对应的结构判定宏（教师提供：不要求修改，也不要删除）。
 * 课本的同名宏都用到了 parent，这里按本次简化的节点只依赖 lc、rc；
 * 参数按课本习惯写成节点本身，调用时写作 IsLeaf(*x)、HasLChild(*x)、HasBothChildren(*x)。
 */
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChildren(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))

#endif
