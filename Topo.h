#ifndef TOPO_H
#define TOPO_H

// ============================================================
//  Topological Sort Module — Directed Acyclic Graph (DAG)
//  Algorithm: Kahn's Algorithm (BFS-based)
//  Use Case: Order processing pipeline, delivery dependency
//  Time: O(V+E)
// ============================================================

#include <iostream>
#include <cstring>
using namespace std;

#define TOPO_MAX 20

class TopologicalSort {
    int  adjMatrix[TOPO_MAX][TOPO_MAX]; // directed adjacency matrix
    int  indegree[TOPO_MAX];
    char stageName[TOPO_MAX][50];
    int  numNodes;

public:
    TopologicalSort() : numNodes(0) {
        for (int i = 0; i < TOPO_MAX; i++) {
            indegree[i] = 0;
            stageName[i][0] = '\0';
            for (int j = 0; j < TOPO_MAX; j++) adjMatrix[i][j] = 0;
        }
        loadDeliveryPipeline();
    }

    // Add a stage/node
    int addNode(const char* name) {
        if (numNodes >= TOPO_MAX) { cout << "[Topo] Max nodes reached.\n"; return -1; }
        int v = numNodes++;
        strncpy(stageName[v], name, 49);
        cout << "[Topo] Added stage [" << v << "]: " << name << "\n";
        return v;
    }

    // Add directed edge u -> v (u must complete before v)
    void addDirectedEdge(int u, int v) {
        if (u < 0 || v < 0 || u >= numNodes || v >= numNodes) {
            cout << "[Topo] Invalid indices.\n"; return;
        }
        if (adjMatrix[u][v]) return;  // already exists
        adjMatrix[u][v] = 1;
        indegree[v]++;
        cout << "[Topo] Edge: " << stageName[u] << " -> " << stageName[v] << "\n";
    }

    // Kahn's Algorithm for topological ordering
    void performSort() const {
        cout << "\n====== TOPOLOGICAL SORT (Kahn's Algorithm) ======\n";
        cout << "Processing Order:\n";

        // Copy indegrees
        int deg[TOPO_MAX];
        for (int i = 0; i < numNodes; i++) deg[i] = indegree[i];

        // Manual queue (array-based)
        int queue[TOPO_MAX];
        int front = 0, rear = 0;

        // Enqueue all nodes with indegree 0
        for (int i = 0; i < numNodes; i++)
            if (deg[i] == 0) queue[rear++] = i;

        int processed = 0;
        int step = 1;
        while (front < rear) {
            int u = queue[front++];
            cout << "  Step " << step++ << ": " << stageName[u] << "\n";
            processed++;

            // Reduce indegree of all neighbors
            for (int v = 0; v < numNodes; v++) {
                if (adjMatrix[u][v]) {
                    deg[v]--;
                    if (deg[v] == 0) queue[rear++] = v;
                }
            }
        }

        if (processed < numNodes) {
            cout << "\n[WARNING] Cycle detected! Graph is NOT a valid DAG.\n";
        } else {
            cout << "\n[Topo] All " << processed << " stages ordered successfully.\n";
        }
    }

    void displayDAG() const {
        cout << "\n====== DELIVERY PIPELINE DAG ======\n";
        for (int i = 0; i < numNodes; i++) {
            cout << "[" << i << "] " << stageName[i] << " -> ";
            bool any = false;
            for (int j = 0; j < numNodes; j++) {
                if (adjMatrix[i][j]) { cout << stageName[j] << "  "; any = true; }
            }
            if (!any) cout << "(terminal)";
            cout << "\n";
        }
    }

    // Pre-load: Payment -> Inventory Check -> Packaging -> Shipping -> Out for Delivery -> Delivered
    void loadDeliveryPipeline() {
        const char* stages[] = {
            "Payment", "Inventory Check", "Packaging",
            "Quality Control", "Shipping", "Out for Delivery", "Delivered"
        };
        for (int i = 0; i < 7; i++) {
            strncpy(stageName[numNodes++], stages[i], 49);
        }
        // Dependencies
        int edges[][2] = {
            {0,1}, {1,2}, {2,3}, {3,4}, {4,5}, {5,6},  // main chain
            {0,2}                                         // Payment also unlocks Packaging
        };
        for (auto& e : edges) {
            adjMatrix[e[0]][e[1]] = 1;
            indegree[e[1]]++;
        }
    }

    int getNumNodes() const { return numNodes; }
};

#endif
