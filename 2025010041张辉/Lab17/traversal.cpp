#include <iostream>
#include <queue>

#include "graph.h"

void dfs(const Graph& graph, int v, bool visited[]) {
    if (visited[v]) return;
    visited[v] = true;
    std::cout << graph.vertices[v] << " ";
    for (int w = 0; w < graph.numVertices; ++w) {
        if (graph.edge[v][w] == 1 && !visited[w]) {
            dfs(graph, w, visited);
        }
    }
}

void dfsAll(const Graph& graph) {
    bool visited[MAXV] = {};
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            dfs(graph, v, visited);
        }
    }
}

void bfs(const Graph& graph, int v, bool visited[]) {
    if (visited[v]) return;
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
    bool visited[MAXV] = {};
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            bfs(graph, v, visited);
        }
    }
}
