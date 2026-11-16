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
    /* 依次检查：编号合法（含下界，防止 -1 越界）、非自环、边不存在。 */
    if (from < 0 || from >= graph.numVertices ||
        to < 0 || to >= graph.numVertices) {
        return false;               /* 编号非法，图不变 */
    }
    if (from == to) {
        return false;               /* 自环不允许，edge[i][i] 恒为 0 */
    }
    if (graph.edge[from][to] == 1) {
        return false;               /* 边已存在，不重复插入 */
    }

    graph.edge[from][to] = 1;       /* 写入正向位置 */
    if (!graph.directed) {
        graph.edge[to][from] = 1;   /* 无向图同步对称位置 */
    }
    ++graph.numEdges;               /* 一条无向边也只计一次 */
    return true;
}

bool removeEdge(Graph& graph, int from, int to) {
    /* 与插入对称：编号合法（含下界）、非自环、边确实存在才动手。 */
    if (from < 0 || from >= graph.numVertices ||
        to < 0 || to >= graph.numVertices) {
        return false;               /* 编号非法，图不变 */
    }
    if (from == to) {
        return false;               /* 自环位置不可能有边 */
    }
    if (graph.edge[from][to] == 0) {
        return false;               /* 边不存在，numEdges 不得减少 */
    }

    graph.edge[from][to] = 0;       /* 清除正向位置 */
    if (!graph.directed) {
        graph.edge[to][from] = 0;   /* 无向图同步对称位置，删除要彻底 */
    }
    --graph.numEdges;
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
