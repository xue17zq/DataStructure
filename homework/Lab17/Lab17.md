# Lab17：邻接矩阵建图与深度、广度优先遍历

> **范围**：固定容量、字符标签、非带权简单图，支持有向图与无向图；图仅使用邻接矩阵存储。
> **提交**：`graph.cpp`、`traversal.cpp`、`Lab17.md`。
> **截止**：2026 年 11 月 12 日 24:00（北京时间 11 月 13 日 00:00）。

---

## 1. 实验范围与文件组织

### 1.1 内容

实验包含三部分：

1. 邻接矩阵的初始化与输出；
2. 边的查找、插入、删除；
3. 深度优先搜索、广度优先搜索及全图遍历。

需要补全的函数共六个：`insertEdge`、`removeEdge`、`dfs`、`dfsAll`、`bfs`、`bfsAll`。

### 1.2 文件

将 `homework/Lab17/` 中的六个文件复制到 `学号姓名/Lab17/`：

| 文件 | 内容 | 提交 |
| :--- | :--- | :---: |
| `graph.h` | 图结构、`MAXV`、函数声明 | 否 |
| `graph.cpp` | 初始化、矩阵输出、边的插入与删除 | 是 |
| `traversal.cpp` | DFS、BFS 与全图遍历 | 是 |
| `main.cpp` | 演示程序与 18 组测试 | 否 |
| `CMakeLists.txt` | 构建与 CTest 配置 | 否 |
| `Lab17.md` | 实验文档与记录 | 是 |

### 1.3 构建与运行

CLion 中打开 `Lab17/CMakeLists.txt`，选择 **Open as Project**，Build type 设为 **Debug**，运行目标为 `lab17_graph`。

命令行：

```sh
cmake -S . -B build
cmake --build build
./build/lab17_graph          # 演示 + 测试
./build/lab17_graph --test   # 仅测试
ctest --test-dir build --output-on-failure
```

`insertEdge`、`removeEdge` 返回 `bool`，函数体为空时编译器会产生 “non-void function does not return a value” 警告，运行结果不可预期。边操作补全前不运行测试。

### 1.4 参数传递约定

函数参数分为四类：

| 写法 | 语义 | 对被调函数可见性 |
| :--- | :--- | :--- |
| `Graph& graph` | 引用，不复制对象 | 可读写调用者的图 |
| `const Graph& graph` | 常引用，不复制对象 | 只读，编译期禁止修改 |
| `bool visited[]` | 数组参数，退化为 `bool*` | 可读写调用者的数组 |
| `int from` / `int v` | 值传递 | 只读副本，修改不影响调用者 |
| `const char* labels` | 指向只读字符序列的指针 | 只能读取字符 |

`Graph` 内含 `MAXV×MAXV` 的矩阵，按值传递会复制整个矩阵。因此修改图的函数使用 `Graph&`，只读函数使用 `const Graph&`。

`bool visited[]` 传递的是首地址，函数内执行 `visited[v] = true` 会直接修改调用者的数组。`dfs`、`bfs` 依赖这一语义共享访问记录。

参数中的 `int` 是顶点编号，即数组下标；`char` 是顶点标签。编号 `0` 对应标签 `graph.vertices[0]`，两者不能混用。

---

## 2. 邻接矩阵

### 2.1 数据结构

`graph.h` 中的定义：

```cpp
const int MAXV = 20;

struct Graph {
    int numVertices;          // 实际顶点数；有效编号 0～numVertices-1
    int numEdges;             // 实际边数；一条无向边只计一次
    char vertices[MAXV];      // 顶点标签，例如 vertices[0] == 'A'
    int edge[MAXV][MAXV];     // 邻接矩阵：1 有边，0 无边
    bool directed;            // true 有向图，false 无向图
};
```

| 成员 | 含义 | 初始化来源 |
| :--- | :--- | :--- |
| `numVertices` | 实际顶点数 | `initGraph` 的 `count` |
| `numEdges` | 实际边数 | `0` |
| `vertices[i]` | 编号 `i` 的标签 | 驱动提供的标签 |
| `edge[i][j]` | `i` 到 `j` 是否有边 | `0` |
| `directed` | 有向 / 无向 | 驱动指定 |

两个约束：

- 编号是下标，标签是数据。`dfs(graph, 0, visited)` 中的 `0` 是编号，输出时使用 `graph.vertices[0]`。
- 只有左上角 `n×n` 区域有效。`MAXV == 20` 时，六顶点图的合法编号为 `0～5`，第 `6～19` 行、列为备用位置，不参与插边或遍历。

