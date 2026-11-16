#include <iostream>
#include <queue>  /* 层次遍历使用，与课堂示例一致 */

#include "binNode.h"

using Node = BinNode<char>;

/* 学生完成：先序遍历（根 -> 左 -> 右），递归实现，访问即输出。 */
void preorder(Node* root) {
    if (root==nullptr) {
        return;
    }
    std::cout<<root->data;
    preorder(root->lc);
    preorder(root->rc);
    /* TODO 1：先写递归出口，再按“根、左、右”的顺序编写。 */
}

/* 学生完成：中序遍历（左 -> 根 -> 右），递归实现，访问即输出。 */
void inorder(Node* root) {
    if (root==nullptr) {
        return;
    }
    preorder(root->lc);
    std::cout<<root->data;
    preorder(root->rc);
    /* TODO 2：与先序相比，只移动访问根节点的那一行。 */
}

/* 学生完成：后序遍历（左 -> 右 -> 根），递归实现，访问即输出。 */
void postorder(Node* root) {
    if (root==nullptr) {
        return;
    }
    postorder(root->lc);
    postorder(root->rc);
    std::cout<<root->data;
    /* TODO 3：访问根节点的动作放在两次递归之后。 */
}

/* 学生完成：层次遍历，借助先进先出队列实现，访问即输出。 */
void levelOrder(Node* root) {
    if (root==nullptr) {
        return;
    }
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* cur=q.front();
        q.pop();
        std::cout<<cur->data;
        if (cur->lc!=nullptr) {
            q.push(cur->lc);
        }
        if (cur->rc!=nullptr) {
            q.push(cur->rc);
        }

    }



    /* TODO 4：先判空，根节点入队；每次出队一个节点访问，再让它的孩子按先左后右入队。 */
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

    /* TODO 5：按 Lab11.md 第三节的树形图手动创建指定的二叉树，
     * 根节点指针命名为 root，供下方的输出框架使用。
     * 第一步逐个 new 节点；第二步给 data 赋值；第三步连接 lc / rc。 */
    Node* root = nullptr;  /* 把这一行替换为手动建树的代码 */
    Node* A=new Node('A');
    Node* B=new Node('B');
    Node* C=new Node('C');
    Node* D=new Node('D');
    Node* E=new Node('E');
    Node* F=new Node('F');

    A->lc=B;
    A->rc=C;
    B->lc=D;
    B->rc=E;
    C->lc=nullptr;
    C->rc=F;

    root=A;

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
