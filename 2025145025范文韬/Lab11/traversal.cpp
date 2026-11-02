#include <iostream>
#include <queue>  /* 层次遍历使用，与课堂示例一致 */

#include "binNode.h"

using Node = BinNode<char>;

/* 学生完成：先序遍历（根 -> 左 -> 右），递归实现，访问即输出。 */
void preorder(Node* root) {
    /* 完成 1：先写递归出口，再按“根、左、右”的顺序编写。 */
    if (root == nullptr) {
        return;
    }
    std::cout << root->data << ' ';
    preorder(root->lc);
    preorder(root->rc);
}

/* 学生完成：中序遍历（左 -> 根 -> 右），递归实现，访问即输出。 */
void inorder(Node* root) {
    /* 完成 2：与先序相比，只移动访问根节点的那一行。 */
    if (root == nullptr) {
        return;
    }
    inorder(root->lc);
    std::cout << root->data << ' ';
    inorder(root->rc);
}

/* 学生完成：后序遍历（左 -> 右 -> 根），递归实现，访问即输出。 */
void postorder(Node* root) {
    /* 完成 3：访问根节点的动作放在两次递归之后。 */
    if (root == nullptr) {
        return;
    }
    postorder(root->lc);
    postorder(root->rc);
    std::cout << root->data << ' ';
}

/* 学生完成：层次遍历，借助先进先出队列实现，访问即输出。 */
void levelOrder(Node* root) {
    /* 完成 4：先判空，根节点入队；每次出队一个节点访问，再让它的孩子按先左后右入队。 */
    if (root == nullptr) {
        return;
    }
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* p = q.front();
        q.pop();
        std::cout << p->data << ' ';
        if (p->lc != nullptr) {
            q.push(p->lc);
        }
        if (p->rc != nullptr) {
            q.push(p->rc);
        }
    }
}

/* 教师提供：按“先左右子树、后根”的顺序释放整棵树，调用一次即可避免内存泄漏。 */
void destroyTree(Node* root) {
    if (root == nullptr) {
        return;
    }
    destroyTree(root->lc);
    destroyTree(root->rc);
    delete root;
}

int main() {
    /* 完成 5：按 Lab11.md 第三节的树形图手动创建指定的二叉树，
     * 根节点指针命名为 root，供下方的输出框架使用。
     * 第一步逐个 new 节点；第二步给 data 赋值；第三步连接 lc / rc。 */
    // 第一步：逐个 new 出 6 个节点
    Node* A = new Node;
    Node* B = new Node;
    Node* C = new Node;
    Node* D = new Node;
    Node* E = new Node;
    Node* F = new Node;

    // 第二步：给每个节点的 data 赋值
    A->data = 'A';
    B->data = 'B';
    C->data = 'C';
    D->data = 'D';
    E->data = 'E';
    F->data = 'F';

    // 第三步：指定父子关系
    A->lc = B;  A->rc = C;
    B->lc = D;  B->rc = E;
    C->lc = nullptr;  C->rc = F;   /* C 没有左孩子，显式写出便于对照树形图 */
    D->lc = nullptr;  D->rc = nullptr;  /* 叶子 */
    E->lc = nullptr;  E->rc = nullptr;  /* 叶子 */
    F->lc = nullptr;  F->rc = nullptr;  /* 叶子 */

    Node* root = A;  /* 根节点指针供输出框架使用 */

    /* 以下输出框架由教师提供：四种遍历各占一行，先打印标签，再由遍历函数输出序列。 */
    std::cout << "先序遍历（根-左-右）：";
    preorder(root);
    std::cout << '\n';

    std::cout << "中序遍历（左-根-右）：";
    inorder(root);
    std::cout << '\n';

    std::cout << "后序遍历（左-右-根）：";
    postorder(root);
    std::cout << '\n';

    std::cout << "层次遍历（从上到下、从左到右）：";
    levelOrder(root);
    std::cout << '\n';

    destroyTree(root);
    return 0;
}
