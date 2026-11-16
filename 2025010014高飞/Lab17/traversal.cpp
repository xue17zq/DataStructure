#include <iostream>
#include <queue>  /* 和 Lab11 的层次遍历一样，使用 std::queue */

#include "graph.h"

/* 深度优先：沿用 Lab11 的先序遍历，先访问当前顶点，再递归访问邻接点。
 * 二叉树只有 lc/rc 两个孩子；图要用循环扫描这一行，寻找所有邻接点。 */
void dfs(const Graph& graph, int v, bool visited[]) {
    /* 图中存在环和交叉边，同一顶点可从多条路径到达；
     * 缺少访问标记时递归会在环上反复进入，最终栈溢出。 */
    if (visited[v]) {
        return;
    }

    visited[v] = true;
    std::cout << graph.vertices[v] << " ";

    /* 上界必须是 numVertices：写成 numVertices - 1 时编号最大的顶点扫不到。
     * 有向图只沿出边前进，因此只扫第 v 行，不扫第 v 列。 */
    for (int w = 0; w < graph.numVertices; ++w) {
        if (graph.edge[v][w] == 1 && !visited[w]) {
            dfs(graph, w, visited);
        }
    }
}

/* 一次搜索不一定覆盖全图：用外层循环补上尚未访问的部分。 */
void dfsAll(const Graph& graph) {
    /* bool visited[MAXV] = {} 初始化 MAXV 个 false。
     * 写成 bool visited[] = { false } 只会创建一个元素，后续下标越界。
     * visited 只创建一次，不能在外层循环里重置，否则各连通分量会互相覆盖。 */
    bool visited[MAXV] = {};

    /* 外层必须传入循环变量 v，写死起点 0 时首个连通分量结束后其余分量无法访问。 */
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            dfs(graph, v, visited);
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
    visited[v] = true;      // 标记发生在入队时
    q.push(v);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        std::cout << graph.vertices[u] << " ";

        for (int w = 0; w < graph.numVertices; ++w) {
            if (graph.edge[u][w] == 1 && !visited[w]) {
                /* 先标记再入队：若推迟到出队时标记，
                 * 同一顶点会被多个前驱重复入队，输出出现重复。 */
                visited[w] = true;
                q.push(w);
            }
        }
    }
}

void bfsAll(const Graph& graph) {
    bool visited[MAXV] = {};

    /* 跳过已访问顶点用 continue，不能写成 return：
     * 写成 return 时首个连通分量结束后函数整体退出，后续分量与孤立点不会输出。 */
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            bfs(graph, v, visited);
        }
    }
}