### 2.2 边的表示

| 情况 | 矩阵操作 | `numEdges` 变化 |
| :--- | :--- | :--- |
| 有向边 `A→B` | `edge[0][1] = 1` | `+1` |
| 无向边 `A—B` | `edge[0][1] = edge[1][0] = 1` | `+1` |
| 自环 | 本次不允许，`edge[i][i]` 恒为 `0` | 无 |
| 删除边 | 对应位置改回 `0`，无向图同步对称位置 | `-1` |

顶点 `v` 的出边位于第 `v` 行：

```cpp
for (int w = 0; w < graph.numVertices; ++w) {
    if (graph.edge[v][w] == 1) {
        // w 是 v 的一个邻接点
    }
}
```

有向图中，第 `v` 列表示指向 `v` 的入边，遍历时只扫描行，不扫描列。

### 2.3 初始化

教师提供的 `initGraph`：

```cpp
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
```

参数：

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `Graph&` | 待初始化的图；函数直接修改调用者的对象 |
| `labels` | `const char*` | 顶点标签数组，如 `"ABCDEF"`；只读 |
| `count` | `int` | 实际顶点数，范围 `0～MAXV`；为 `0` 时允许 `labels == nullptr` |
| `directed` | `bool` | `true` 有向图，`false` 无向图；默认 `false` |

实现要点：

1. 先写入 `numVertices`、`numEdges`、`directed`，再将整个 `MAXV×MAXV` 矩阵清零。
2. 标签只写入有效区：`i < count` 时复制 `labels[i]`，否则写 `'\0'`。
3. `count == 0` 时允许 `labels == nullptr`，条件表达式不会读取空指针。

`initGraph` 只建立空图，不创建任何边。

### 2.4 矩阵输出

```cpp
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
```

参数：

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `const Graph&` | 只读图；不复制矩阵，不允许修改 |

两层循环的上界均为 `numVertices`，不是 `MAXV`。备用行列不输出。行末空格不影响测试结果。

---

## 3. 边操作

### 3.1 查找边

给定编号 `u`、`v`，判断边是否存在只需读取一个矩阵元素：

```cpp
bool hasEdge(const Graph& graph, int u, int v) {
    return graph.edge[u][v] == 1;
}
```

- 矩阵为定长二维数组，`edge[u][v]` 的地址为 `基址 + (u * MAXV + v) * sizeof(int)`，一次寻址完成访问，时间复杂度 **O(1)**。
- 无向图的插入与删除保持对称，因此 `edge[u][v] == 1` 等价于 `edge[v][u] == 1`，无需检查两个位置。
- 前提是 `u`、`v` 为合法编号。若输入是字符标签，需要先扫描 `vertices` 获取编号，时间复杂度 O(n)。

本次不单独实现 `hasEdge`；`insertEdge` 与 `removeEdge` 的合法性判断包含该检查。

### 3.2 插入边

```cpp
bool insertEdge(Graph& graph, int from, int to);
```

参数与返回值：

| 参数/返回值 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `Graph&` | 待修改的图；函数直接修改调用者的对象 |
| `from` | `int` | 起点编号（下标），例如 `A` 对应 `0` |
| `to` | `int` | 终点编号（下标） |
| 返回值 | `bool` | 成功返回 `true`；编号非法、自环、边已存在返回 `false`，图不变 |

执行顺序：

| 步骤 | 检查/操作 | 失败处理 |
| :---: | :--- | :--- |
| 1 | `from`、`to` 位于 `0～numVertices-1` | 返回 `false`，不修改 |
| 2 | `from != to` | 返回 `false`，不修改 |
| 3 | `edge[from][to] == 0` | 返回 `false`，不修改 |
| 4 | `edge[from][to] = 1` | — |
| 5 | 无向图：`edge[to][from] = 1` | — |
| 6 | `++numEdges`，返回 `true` | — |

约束：

- 合法性检查包含下界。仅判断 `from >= numVertices` 时，`-1` 会通过检查并导致 `edge[-1][1]` 越界。
- 无向图的两个矩阵位置对应同一条边，`numEdges` 只增加一次。
- 不能用矩阵中 `1` 的总数推算边数：无向图存在对称位置，有向图存在方向。

### 3.3 删除边

```cpp
bool removeEdge(Graph& graph, int from, int to);
```

参数与返回值：

