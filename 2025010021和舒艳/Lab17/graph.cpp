#include <cassert>
#include <iostream>

#include "graph.h"

/* 教师提供：先准备顶点，再把所有邻接关系置为“无边”。
 * 和 Lab11 中“创建节点、赋 data、再接 lc/rc”的建树过程对应。
 *
 * 参数：
 *   graph     要初始化的图。Graph& 是引用：函数内改的就是调用者的原图，
 *             不会复制一份 20×20 的矩阵。
 *   labels    顶点标签数组，例如 "ABCDEF"；只读，函数只从中取字符。
 *   count     实际顶点数，范围 0～MAXV；为 0 时允许 labels 是 nullptr。
 *   directed  true 表示有向图，false 表示无向图；默认值为 false。
 */
void initGraph(Graph& graph, const char* labels, int count, bool directed) {
    assert(count >= 0 && count <= MAXV);
    assert(count == 0 || labels != nullptr);

    graph.numVertices = count;
    graph.numEdges = 0;
    graph.directed = directed;
    for (int i = 0; i < MAXV; ++i) {
        graph.vertices[i] = i < count ? labels[i] : '\0';
        for (int j = 0; j < MAXV; ++j) {
            graph.edge[i][j] = 0;
        }
    }
}

bool insertEdge(Graph& graph, int from, int to) {
    /* TODO 1：先检查两个编号是否合法、是否为自环、是否已有这条边。
     * 检查失败直接返回 false，不修改任何成员。
     * 其余情况把 edge[from][to] 置为 1；无向图还要把反向位置置为 1。
     * 最后只把 numEdges 增加 1，返回 true。
     *
     * 参数：
     *   graph  要修改的图。Graph& 让函数直接改调用者的原图。
     *   from   起点编号，是数组下标（例如 A 对应 0），不是字符 'A'。
     *   to     终点编号，含义同 from。
     * 返回：
     *   成功修改返回 true；编号非法、自环、边已存在返回 false，且图保持不变。
     */
    if (from < 0 || from >= graph.numVertices || to < 0 || to >= graph.numVertices) {
        return false;
    }

    if (from == to) {
        return false;
    }

    if (graph.edge[from][to] == 1) {
        return false;
    }


    graph.edge[from][to] = 1;
    if (!graph.directed) {
        graph.edge[to][from] = 1;
    }
    graph.numEdges++;
    return true;
}

bool removeEdge(Graph& graph, int from, int to) {
    /* TODO 2：先检查两个编号是否合法、是否为自环、是否存在这条边。
     * 检查失败直接返回 false，不修改任何成员。
     * 其余情况把 edge[from][to] 置为 0；无向图还要把反向位置置为 0。
     * 最后只把 numEdges 减少 1，返回 true。
     *
     * 参数：
     *   graph  要修改的图。Graph& 让函数直接改调用者的原图。
     *   from   起点编号（下标，不是字符）。
     *   to     终点编号（下标，不是字符）。
     * 返回：
     *   成功修改返回 true；编号非法、自环、边不存在返回 false，且图保持不变。
     */
    if (from < 0 || from >= graph.numVertices || to < 0 || to >= graph.numVertices) {
        return false;
    }

    if (from == to) {
        return false;
    }

    if (graph.edge[from][to] == 0) {
        return false;
    }


    graph.edge[from][to] = 0;
    if (!graph.directed) {
        graph.edge[to][from] = 0;
    }
    graph.numEdges--;
    return true;
}

/* 教师提供：只打印有效的 numVertices 行、列，不打印备用空间。
 * 参数：
 *   graph  只读的图。const Graph& 表示不复制整张图，也不允许在函数内修改它。
 */
void printMatrix(const Graph& graph) {
    std::cout << "  ";
    for (int j = 0; j < graph.numVertices; ++j) {
        std::cout << graph.vertices[j] << ' ';
    }
    std::cout << '\n';
    for (int i = 0; i < graph.numVertices; ++i) {
        std::cout << graph.vertices[i] << ' ';
        for (int j = 0; j < graph.numVertices; ++j) {
            std::cout << graph.edge[i][j] << ' ';
        }
        std::cout << '\n';
    }
}
