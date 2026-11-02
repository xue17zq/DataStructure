#ifndef BINTREE_H
#define BINTREE_H

#include <string>

#include "binNode.h"

/* 学生完成：按课本 BinTree 的组织方式，在简化的二叉链表上实现一棵无序二叉树。
 *
 * 本次只用 char，所以树不是模板：元素的类型直接写死为 char，省掉一层 “<T>”。
 *
 * 本次只练习四种操作：建树、查找、删除、修改；不涉及有序性（不要求“左小右大”），
 * 也不涉及析构释放、深拷贝、平衡与旋转。
 * 因为没有有序性，三件事会跟着变：查找只能遍历整棵树（Θ(n)，不是 Θ(h)）；
 * 删除只允许删叶子（无序树没法用“中序后继”顶替）；修改可以直接改写节点的 data。
 *
 * 建树的界面是“一个字符串”：每个字符是一个元素，'#' 表示空子树，
 * '\0' 表示序列结束，写法与课本 CreateBiTree 的先序递归一致。
 *
 * 树只有一个数据成员 _root，不要增加其他成员（例如计数、高度、父指针）；
 * 除 buildIn、searchIn 这两个私有静态递归外，不要再写其他辅助函数，
 * 四种操作各自在下面的函数体内完成。
 *
 * 教师另外提供了 root() 与文件末尾的三个遍历函数（Lab11 已经实现过，这里直接搬来），
 * 它们只供 main 核对整棵树，不属于学生任务，也不要改动。
 */
class BinTree {
private:
    BinNodePosi _root = nullptr;       /* 树根；空树时为 nullptr */

    /* 遍历查找：按“根、左、右”把整棵树走一遍，返回保存 e 的那一格指针槽；
     * 落空时返回某个空槽（无序树里没有“元素该待的位置”，调用者只看槽里是不是 nullptr）。
     * 参数 slot 是引用：传 v->lc / v->rc 时，返回值就直接是父节点的孩子指针。
     */
    static BinNodePosi& searchIn(BinNodePosi& slot, char e);   /* TODO 2 */

    /* 先序扩展序列建树：每读一个字符，'#' 表示空子树，'\0' 表示序列结束，
     * 否则建立节点，再递归建它的左、右子树。i 是当前读到第几个字符，按引用推进。
     */
    static BinNodePosi buildIn(const char* seq, int& i);   /* TODO 3 */

public:
    /* 教师提供：返回树根，供文件末尾的三种遍历核对整棵树（与课本 BinTree::root() 一致）。
     * 学生不需要实现它，也不要再增加其他访问接口。 */
    BinNodePosi root() const { return _root; }

    /* 增：按先序扩展序列建树，例如 "ABD##E##C#F##"。
     * 空树时建树成功返回 true；树已经非空时拒绝并返回 false。
     * 空串或全是 '#' 的串建出的是空树，这种情况同样算成功。
     */
    bool build(const char* seq);                          /* TODO 3 */

    /* 查：遍历整棵树查找 e。命中返回保存 e 的那一格槽（槽里就是那个节点），
     * 落空返回空槽（槽里是 nullptr）。没有有序性，所以这里只走遍历，不做二分下降。
     */
    BinNodePosi& search(char e);                          /* TODO 2 */

    /* 删：删除元素 e。只允许删除叶子：不是叶子、或者树中没有 e，都不做任何改动并返回 false。 */
    bool remove(char e);                                  /* TODO 4 */

    /* 改：把元素 oldValue 改成 newValue，返回是否真的修改成功。
     * 旧值不存在、新值已存在、两个值相等这三种情况都不修改，返回 false。
     */
    bool update(char oldValue, char newValue);            /* TODO 5 */
};

BinNodePosi& BinTree::searchIn(BinNodePosi& slot, char e) {
    /* TODO 2 之一：遍历查找。先写递归出口，再按“根、左、右”的顺序找：
     * 当前槽为空、或者槽里就是这个元素，直接返回 slot；
     * 否则先递归查左子树，左子树找到就返回它那一个槽，没找到再递归查右子树并返回。
     * 关键：要返回“指针槽的引用”，不能返回节点指针的副本——删除时要靠它把叶子摘下来。
     * 与有序树的同类函数相比，这里每层两支都要走，所以是 Θ(n) 的遍历，不是 Θ(h) 的下降。
     */
    if (!slot || slot->data == e) return slot; // 命中或空槽直接返回
    BinNodePosi& leftRes = searchIn(slot->lc, e);
    if (leftRes) return leftRes; // 如果在左边找到了，直接返回左边的结果，不再搜右边
    return searchIn(slot->rc, e); // 否则返回右边的结果
}



