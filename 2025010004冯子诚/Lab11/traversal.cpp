#include <iostream>
#include <queue>  /* 层次遍历使用，与课堂示例一致 */

#include "binNode.h"

using Node = BinNode<char>;

/* 先序遍历（根 -> 左 -> 右），递归实现，访问即输出。 */
void preorder(Node* root) {
    if (root == nullptr) {
        return;  /* 递归出口：空树直接返回 */
    }
    std::cout << root->data << ' ';  /* 先访问根 */
    preorder(root->lc);
    preorder(root->rc);
}

/* 中序遍历（左 -> 根 -> 右），递归实现，访问即输出。 */
void inorder(Node* root) {
    if (root == nullptr) {
        return;  /* 递归出口：空树直接返回 */
    }
    inorder(root->lc);               /* 左子树处理完 */
    std::cout << root->data << ' ';  /* 才轮到根 */
    inorder(root->rc);
}

/* 后序遍历（左 -> 右 -> 根），递归实现，访问即输出。 */
void postorder(Node* root) {
    if (root == nullptr) {
        return;  /* 递归出口：空树直接返回 */
    }
    postorder(root->lc);             /* 两棵子树都处理完 */
    postorder(root->rc);
    std::cout << root->data << ' ';  /* 才访问根 */
}

/* 层次遍历，借助先进先出队列实现，访问即输出。 */
void levelOrder(Node* root) {
    if (root == nullptr) {
        return;  /* 空树直接返回 */
    }
    std::queue<Node*> Q;  /* 只做 push、front、pop、empty 操作 */
    Q.push(root);
    while (!Q.empty()) {
        Node* p = Q.front();
        Q.pop();
        std::cout << p->data << ' ';  /* 出队即访问 */
        if (p->lc) {
            Q.push(p->lc);
        }
        if (p->rc) {
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
    /* 手动创建 Lab11.md 第三节指定的二叉树：
     * A 的左、右孩子是 B、C；B 的左、右孩子是 D、E；C 只有右孩子 F。
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

    A->lc = B;  A->rc = C;
    B->lc = D;  B->rc = E;
    C->lc = nullptr;  C->rc = F;

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
