#include <cassert>
#include <iostream>

#include "graph.h"

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
    if (from < 0 || from >= graph.numVertices) return false;
    if (to < 0 || to >= graph.numVertices) return false;
    if (from == to) return false;
    if (graph.edge[from][to] == 1) return false;

    graph.edge[from][to] = 1;
    if (!graph.directed) {
        graph.edge[to][from] = 1;
    }
    graph.numEdges++;
    return true;
}

bool removeEdge(Graph& graph, int from, int to) {
    if (from < 0 || from >= graph.numVertices) return false;
    if (to < 0 || to >= graph.numVertices) return false;
    if (from == to) return false;
    if (graph.edge[from][to] == 0) return false;

    graph.edge[from][to] = 0;
    if (!graph.directed) {
        graph.edge[to][from] = 0;
    }
    graph.numEdges--;
    return true;
}

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