BinNodePosi& BinTree::search(char e) {
    /* TODO 2 之二：按课本写法，把 _root 交给 searchIn。树为空时返回的就是 _root 自身。
     */ return searchIn(_root, e);
}

BinNodePosi BinTree::buildIn(const char* seq, int& i) {
    /* TODO 3 之一：先序扩展序列建树 */
    char ch = seq[i++]; // 先取出当前字符，i 后移

    // 读到结束符或空树符，返回空指针
    if (ch == '\0' || ch == '#') {
        return nullptr;
    }

    // 否则用当前字符创建新节点
    BinNodePosi node = new BinNode{ch, nullptr, nullptr};

    // 递归建左右子树，并直接接到当前节点的 lc 和 rc 上
    node->lc = buildIn(seq, i);
    node->rc = buildIn(seq, i);

    // 最后返回建好的这棵子树的根节点
    return node;
}

bool BinTree::build(const char* seq) {
    /* TODO 3 之二：先判断非空 */
    if (_root != nullptr) {
        return false; // 树非空，拒绝建树
    }

    int i = 0; // 初始化索引
    // 直接把 buildIn 返回的根节点赋给 _root
    _root = buildIn(seq, i);

    return true;
}

bool BinTree::remove(char e) {
    /* TODO 4：... */
    BinNodePosi& slot = search(e); // 获取槽的引用

    if (!slot) return false; // 槽为空，说明不存在

    // 判断是否有孩子 (使用教师提供的宏，或者自己写 if(slot->lc || slot->rc))
    if (HasChild(*slot)) return false; // 有孩子，不是叶子，拒绝删除

    // 是叶子，执行删除
    delete slot;    // 释放内存
    slot = nullptr; // 关键一步：把父节点的指针（即这个槽）置空，防止野指针

    return true;
}

bool BinTree::update(char oldValue, char newValue) {
    /* TODO 5：... */
    // 1. 检查新旧值是否相等
    if (oldValue == newValue) return false;

    // 2. 查找旧值所在槽
    BinNodePosi& slot = search(oldValue);
    if (!slot) return false; // 旧值不存在

    // 3. 检查新值是否已经存在 (调用search，如果返回非空说明存在)
    if (search(newValue) != nullptr) return false;

    // 4. 原地修改
    slot->data = newValue;

    return true;
}

/* ---------- 教师提供：三种遍历 ----------
 * 这三个函数是 Lab11 已经实现过的内容，这里把“访问节点”固定成“把字符追加到结果串”，
 * 直接返回遍历序列，供 main 核对整棵树。本次只用 char，所以不需要模板或访问器类型。
 * 用法：travPre(T.root()) 返回形如 "ABDEF" 的字符串，空树返回空串。
 * 三个函数之间的差别仍然只有“追加当前字符”那一行的位置，与 Lab11 的结论一致。
 */
inline void travPre(BinNodePosi x, std::string& out) {
    if (x == nullptr) {
        return;
    }
    out.push_back(x->data);
    travPre(x->lc, out);
    travPre(x->rc, out);
}

inline std::string travPre(BinNodePosi x) {
    std::string out;
    travPre(x, out);
    return out;
}

inline void travIn(BinNodePosi x, std::string& out) {
    if (x == nullptr) {
        return;
    }
    travIn(x->lc, out);
    out.push_back(x->data);
    travIn(x->rc, out);
}

inline std::string travIn(BinNodePosi x) {
    std::string out;
    travIn(x, out);
    return out;
}

inline void travPost(BinNodePosi x, std::string& out) {
    if (x == nullptr) {
        return;
    }
    travPost(x->lc, out);
    travPost(x->rc, out);
    out.push_back(x->data);
}

inline std::string travPost(BinNodePosi x) {
    std::string out;
    travPost(x, out);
    return out;
}

#endif
