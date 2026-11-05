#include <iostream>
#include <sstream>
#include <string>

#include "graph.h"

/* 教师提供：演示与 18 组统一测试。本文件不修改、不提交。
 * captureAll / captureFrom 只是临时收集 cout 的输出以便核对，
 * 学生仍像 Lab11 一样直接 cout，无需学习或改写输出收集代码。 */
std::string captureAll(const Graph& graph, bool breadth) {
    std::ostringstream out;
    std::streambuf* saved = std::cout.rdbuf(out.rdbuf());
    if (breadth) {
        bfsAll(graph);
    } else {
        dfsAll(graph);
    }
    std::cout.rdbuf(saved);
    return out.str();
}

std::string captureFrom(const Graph& graph, int start, bool visited[], bool breadth) {
    std::ostringstream out;
    std::streambuf* saved = std::cout.rdbuf(out.rdbuf());
    if (breadth) {
        bfs(graph, start, visited);
    } else {
        dfs(graph, start, visited);
    }
    std::cout.rdbuf(saved);
    return out.str();
}

bool sameGraph(const Graph& left, const Graph& right) {
    if (left.numVertices != right.numVertices || left.numEdges != right.numEdges
        || left.directed != right.directed) {
        return false;
    }
    for (int i = 0; i < MAXV; ++i) {
        if (left.vertices[i] != right.vertices[i]) {
            return false;
        }
        for (int j = 0; j < MAXV; ++j) {
            if (left.edge[i][j] != right.edge[i][j]) {
                return false;
            }
        }
    }
    return true;
}

/* 教师提供：测试夹具直接准备边，使遍历测试不依赖 insertEdge 是否正确。
 * 演示程序则使用学生的 insertEdge 建图。 */
void fixtureEdge(Graph& graph, int from, int to) {
    graph.edge[from][to] = 1;
    if (!graph.directed) {
        graph.edge[to][from] = 1;
    }
    ++graph.numEdges;
}

void exampleFixture(Graph& graph) {
    initGraph(graph, "ABCDEF", 6);
    fixtureEdge(graph, 0, 1);
    fixtureEdge(graph, 0, 2);
    fixtureEdge(graph, 1, 2);
    fixtureEdge(graph, 1, 3);
    fixtureEdge(graph, 2, 4);
}

bool check(const char* name, bool ok) {
    std::cout << name << ": " << (ok ? "PASS" : "FAIL") << '\n';
    return ok;
}

