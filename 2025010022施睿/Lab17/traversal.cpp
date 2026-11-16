#include <iostream>
#include <queue>  /* 和 Lab11 的层次遍历一样，使用 std::queue */

#include "graph.h"

/* 深度优先：沿用 Lab11 的先序遍历，先访问当前顶点，再递归访问邻接点。
 * 二叉树只有 lc/rc 两个孩子；图要用循环扫描这一行，寻找所有邻接点。 */
void dfs(const Graph& graph, int v, bool visited[]) {
    if (visited[v]) {
        return;                             // 已访问：图中可能有环，必须在此截断
    }
    visited[v] = true;
    std::cout << graph.vertices[v] << " ";

    /* 循环上界是 numVertices，不是 numVertices - 1，
     * 否则编号最大的顶点永远扫不到。有向图只扫行，不扫列。 */
    for (int w = 0; w < graph.numVertices; ++w) {
        if (graph.edge[v][w] == 1 && !visited[w]) {
            dfs(graph, w, visited);
        }
    }
}

/* 一次搜索不一定覆盖全图：用外层循环补上尚未访问的部分。 */
void dfsAll(const Graph& graph) {
    /* bool visited[MAXV] = {}; 初始化 MAXV 个 false。
     * 写成 bool visited[] = { false }; 只会创建一个元素，后续下标越界。 */
    bool visited[MAXV] = {};
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            dfs(graph, v, visited);         // 用 v，不能写死 0
        }
    }
}

/* 广度优先：沿用 Lab11 的层次遍历，先入队，再循环出队、访问、扩展。
 * 树的队列保存 Node*；本次队列保存顶点编号 int。 */
void bfs(const Graph& graph, int v, bool visited[]) {
    if (visited[v]) {
        return;
    }

    std::queue<int> q;
    visited[v] = true;                      // 入队前就标记
    q.push(v);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        std::cout << graph.vertices[u] << " ";

        for (int w = 0; w < graph.numVertices; ++w) {
            if (graph.edge[u][w] == 1 && !visited[w]) {
                visited[w] = true;          // 标记发生在入队时，防止重复入队
                q.push(w);
            }
        }
    }
}

void bfsAll(const Graph& graph) {
    bool visited[MAXV] = {};
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            bfs(graph, v, visited);         // 已访问时跳过，不能用 return
        }
    }
}