| 参数/返回值 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `Graph&` | 待修改的图；函数直接修改调用者的对象 |
| `from` | `int` | 起点编号（下标） |
| `to` | `int` | 终点编号（下标） |
| 返回值 | `bool` | 成功返回 `true`；编号非法、自环、边不存在返回 `false`，图不变 |

执行顺序与插入对称：

| 步骤 | 检查/操作 | 失败处理 |
| :---: | :--- | :--- |
| 1 | 两个编号合法 | 返回 `false`，不修改 |
| 2 | `from != to` | 返回 `false`，不修改 |
| 3 | `edge[from][to] == 1` | 返回 `false`，不修改 |
| 4 | `edge[from][to] = 0` | — |
| 5 | 无向图：`edge[to][from] = 0` | — |
| 6 | `--numEdges`，返回 `true` | — |

删除不存在的边、非法编号、自环均返回 `false`，`numEdges` 不得减为负数。

### 3.4 时间复杂度

| 操作 | 时间 | 原因 |
| :--- | :---: | :--- |
| 查找边 `edge[u][v]` | **O(1)** | 一次下标寻址 |
| 插入一条边 | **O(1)** | 常数次判断与一个或两个位置的写入 |
| 删除一条边 | **O(1)** | 同插入 |

顶点由 `initGraph` 一次性建立，本次不增删顶点。增删顶点需要移动 `vertices` 并重建矩阵，时间复杂度 O(n²)，不在本次范围。

### 3.5 常见缺陷

| 缺陷 | 后果 |
| :--- | :--- |
| 只检查编号上界 | 负数编号越界读写 |
| 无向图漏写反向位置 | 矩阵不对称，删除不彻底 |
| 无向边将 `numEdges` 加减两次 | 边数统计错误 |
| 先修改矩阵再检查 | 拒绝操作时图已被破坏 |

---

## 4. 深度优先搜索

### 4.1 与先序遍历的对应

Lab11 的先序遍历：

```cpp
void preorder(Node* root) {
    if (root == nullptr) return;
    std::cout << root->data << ' ';
    preorder(root->lc);
    preorder(root->rc);
}
```

对应关系：

| 二叉树 | 图 |
| :--- | :--- |
| 空指针返回 | 顶点已访问则返回 |
| 输出 `root->data` | 标记 `v`，输出 `graph.vertices[v]` |
| 递归 `lc`、`rc` | 扫描第 `v` 行，对未访问邻接点递归 |
| 孩子指针无回路 | 图存在回路，依赖 `visited` 防止重复访问 |

### 4.2 `visited` 数组

树中每个节点只有一个父节点，沿孩子指针不会回到已访问节点。图中存在环和交叉边，同一顶点可能从多条路径到达。缺少 `visited` 时，递归会在环上反复进入，最终栈溢出。

`visited` 由调用者创建并传入，所有递归共享同一数组，函数内不清空：

```cpp
bool visited[MAXV] = {};
dfs(graph, 0, visited);
```

### 4.3 单次 DFS

```cpp
void dfs(const Graph& graph, int v, bool visited[]);
```

参数：

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `const Graph&` | 只读图；不复制、不修改 |
| `v` | `int` | 当前顶点编号；首次调用为起点，递归时为邻接点 |
| `visited` | `bool[]` | 访问标记数组；传首地址，函数内修改调用者的数组；长度至少 `numVertices`，由调用者初始化 |

步骤：

1. `visited[v]` 为 `true` 时返回；
2. `visited[v] = true`；
3. 输出 `graph.vertices[v]` 和一个空格；
4. 按 `w = 0～numVertices-1` 扫描第 `v` 行；
5. `edge[v][w] == 1` 且 `!visited[w]` 时递归 `dfs(graph, w, visited)`。

代码：

```cpp
if (visited[v]) return;

visited[v] = true;
std::cout << graph.vertices[v] << " ";

for (int w = 0; w < graph.numVertices; ++w) {
    if (graph.edge[v][w] == 1 && !visited[w]) {
        dfs(graph, w, visited);
    }
}
```

循环上界为 `numVertices`。写成 `numVertices - 1` 时，编号最大的顶点不会被扫描。

### 4.4 全图 DFS

单次 DFS 只访问起点可达的顶点。`dfsAll` 通过外层循环处理其余连通分量：

```cpp
void dfsAll(const Graph& graph) {
    bool visited[MAXV] = {};
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            dfs(graph, v, visited);
        }
    }
}
```

参数：

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `const Graph&` | 只读图；`visited` 由函数内部创建 |

两个约束：