int runTests() {
    int passed = 0;
    Graph graph;

    // 1：空图。外层循环不应访问任何数组元素或调用单次搜索。
    initGraph(graph, nullptr, 0);
    passed += check("empty graph", captureAll(graph, false).empty()
                    && captureAll(graph, true).empty());

    // 2：只有一个孤立顶点。
    initGraph(graph, "X", 1);
    passed += check("single vertex", captureAll(graph, false) == "X "
                    && captureAll(graph, true) == "X ");

    // 3：无向边的两个位置都置 1，边数只增加一次。
    initGraph(graph, "ABC", 3);
    bool inserted = insertEdge(graph, 0, 1);
    passed += check("insert undirected", inserted && graph.numEdges == 1
                    && graph.edge[0][1] == 1 && graph.edge[1][0] == 1);

    // 4：正向、反向重复插入同一条无向边，整张图不变。
    Graph before = graph;
    bool duplicateForward = !insertEdge(graph, 0, 1);
    bool duplicateReverse = !insertEdge(graph, 1, 0);
    passed += check("reject duplicate", duplicateForward && duplicateReverse
                    && sameGraph(graph, before));

    // 5：负数、实际范围以外、备用位置、自环，均不得修改图。
    bool invalidInsert = !insertEdge(graph, -1, 1);
    invalidInsert = !insertEdge(graph, 0, 3) && invalidInsert;
    invalidInsert = !insertEdge(graph, MAXV, 0) && invalidInsert;
    invalidInsert = !insertEdge(graph, 2, 2) && invalidInsert;
    passed += check("reject invalid insert", invalidInsert && sameGraph(graph, before));

    // 6：可从反向删除无向边，两个位置清零，边数只减少一次。
    bool removed = removeEdge(graph, 1, 0);
    passed += check("remove undirected", removed && graph.numEdges == 0
                    && graph.edge[0][1] == 0 && graph.edge[1][0] == 0);

    // 7：删除不存在的边、非法编号、自环，整张图不变。
    before = graph;
    bool invalidRemove = !removeEdge(graph, 0, 1);
    invalidRemove = !removeEdge(graph, -1, 0) && invalidRemove;
    invalidRemove = !removeEdge(graph, 0, 3) && invalidRemove;
    invalidRemove = !removeEdge(graph, 0, MAXV) && invalidRemove;
    invalidRemove = !removeEdge(graph, 1, 1) && invalidRemove;
    passed += check("reject invalid remove", invalidRemove && sameGraph(graph, before));

    // 8：有向边的两个方向独立计数、独立删除。
    initGraph(graph, "ABC", 3, true);
    bool directedOk = insertEdge(graph, 0, 1);
    directedOk = directedOk && graph.numEdges == 1 && graph.edge[1][0] == 0;
    bool reverseInserted = insertEdge(graph, 1, 0);
    directedOk = directedOk && reverseInserted && graph.numEdges == 2;
    bool forwardRemoved = removeEdge(graph, 0, 1);
    directedOk = directedOk && forwardRemoved && graph.numEdges == 1
                 && graph.edge[0][1] == 0 && graph.edge[1][0] == 1;
    passed += check("directed edge operations", directedOk);

    // 9、10：三角形加两个分支和一个孤立顶点。
    exampleFixture(graph);
    passed += check("DFS cycle and isolated", captureAll(graph, false) == "A B C E D F ");
    passed += check("BFS cycle and isolated", captureAll(graph, true) == "A B C D E F ");

    // 11：从 A 单次搜索访问不到 F；visited 应保留搜索结果。
    bool visitedDFS[MAXV] = {};
    bool visitedBFS[MAXV] = {};
    bool fromOk = captureFrom(graph, 0, visitedDFS, false) == "A B C E D "
                  && captureFrom(graph, 0, visitedBFS, true) == "A B C D E ";
    for (int i = 0; i < 6; ++i) {
        fromOk = fromOk && visitedDFS[i] == (i < 5) && visitedBFS[i] == (i < 5);
    }
    passed += check("single search reachability", fromOk);

    // 12：起点已经访问时直接返回；预先标记的邻接点不能再输出或扩展。
    bool markedDFS[MAXV] = {};
    bool markedBFS[MAXV] = {};
    markedDFS[1] = true;
    markedBFS[1] = true;
    bool markedOk = captureFrom(graph, 0, markedDFS, false) == "A C E "
                    && captureFrom(graph, 0, markedBFS, true) == "A C E ";
    markedOk = markedOk && !markedDFS[3] && !markedBFS[3]
               && captureFrom(graph, 0, markedDFS, false).empty()
               && captureFrom(graph, 0, markedBFS, true).empty();
    passed += check("respect existing visited", markedOk);

    // 13：把 Lab11 指定的二叉树当作无向图，顺序应与先序、层次遍历对应。
    initGraph(graph, "ABCDEF", 6);
    fixtureEdge(graph, 0, 1);
    fixtureEdge(graph, 0, 2);
    fixtureEdge(graph, 1, 3);
    fixtureEdge(graph, 1, 4);
    fixtureEdge(graph, 2, 5);
    passed += check("Lab11 tree as graph", captureAll(graph, false) == "A B D E C F "
                    && captureAll(graph, true) == "A B C D E F ");

    // 14：两个三角形加一个孤立顶点，不能在外层每轮清空 visited。
    initGraph(graph, "ABCDEFG", 7);
    fixtureEdge(graph, 0, 1);
    fixtureEdge(graph, 1, 2);
    fixtureEdge(graph, 0, 2);
    fixtureEdge(graph, 3, 4);
    fixtureEdge(graph, 4, 5);
    fixtureEdge(graph, 3, 5);
    passed += check("multiple components", captureAll(graph, false) == "A B C D E F G "
                    && captureAll(graph, true) == "A B C D E F G ");

    // 15：有向图只沿出边，不能把矩阵的列也当作当前顶点的邻接点。
    initGraph(graph, "ABCD", 4, true);
    fixtureEdge(graph, 1, 0);
    fixtureEdge(graph, 1, 2);
    fixtureEdge(graph, 2, 3);
    fixtureEdge(graph, 3, 1);
    bool directedDFS[MAXV] = {};
    bool directedBFS[MAXV] = {};
    bool directionOk = captureFrom(graph, 0, directedDFS, false) == "A "
                       && captureFrom(graph, 0, directedBFS, true) == "A ";
    directionOk = directionOk && !directedDFS[1] && !directedBFS[1]
                  && captureAll(graph, false) == "A B C D "
                  && captureAll(graph, true) == "A B C D ";
    passed += check("follow directed edges", directionOk);

    // 16：完全图有很多重复到达机会，每个顶点仍只能输出一次。
    initGraph(graph, "ABCDE", 5);
    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            fixtureEdge(graph, i, j);
        }
    }
    passed += check("dense graph", captureAll(graph, false) == "A B C D E "
                    && captureAll(graph, true) == "A B C D E ");

    // 17：增删边应影响实际搜索；反复全图遍历结果一致，原图保持不变。
    exampleFixture(graph);
    bool mutationOk = removeEdge(graph, 1, 2);
    mutationOk = mutationOk && graph.numEdges == 4
                 && captureAll(graph, false) == "A B D C E F ";
    bool restored = insertEdge(graph, 1, 2);
    bool linked = insertEdge(graph, 4, 5);
    mutationOk = mutationOk && restored && linked && graph.numEdges == 6;
    before = graph;
    for (int repeat = 0; repeat < 2; ++repeat) {
        mutationOk = mutationOk && captureAll(graph, false) == "A B C E F D "
                     && captureAll(graph, true) == "A B C D E F ";
    }
    passed += check("mutate and repeat", mutationOk && sameGraph(graph, before));

    // 18：全部 MAXV 个位置都有效，检查遍历没有漏掉最后一个顶点。
    std::string labels;
    std::string expected;
    for (int i = 0; i < MAXV; ++i) {
        char label = static_cast<char>('A' + i);
        labels.push_back(label);
        expected.push_back(label);
        expected.push_back(' ');
    }
    initGraph(graph, labels.c_str(), MAXV);
    passed += check("capacity boundary", captureAll(graph, false) == expected
                    && captureAll(graph, true) == expected);

    std::cout << "graph tests: " << passed << "/18 passed\n";
    return passed == 18 ? 0 : 1;
}

void runExample() {
    Graph graph;
    initGraph(graph, "ABCDEF", 6);
    insertEdge(graph, 0, 1);   // A—B
    insertEdge(graph, 0, 2);   // A—C
    insertEdge(graph, 1, 2);   // B—C
    insertEdge(graph, 1, 3);   // B—D
    insertEdge(graph, 2, 4);   // C—E；F 为孤立顶点

    std::cout << "邻接矩阵：\n";
    printMatrix(graph);
    std::cout << "顶点数：" << graph.numVertices << "，边数：" << graph.numEdges << '\n';

    bool visitedDFS[MAXV] = {};
    std::cout << "从 A 出发的 DFS：";
    dfs(graph, 0, visitedDFS);
    std::cout << '\n';

    bool visitedBFS[MAXV] = {};
    std::cout << "从 A 出发的 BFS：";
    bfs(graph, 0, visitedBFS);
    std::cout << '\n';

    std::cout << "全图 DFS：";
    dfsAll(graph);
    std::cout << '\n';
    std::cout << "全图 BFS：";
    bfsAll(graph);
    std::cout << '\n';
}

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "--test") {
        return runTests();
    }
    runExample();
    std::cout << '\n';
    return runTests();
}
