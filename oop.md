# From Procedural C to Object-Oriented Design: A Dependency Graph Case Study

When learning programming, many developers start with procedural languages like C. The approach is straightforward: define data structures, write functions that operate on them, and execute everything from `main`. This works well for small programs, but as software grows, managing complexity becomes difficult.

Object-oriented programming (OOP), used in languages like C++ and Python, provides a different approach. It organizes code around **objects** that combine data and behavior, leading to more maintainable and extensible programs.

A simple dependency graph analyzer is a good example to illustrate the differences between:

- Plain C (procedural)
- C++ with object-oriented design
- Python with object-oriented design

## The Problem: Dependency Graph Analysis

Assume we have a file containing dependencies:
A -> B
B -> C
C -> A
D -> E

We want to:

1. Build a directed graph.
2. Find all paths.
3. Detect circular dependencies.

## Procedural Approach (Plain C)

In C, we typically:

1. Define structs for nodes and edges.
2. Manually manage memory.
3. Write functions that operate on these structures.

### Example (simplified plain C)

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_EDGES 10
#define NAME_LEN 32

typedef struct {
    char name[NAME_LEN];
    int neighbors[MAX_EDGES];
    int neighbor_count;
} Node;

Node graph[MAX_NODES];
int node_count = 0;

int find_node(const char *name) {
    for (int i = 0; i < node_count; i++) {
        if (strcmp(graph[i].name, name) == 0)
            return i;
    }
    return -1;
}

int add_node(const char *name) {
    int idx = find_node(name);
    if (idx != -1)
        return idx;

    strcpy(graph[node_count].name, name);
    graph[node_count].neighbor_count = 0;
    return node_count++;
}

void add_edge(const char *from, const char *to) {
    int a = add_node(from);
    int b = add_node(to);
    graph[a].neighbors[graph[a].neighbor_count++] = b;
}

int main() {
    add_edge("A", "B");
    add_edge("B", "C");
    add_edge("C", "A");

    for (int i = 0; i < node_count; i++) {
        printf("%s -> ", graph[i].name);
        for (int j = 0; j < graph[i].neighbor_count; j++) {
            printf("%s ", graph[graph[i].neighbors[j]].name);
        }
        printf("\n");
    }

    return 0;
}
```
### Characteristics of the C approach

**Advantages:**
- Very efficient.
- Full control over memory and layout.
- Simple execution model.

**Disadvantages:**
- Manual memory and structure management.
- No encapsulation.
- Harder to extend or reuse.
- Data and logic are loosely connected.

As the program grows, the number of parameters and global structures tends to increase.

## Object-Oriented Approach in C++

In C++, we can represent the graph as a class that owns its data and behavior.

**Example (C++ OOP)**
```cpp
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

class Graph {
private:
    std::unordered_map<std::string, std::vector<std::string>> adj;

public:
    void add_edge(const std::string& from, const std::string& to) {
        adj[from].push_back(to);
    }

    void print() const {
        for (const auto& pair : adj) {
            std::cout << pair.first << " -> ";
            for (const auto& n : pair.second) {
                std::cout << n << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    Graph g;
    g.add_edge("A", "B");
    g.add_edge("B", "C");
    g.add_edge("C", "A");

    g.print();
    return 0;
}
```

### Improvements over plain C
1. Encapsulation

The adjacency list is hidden inside the Graph class.

Only class methods can modify it.

2. Clear structure

Instead of:
- global arrays
- loose functions

We have:

Graph
 ├──|-- data: adjacency list
 └──|__ methods: add_edge, print, find_paths, etc.

3. Easier extension

We can add new methods:
```cpp
bool has_cycle();
std::vector<std::string> topological_sort();
```

Without changing the rest of the program.

## Object-Oriented Approach in Python

Python encourages a lightweight OOP style with less boilerplate.

**Example (Python OOP)**
```cpp
class Graph:
    def __init__(self):
        self.adj = {}

    def add_edge(self, a, b):
        self.adj.setdefault(a, []).append(b)

    def print(self):
        for node, neighbors in self.adj.items():
            print(node, "->", " ".join(neighbors))


g = Graph()
g.add_edge("A", "B")
g.add_edge("B", "C")
g.add_edge("C", "A")

g.print()
```

## Python OOP advantages
- Very concise syntax.
- Dynamic data structures.
- Easy to prototype.
- Minimal boilerplate.

For tasks like dependency analysis or scripting tools, Python often results in the shortest and most readable solution.

## Key Differences at a Glance
| Feature |	Plain C|	C++ OOP	| Python OOP |
|---------|--------|------------|------------|
|Memory management |	Manual	| Automatic (RAII) |	Automatic (GC)|
|Encapsulation|	No	|Yes	|Yes
|Code organization |	Functions + structs	| Classes |	Classes|
|Boilerplate	| Medium|	Medium|	Low |
|Performance	|Very high	|Very high	|Lower|
| Ease of extension	| Harder |	Easy	|Very easy|
|Rapid prototyping|	Moderate|	Moderate|	Excellent|

## Why OOP Helps in This Example

Using the dependency analyzer as context:

**In procedural style**
- Graph stored as loose containers.
- Path-finding functions receive many parameters.
- Logic spread across main and helper functions.

**In OOP style**

A single Graph object:
- Owns the adjacency list.
- Contains path-finding logic.
- Provides clear interfaces.

Example:
```cpp
Graph g;
g.load("dependencies.txt");
g.find_all_paths();
g.detect_cycles();
``` 

The intent of the program becomes immediately clear.

## When Plain C Is Still the Right Choice

Despite the advantages of OOP, plain C remains useful when:

1. Performance and memory control are critical.
2. The program is small and self-contained.
3. You are writing:
        - Operating systems
        - Embedded software
        - HPC kernels
        - Low-level libraries

In these cases, the simplicity and predictability of C are more important than abstraction.

## Conclusion

Procedural programming, as used in plain C, is simple and efficient, and works well for small or low-level programs. However, as software grows, managing complexity becomes difficult.

Object-oriented programming in C++ and Python provides:

Better organization

Clearer structure

Encapsulation of data and behavior

Easier extension and maintenance

Greater reusability

In the dependency graph example, the shift from procedural design to an object-oriented structure turns a collection of containers and functions into a coherent, reusable abstraction. This is the core strength of OOP: it helps manage complexity as programs scale.
