#ifndef GRAPH_H
#define GRAPH_H

/* 教师提供：和 Lab11 的 BinNode 一样，先用简单结构体保存数据，
 * 操作写成普通函数。本次不使用模板、继承或抽象图类。 */
const int MAXV = 20;

struct Graph {
    int numVertices;          // 实际顶点数；有效编号为 0～numVertices-1
    int numEdges;             // 实际边数；一条无向边只计一次
    char vertices[MAXV];      // 顶点的字符标签；编号和标签是两回事
    int edge[MAXV][MAXV];     // 0 表示无边，1 表示有边
    bool directed;            // true：有向图；false：无向图
};

/* 教师提供：labels 至少含 count 个字符，0 <= count <= MAXV。
 * count == 0 时允许 labels 为 nullptr。顶点标签由驱动保证互不相同。
 * 初始化后有指定的顶点，但没有边；对角线也全部为 0。 */
void initGraph(Graph& graph, const char* labels, int count, bool directed = false);
void printMatrix(const Graph& graph);

/* 学生完成：成功修改返回 true；非法编号、自环、重复插入或删除不存在的边
 * 返回 false，且整张图保持不变。无向边须同步处理两个对称位置。 */
bool insertEdge(Graph& graph, int from, int to);   // graph.cpp：TODO 1
bool removeEdge(Graph& graph, int from, int to);   // graph.cpp：TODO 2

/* 学生完成：仿照 Lab11 的 preorder / levelOrder，访问即输出。
 * v 由调用者保证为合法编号；visited 至少有 numVertices 个元素。
 * 已标记的顶点不再访问；数组按引用所指向的原数组更新，不在函数内重置。
 * 邻接点统一按编号递增扫描。有向图只沿出边前进。 */
void dfs(const Graph& graph, int v, bool visited[]);   // traversal.cpp：TODO 3
void bfs(const Graph& graph, int v, bool visited[]);   // traversal.cpp：TODO 5

/* 学生完成：初始化一次 visited，按编号递增检查所有顶点，
 * 对尚未访问的顶点调用上面的单次搜索，保证全图每个顶点恰好输出一次。
 * 空图不输出；标签和换行由 main 负责。 */
void dfsAll(const Graph& graph);   // traversal.cpp：TODO 4
void bfsAll(const Graph& graph);   // traversal.cpp：TODO 6

#endif
