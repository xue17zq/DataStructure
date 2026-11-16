#include <iostream>
#include <queue>  /* 和 Lab11 的层次遍历一样，使用 std::queue */

#include "graph.h"

/* 深度优先：沿用 Lab11 的先序遍历，先访问当前顶点，再递归访问邻接点。
 * 二叉树只有 lc/rc 两个孩子；图要用循环扫描这一行，寻找所有邻接点。 */
void dfs(const Graph& graph, int v, bool visited[]) {
    if (visited[v]) {
        return;
    }
    visited[v]=true;
    std::cout<<graph.vertices[v]<<' ';
    for (int w=0;w<graph.numVertices;w++) {
        if (graph.edge[v][w]) {
            if (!visited[w]) {
                dfs(graph,w,visited);
            }
        }
    }/* TODO 3：已访问就直接返回；否则标记并输出 graph.vertices[v]。
     * 按 w = 0～numVertices-1 扫描当前行，遇到存在边且未访问的 w 时递归。
     * 每个字符后输出一个空格，不输出标签或换行，不重置 visited。
     *
     * 参数：
     *   graph    只读的图。const Graph& 表示不复制整张图，也不允许修改它。
     *   v        当前顶点编号。第一次调用时是起点，递归调用时是某个邻接点。
     *   visited  访问标记数组。bool visited[] 传入的是首地址，
     *            函数内写 visited[v] 改的就是调用者的那个数组；
     *            长度至少为 graph.numVertices，调用者负责初始化，函数内不清空。
     */
}

/* 一次搜索不一定覆盖全图：用外层循环补上尚未访问的部分。 */
void dfsAll(const Graph& graph) {
    bool visited[MAXV] = {};
    for (int v=0;v<graph.numVertices;v++) {
        if (!visited[v]) {
            dfs(graph,v,visited);
        }
    }/* TODO 4：创建 bool visited[MAXV] = {}，只初始化这一次。
     * 按编号递增检查所有顶点，对未访问的顶点调用 dfs。
     *
     * 参数：
     *   graph  只读的图。visited 由本函数自己创建，不需要调用者传入。
     */
}

/* 广度优先：沿用 Lab11 的层次遍历，先入队，再循环出队、访问、扩展。
 * 树的队列保存 Node*；本次队列保存顶点编号 int。 */
void bfs(const Graph& graph, int v, bool visited[]) {
    if (visited[v])
        return;
    std::queue<int> q;
    visited[v]=true;
    q.push(v);
    while (!q.empty()) {
        int u=q.front();
        q.pop();
        std::cout<<graph.vertices[u]<<" ";
        for (int w=0;w<graph.numVertices;w++) {
            if (graph.edge[u][w] && !visited[w]) {
                visited[w]=true;
                q.push(w);
            }
        }
    }/* TODO 5：已访问就直接返回；否则创建 std::queue<int>，
     * 先把起点标记为已访问，再入队。
     * 每轮取队首编号、出队、输出该顶点，然后按编号递增扫描它的邻接点。
     * 存在边且未访问的邻接点，先标记再入队，避免重复入队。
     * 只使用 push、front、pop、empty；不重置 visited，不输出标签或换行。
     *
     * 参数：
     *   graph    只读的图。const Graph& 表示不复制、不修改。
     *   v        起点编号（下标，不是字符）。
     *   visited  访问标记数组，与 dfs 共用同一套规则：
     *            传首地址、函数内直接修改、长度至少 numVertices、不在函数内清空。
     */
}

void bfsAll(const Graph& graph) {
    bool visited[MAXV] = {};
    for (int v=0;v<graph.numVertices;v++) {
        if (!visited[v]) {
            bfs(graph,v,visited);
        }
    }/* TODO 6：创建 bool visited[MAXV] = {}，只初始化这一次。
     * 按编号递增检查所有顶点，对未访问的顶点调用 bfs。
     *
     * 参数：
     *   graph  只读的图。visited 由本函数自己创建，不需要调用者传入。
     */
}
