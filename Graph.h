#ifndef GRAPH_H
#define GRAPH_H

// ============================================================
//  Graph Module (Adjacency List, manual implementation)
//  Use Case: Product recommendation system
//  BFS/DFS traversal, neighbor recommendation
//  Time: O(V+E) for BFS/DFS
// ============================================================

#include <iostream>
#include <cstring>
using namespace std;

#define GRAPH_MAX_V 50   // max vertices (products/categories)

// Adjacency list node
struct AdjNode {
    int       vertex;
    char      label[30];   // product name stored for display
    AdjNode*  next;

    AdjNode(int v, const char* l) : vertex(v), next(nullptr) {
        strncpy(label, l, 29);
    }
};

// Simple manual Queue for BFS
struct GQueue {
    int data[GRAPH_MAX_V * 2];
    int front, rear;
    GQueue() : front(0), rear(0) {}
    bool isEmpty() const { return front == rear; }
    void enqueue(int v) { data[rear++ % (GRAPH_MAX_V*2)] = v; }
    int  dequeue()      { return data[front++ % (GRAPH_MAX_V*2)]; }
};

class Graph {
    AdjNode* adjList[GRAPH_MAX_V];  // adjacency list heads
    char     vertexName[GRAPH_MAX_V][50];
    bool     active[GRAPH_MAX_V];
    int      numVertices;

    bool visited[GRAPH_MAX_V];

    void dfsHelper(int v) {
        visited[v] = true;
        cout << "  -> [" << v << "] " << vertexName[v] << "\n";
        AdjNode* cur = adjList[v];
        while (cur) {
            if (!visited[cur->vertex]) dfsHelper(cur->vertex);
            cur = cur->next;
        }
    }

    void clearVisited() {
        for (int i = 0; i < GRAPH_MAX_V; i++) visited[i] = false;
    }

public:
    Graph() : numVertices(0) {
        for (int i = 0; i < GRAPH_MAX_V; i++) {
            adjList[i] = nullptr;
            active[i]  = false;
            visited[i] = false;
            vertexName[i][0] = '\0';
        }
        loadSampleData();
    }

    ~Graph() {
        for (int i = 0; i < GRAPH_MAX_V; i++) {
            AdjNode* cur = adjList[i];
            while (cur) { AdjNode* tmp = cur->next; delete cur; cur = tmp; }
        }
    }

    // Add a vertex (product/category node), returns its index
    int addVertex(const char* name) {
        if (numVertices >= GRAPH_MAX_V) { cout << "[Graph] Max vertices reached.\n"; return -1; }
        int v = numVertices++;
        strncpy(vertexName[v], name, 49);
        active[v] = true;
        cout << "[Graph] Added vertex [" << v << "] " << name << "\n";
        return v;
    }

    // Add undirected edge
    void addEdge(int u, int v) {
        if (u < 0 || v < 0 || u >= numVertices || v >= numVertices) {
            cout << "[Graph] Invalid vertex indices.\n"; return;
        }
        // u -> v
        AdjNode* node1 = new AdjNode(v, vertexName[v]);
        node1->next = adjList[u];
        adjList[u] = node1;
        // v -> u (undirected)
        AdjNode* node2 = new AdjNode(u, vertexName[u]);
        node2->next = adjList[v];
        adjList[v] = node2;
        cout << "[Graph] Edge: " << vertexName[u] << " <-> " << vertexName[v] << "\n";
    }

    // BFS from source — O(V+E)
    void bfs(int start) {
        if (start < 0 || start >= numVertices) { cout << "[BFS] Invalid start.\n"; return; }
        clearVisited();
        GQueue q;
        cout << "\n--- BFS from [" << start << "] " << vertexName[start] << " ---\n";
        visited[start] = true;
        q.enqueue(start);
        while (!q.isEmpty()) {
            int v = q.dequeue();
            cout << "  [" << v << "] " << vertexName[v] << "\n";
            AdjNode* cur = adjList[v];
            while (cur) {
                if (!visited[cur->vertex]) {
                    visited[cur->vertex] = true;
                    q.enqueue(cur->vertex);
                }
                cur = cur->next;
            }
        }
    }

    // DFS from source — O(V+E)
    void dfs(int start) {
        if (start < 0 || start >= numVertices) { cout << "[DFS] Invalid start.\n"; return; }
        clearVisited();
        cout << "\n--- DFS from [" << start << "] " << vertexName[start] << " ---\n";
        dfsHelper(start);
    }

    // Recommend products related to a given product (vertex)
    void recommend(int productVertex) {
        if (productVertex < 0 || productVertex >= numVertices) {
            cout << "[Recommend] Invalid product.\n"; return;
        }
        cout << "\n--- Recommendations for: " << vertexName[productVertex] << " ---\n";
        AdjNode* cur = adjList[productVertex];
        if (!cur) { cout << "No recommendations available.\n"; return; }
        while (cur) {
            cout << "  You might also like: [" << cur->vertex << "] " << vertexName[cur->vertex] << "\n";
            cur = cur->next;
        }
    }

    void displayGraph() const {
        cout << "\n===== PRODUCT GRAPH (Adjacency List) =====\n";
        for (int i = 0; i < numVertices; i++) {
            cout << "[" << i << "] " << vertexName[i] << " -> ";
            AdjNode* cur = adjList[i];
            if (!cur) { cout << "NULL\n"; continue; }
            while (cur) {
                cout << vertexName[cur->vertex];
                if (cur->next) cout << " -> ";
                cur = cur->next;
            }
            cout << "\n";
        }
    }

    int getNumVertices() const { return numVertices; }
    const char* getVertexName(int v) const { return vertexName[v]; }

    // Pre-load sample product relationships
    void loadSampleData() {
        // 0: Laptop, 1: Mouse, 2: Keyboard, 3: Monitor, 4: USB Hub,
        // 5: Webcam, 6: Headphones, 7: SSD, 8: RAM, 9: Smartphone
        const char* prods[] = {
            "Laptop", "Mouse", "Keyboard", "Monitor",
            "USB Hub", "Webcam", "Headphones", "SSD", "RAM", "Smartphone"
        };
        for (int i = 0; i < 10; i++) {
            int v = numVertices++;
            strncpy(vertexName[v], prods[i], 49);
            active[v] = true;
        }
        // Add edges (related products)
        auto addE = [&](int u, int v) {
            AdjNode* n1 = new AdjNode(v, vertexName[v]); n1->next = adjList[u]; adjList[u] = n1;
            AdjNode* n2 = new AdjNode(u, vertexName[u]); n2->next = adjList[v]; adjList[v] = n2;
        };
        addE(0, 1); addE(0, 2); addE(0, 3); addE(0, 4);  // Laptop <-> Mouse/Keyboard/Monitor/Hub
        addE(0, 7); addE(0, 8);                            // Laptop <-> SSD/RAM
        addE(3, 5);                                        // Monitor <-> Webcam
        addE(2, 6);                                        // Keyboard <-> Headphones
        addE(9, 1);                                        // Smartphone <-> Mouse
        addE(4, 5);                                        // USB Hub <-> Webcam
    }
};

#endif
