#include <iostream>
#include <queue>  /* 和 Lab11 的层次遍历一样，使用 std::queue */

#include "graph.h"

/* 深度优先：沿用 Lab11 的先序遍历，先访问当前顶点，再递归访问邻接点。
 * 二叉树只有 lc/rc 两个孩子；图要用循环扫描这一行，寻找所有邻接点。 */
void dfs(const Graph& graph, int v, bool visited[]) {
    /* 已访问就直接返回；否则标记当前顶点并输出其标签，
     * 再按编号递增扫描第 v 行，对存在边且未访问的邻接点递归。 */
    if (visited[v]) {
        return;
    }
    visited[v] = true;
    std::cout << graph.vertices[v] << " ";

    for (int w = 0; w < graph.numVertices; ++w) {
        if (graph.edge[v][w] == 1 && !visited[w]) {
            dfs(graph, w, visited);
        }
    }
}

/* 一次搜索不一定覆盖全图：用外层循环补上尚未访问的部分。 */
void dfsAll(const Graph& graph) {
    /* visited 只初始化一次，由外层循环与各次递归共同维护。 */
    bool visited[MAXV] = {};
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            dfs(graph, v, visited);
        }
    }
}

/* 广度优先：沿用 Lab11 的层次遍历，先入队，再循环出队、访问、扩展。
 * 树的队列保存 Node*；本次队列保存顶点编号 int。 */
void bfs(const Graph& graph, int v, bool visited[]) {
    /* 已访问就直接返回；起点先标记再入队，之后每轮出队一个顶点、
     * 输出其标签，并把本行所有未访问的邻接点标记后入队。 */
    if (visited[v]) {
        return;
    }

    std::queue<int> q;
    visited[v] = true;
    q.push(v);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        std::cout << graph.vertices[u] << " ";

        for (int w = 0; w < graph.numVertices; ++w) {
            if (graph.edge[u][w] == 1 && !visited[w]) {
                visited[w] = true;
                q.push(w);
            }
        }
    }
}

void bfsAll(const Graph& graph) {
    /* visited 只初始化一次；外层循环遇到未访问顶点就做一次单次搜索。 */
    bool visited[MAXV] = {};
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            bfs(graph, v, visited);
        }
    }
}
