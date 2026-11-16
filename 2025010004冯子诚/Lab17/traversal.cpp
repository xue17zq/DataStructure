#include <iostream>
#include <queue>  /* 和 Lab11 的层次遍历一样，使用 std::queue */

#include "graph.h"

/* 深度优先：沿用 Lab11 的先序遍历，先访问当前顶点，再递归访问邻接点。
 * 二叉树只有 lc/rc 两个孩子；图要用循环扫描这一行，寻找所有邻接点。 */
void dfs(const Graph& graph, int v, bool visited[]) {
    /* 与先序遍历同构：已访问相当于"空指针直接返回"。 */
    if (visited[v]) {
        return;                     /* 已访问过，防止环上重复进入 */
    }

    visited[v] = true;              /* 先标记，再访问（输出） */
    std::cout << graph.vertices[v] << " ";

    for (int w = 0; w < graph.numVertices; ++w) {
        if (graph.edge[v][w] == 1 && !visited[w]) {
            dfs(graph, w, visited); /* 按编号递增深入未访问邻接点 */
        }
    }
}

/* 一次搜索不一定覆盖全图：用外层循环补上尚未访问的部分。 */
void dfsAll(const Graph& graph) {
    bool visited[MAXV] = {};        /* MAXV 个位置全部初始化为 false */
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            dfs(graph, v, visited); /* 每个连通分量/孤立点各补一次 */
        }
    }
}

/* 广度优先：沿用 Lab11 的层次遍历，先入队，再循环出队、访问、扩展。
 * 树的队列保存 Node*；本次队列保存顶点编号 int。 */
void bfs(const Graph& graph, int v, bool visited[]) {
    /* 与层次遍历同构：队列保存编号，入队时即标记。 */
    if (visited[v]) {
        return;                     /* 已发现过，直接返回 */
    }

    std::queue<int> q;
    visited[v] = true;              /* 标记发生在入队时，防止重复入队 */
    q.push(v);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        std::cout << graph.vertices[u] << " ";

        for (int w = 0; w < graph.numVertices; ++w) {
            if (graph.edge[u][w] == 1 && !visited[w]) {
                visited[w] = true;  /* 先标记再入队 */
                q.push(w);
            }
        }
    }
}

void bfsAll(const Graph& graph) {
    bool visited[MAXV] = {};        /* MAXV 个位置全部初始化为 false */
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            bfs(graph, v, visited); /* 跳过已访问分量，不能 return */
        }
    }
}
