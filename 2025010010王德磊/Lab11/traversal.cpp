#include <iostream>
#include <queue>  /* 层次遍历使用，与课堂示例一致 */

#include "binNode.h"

using Node = BinNode<char>;

/* 学生完成：先序遍历（根 -> 左 -> 右），递归实现，访问即输出。 */
void preorder(Node* root) {
    if (root == nullptr) {
        return;
    }
    std::cout << root->data << ' ';
    preorder(root->lc);
    preorder(root->rc);
}

/* 学生完成：中序遍历（左 -> 根 -> 右），递归实现，访问即输出。 */
void inorder(Node* root) {
    if (root == nullptr) {
        return;
    }
    inorder(root->lc);
    std::cout << root->data << ' ';
    inorder(root->rc);
}

/* 学生完成：后序遍历（左 -> 右 -> 根），递归实现，访问即输出。 */
void postorder(Node* root) {
    if (root == nullptr) {
        return;
    }
    postorder(root->lc);
    postorder(root->rc);
    std::cout << root->data << ' ';
}

/* 学生完成：层次遍历，借助先进先出队列实现，访问即输出。 */
void levelOrder(Node* root) {
    if (root == nullptr) {
        return;
    }
    std::queue<Node*> Q;
    Q.push(root);
    while (!Q.empty()) {
        Node* p = Q.front();
        Q.pop();
        std::cout << p->data << ' ';
        if (p->lc != nullptr) {
            Q.push(p->lc);
        }
        if (p->rc != nullptr) {
            Q.push(p->rc);
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
    /* 按 Lab11.md 第三节的树形图手动创建指定的二叉树：
     * 第一步逐个 new 节点；第二步给 data 赋值；第三步连接 lc / rc。 */
    Node* A = new Node;
    Node* B = new Node;
    Node* C = new Node;
    Node* D = new Node;
    Node* E = new Node;
    Node* F = new Node;

    A->data = 'A';
    B->data = 'B';
    C->data = 'C';
    D->data = 'D';
    E->data = 'E';
    F->data = 'F';

    A->lc = B;
    A->rc = C;
    B->lc = D;
    B->rc = E;
    C->lc = nullptr;  /* C 只有右孩子，左孩子显式置空便于对照树形图 */
    C->rc = F;
    D->lc = nullptr;
    D->rc = nullptr;
    E->lc = nullptr;
    E->rc = nullptr;
    F->lc = nullptr;
    F->rc = nullptr;

    Node* root = A;  /* 根节点指针供下方的输出框架使用 */

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