- `bool visited[MAXV] = {};` 初始化 `MAXV` 个 `false`。`bool visited[] = { false };` 只创建一个元素，后续下标越界。
- 外层调用使用 `dfs(graph, v, visited)`。写死为 `dfs(graph, 0, visited)` 时，首个连通分量结束后其余分量无法访问。

### 4.5 示例追踪

对 6.1 节的图从 `A` 出发，按编号递增扫描：

| 步骤 | 顶点 | 进入/返回 | 过程 |
| :---: | :---: | :--- | :--- |
| 1 | A | 进入 | 标记 A，输出 A，扫描到 B |
| 2 | B | 进入 | 标记 B，输出 B，扫描到 C |
| 3 | C | 进入 | 标记 C，输出 C；A、B 已访问，扫描到 E |
| 4 | E | 进入 | 标记 E，输出 E；C 已访问，无新邻接点 |
| 5 | E | 返回 | 返回 C |
| 6 | C | 返回 | C 扫描结束，返回 B |
| 7 | D | 进入 | B 继续扫描到 D，标记 D，输出 D |
| 8 | D | 返回 | D 的邻接点 B 已访问，返回 B |
| 9 | B | 返回 | B 扫描结束，返回 A |
| 10 | A | 返回 | A 扫描结束，单次 DFS 结束 |

输出：

```text
A B C E D
```

`F` 是孤立点，从 `A` 不可达，由 `dfsAll` 的外层循环补充。全图 DFS 输出 `A B C E D F`。

### 4.6 复杂度

设实际顶点数为 `n`。

- 每个被访问顶点扫描一整行，共 `n` 个位置；
- 最多访问 `n` 个顶点；
- 单次 DFS 的时间复杂度为 **O(n²)**。若起点可达 `k` 个顶点，则为 **Θ(k·n)**；连通图下为 **Θ(n²)**；
- `dfsAll` 中每个顶点访问一次、每行扫描一次，复杂度 **O(n²)**；
- 递归栈最坏 **O(n)**，`visited` 数组 **O(n)**。

矩阵存储下，每个顶点都需要扫描 `n` 个位置，边的数量不影响扫描次数，因此不能写成 O(n+e)。

---

## 5. 广度优先搜索

### 5.1 与层次遍历的对应

Lab11 的层次遍历使用 `std::queue<Node*>` 保存待访问节点。图的 BFS 将“左右孩子”替换为“当前行的所有邻接点”，队列元素为顶点编号 `int`：

| 层次遍历 | BFS |
| :--- | :--- |
| 根节点入队 | 起点编号入队 |
| 出队并输出 `data` | 出队并输出 `vertices[u]` |
| 左右孩子入队 | 当前行所有未访问邻接点入队 |
| 树中不会重复到达 | 图中可能重复到达，依赖 `visited` |

### 5.2 单次 BFS

```cpp
void bfs(const Graph& graph, int v, bool visited[]);
```

参数：

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `const Graph&` | 只读图；不复制、不修改 |
| `v` | `int` | 起点编号（下标） |
| `visited` | `bool[]` | 访问标记数组；传首地址，函数内修改；长度至少 `numVertices`，不重置 |

步骤：

1. `visited[v]` 为 `true` 时返回；
2. 创建 `std::queue<int>`，标记起点，起点入队；
3. 队列非空时，取队首 `u`、出队、输出 `graph.vertices[u]`；
4. 按 `w = 0～numVertices-1` 扫描第 `u` 行；存在边且未访问时，标记 `w` 后入队；
5. 重复至队列为空。

代码：

```cpp
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
```

标记发生在入队时。若推迟到出队时标记，同一顶点可能被多个前驱重复入队，输出出现重复。`visited` 在此表示“已发现、不再入队”，而非“已输出”。

### 5.3 全图 BFS

```cpp
void bfsAll(const Graph& graph) {
    bool visited[MAXV] = {};
    for (int v = 0; v < graph.numVertices; ++v) {
        if (!visited[v]) {
            bfs(graph, v, visited);
        }
    }
}
```

参数：

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `const Graph&` | 只读图；`visited` 由函数内部创建 |

外层使用 `continue` 跳过已访问顶点，不能写成 `return`。写成 `return` 时，首个连通分量结束后函数整体退出，后续分量与孤立点不会输出。

### 5.4 示例追踪

从 `A` 出发，队列按“队首 → 队尾”记录：

| 轮次 | 出队 | 新入队 | 本轮结束队列 | 已标记 |
| :---: | :---: | :--- | :--- | :--- |
| 1 | A | B、C | `[B, C]` | A B C |
| 2 | B | D | `[C, D]` | A B C D |
| 3 | C | E | `[D, E]` | A B C D E |
| 4 | D | 无 | `[E]` | A B C D E |
| 5 | E | 无 | `[]` | A B C D E |

输出：

```text
A B C D E
```

`F` 由 `bfsAll` 补充，全图 BFS 输出 `A B C D E F`。

### 5.5 复杂度

每个顶点出队一次并扫描一行 `n` 个位置。单次 BFS 为 **O(n²)**，可达 `k` 个顶点时为 **Θ(k·n)**；`bfsAll` 为 **O(n²)**。队列最坏同时保存 **O(n)** 个编号，`visited` 为 **O(n)**。

---

## 6. 验证

### 6.1 预期输出

演示图包含 6 个顶点 `A～F`，无向边为 `A—B、A—C、B—C、B—D、C—E`，`F` 为孤立点：

```text
邻接矩阵：
  A B C D E F
A 0 1 1 0 0 0
B 1 0 1 1 0 0
C 1 1 0 0 1 0
D 0 1 0 0 0 0
E 0 0 1 0 0 0
F 0 0 0 0 0 0
顶点数：6，边数：5
从 A 出发的 DFS：A B C E D
从 A 出发的 BFS：A B C D E
全图 DFS：A B C E D F
全图 BFS：A B C D E F
```

每个字符后有一个空格，行末空格可忽略。序列必须由实际搜索产生，不能直接输出固定字符串。

### 6.2 测试

`main.cpp` 包含 18 组测试，覆盖：

- 空图、单顶点、容量边界；
- 无向边的插入、重复插入、非法插入；
- 无向边的删除、非法删除；
- 有向边的方向与独立计数；
- 有环图、孤立点、多个连通分量；
- 单次搜索的可达性与已有访问标记；
- Lab11 树形图的 DFS/BFS 对应；
- 稠密图、增删边后的重复遍历。

全部通过时输出：

```text
graph tests: 18/18 passed
```

任一组失败，程序返回退出码 `1`。CTest 超时时间为 10 秒，未终止的递归或搜索会导致超时。

### 6.3 失败模式

| 现象 | 检查项 |
| :--- | :--- |
| 全图遍历缺少编号最大的顶点 | 循环条件是否写成 `w < numVertices - 1` |
| 全图只输出首个连通分量 | `dfsAll` 是否写死起点 `0`；`bfsAll` 是否将 `continue` 写成 `return` |
| 负数编号未被拒绝 | 合法性检查是否缺少 `from < 0` / `to < 0` |
| 无向图删除不彻底 | 是否同步清除 `edge[to][from]` |
| 边数多一或少一 | 无向边是否将 `numEdges` 增减两次 |
| BFS 重复输出顶点 | 是否在出队时才标记 `visited` |
| 递归栈溢出 | 是否缺少 `visited[v]` 的判断或置位 |
| 编译警告 non-void 无返回值 | `insertEdge` / `removeEdge` 尚未补全 |

### 6.4 调试顺序

1. 运行演示程序，检查矩阵与边数；
2. 使用 `--test` 检查前 8 组边操作，全部通过后再检查遍历；
3. 对失败用例绘制小图，按编号顺序手工执行；
4. 运行 `ctest`，确认 18 组全部通过。

---

## 7. 实验记录

本节记录个人运行结果与三个简答题，不需要抄写题目。

### 7.1 运行记录

运行 `lab17_graph`，记录：

1. 邻接矩阵、顶点数、边数；
2. 从 `A` 出发的 DFS、BFS；
3. 全图 DFS、BFS；
4. 18 组测试的完整输出及汇总行。

> （填写）

### 7.2 简答

1. 结合 4.5、5.4 节的追踪，说明 DFS 与 BFS 输出顺序不同的原因。

   > （填写）

2. BFS 中顶点尚未输出时 `visited` 已为 `true`。说明该标记的含义，以及改为出队时标记的后果。

   > （填写）

3. 说明矩阵存储下 `dfsAll`、`bfsAll` 为 O(n²) 而非 O(n+e) 的原因。

   > （填写）

---

## 8. 复杂度汇总

设实际顶点数为 `n`，边数为 `e`。

| 操作 | 时间 | 空间 | 说明 |
| :--- | :---: | :---: | :--- |
| 查找一条边 | **O(1)** | O(1) | 直接读取 `edge[u][v]` |
| 插入一条边 | **O(1)** | O(1) | 写入一个或两个对称位置 |
| 删除一条边 | **O(1)** | O(1) | 同插入 |
| 扫描一个顶点的邻接点 | **O(n)** | O(1) | 扫描完整一行 |
| 单次 DFS | **O(n²)** | O(n) | 可达 `k` 个顶点时为 Θ(k·n)；递归栈最坏 O(n) |
| 单次 BFS | **O(n²)** | O(n) | 队列最坏 O(n) |
| `dfsAll` / `bfsAll` | **O(n²)** | O(n) | 每个顶点访问一次，每行扫描一次 |

补充：

- 源码中 `visited` 为 `bool visited[MAXV]`，固定分配 `MAXV` 个位置；可扩展版本按 `n` 计。
- `n×n` 矩阵属于图的存储结构，不计入遍历的辅助空间。
- `initGraph` 清零整个 `MAXV×MAXV` 矩阵；`MAXV` 固定时为常数，容量按顶点数配置时为 O(n²)。

---

## 9. 提交

提交文件：

```text
学号姓名/
└── Lab17/
    ├── graph.cpp
    ├── traversal.cpp
    └── Lab17.md
```

要求：

- `graph.h`、`main.cpp`、`CMakeLists.txt` 为教师文件，本地保留，不修改、不提交。
- 保留六个学生函数的名称、参数与返回类型；保留 `initGraph`、`printMatrix`。
- 六处 `TODO` 补全为实际实现，并删除对应 `TODO` 注释；不得保留空函数、固定返回值或固定输出序列。
- 保留本文档章节结构，仅填写第七节记录；不要求抄题、不要求截图。
- 不提交 `build/`、`cmake-build-*`、`.idea/`、可执行文件或其他编译产物。
- 文件名与目录名区分大小写；PR 标题为 `[学号姓名]Lab17作业提交`，右方括号后不留空格。

---

## 附录 A：函数参数速查

### `initGraph(Graph& graph, const char* labels, int count, bool directed)`

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `Graph&` | 待初始化的图；直接修改调用者的对象 |
| `labels` | `const char*` | 顶点标签数组，如 `"ABCDEF"`；只读 |
| `count` | `int` | 实际顶点数，范围 `0～MAXV` |
| `directed` | `bool` | `true` 有向图，`false` 无向图；默认 `false` |

### `printMatrix(const Graph& graph)`

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `const Graph&` | 只读图；不复制矩阵，不允许修改 |

### `insertEdge(Graph& graph, int from, int to)`

| 参数/返回值 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `Graph&` | 待修改的图；直接修改调用者的对象 |
| `from` | `int` | 起点编号（下标），例如 `A` 对应 `0` |
| `to` | `int` | 终点编号（下标） |
| 返回值 | `bool` | `true` 插入成功；`false` 编号非法、自环或边已存在，图不变 |

### `removeEdge(Graph& graph, int from, int to)`

| 参数/返回值 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `Graph&` | 待修改的图；直接修改调用者的对象 |
| `from` | `int` | 起点编号（下标） |
| `to` | `int` | 终点编号（下标） |
| 返回值 | `bool` | `true` 删除成功；`false` 编号非法、自环或边不存在，图不变 |

### `dfs(const Graph& graph, int v, bool visited[])`

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `const Graph&` | 只读图；不复制、不修改 |
| `v` | `int` | 当前顶点编号；首次调用为起点，递归时为邻接点 |
| `visited` | `bool[]` | 访问标记数组；传首地址，函数内修改调用者的数组；长度至少 `numVertices`，由调用者初始化，函数内不清空 |

### `dfsAll(const Graph& graph)`

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `const Graph&` | 只读图；`visited` 由函数内部创建 |

### `bfs(const Graph& graph, int v, bool visited[])`

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `const Graph&` | 只读图；不复制、不修改 |
| `v` | `int` | 起点编号（下标） |
| `visited` | `bool[]` | 访问标记数组；传首地址，函数内修改；长度至少 `numVertices`，不重置 |

### `bfsAll(const Graph& graph)`

| 参数 | 类型 | 含义 |
| :--- | :--- | :--- |
| `graph` | `const Graph&` | 只读图；`visited` 由函数内部创建 |

### 通用规则

1. `&` 表示引用：不复制实参，函数内修改调用者的对象；`const` 表示只读。
2. `bool visited[]` 退化为 `bool*`，函数内通过下标修改原数组，`dfs`、`bfs` 因此共享访问记录。
3. `int` 为编号，`char` 为标签；输出标签使用 `graph.vertices[v]`。
