#ifndef HEAP_H
#define HEAP_H

// ============================================================
//  Heap Module — Max-Heap and Min-Heap (array-based)
//  Used for: Top/Bottom selling products, extract max sales
//  Time: O(log n) insert/extract, O(n) build
// ============================================================

#include <iostream>
#include <cstring>
using namespace std;

#define HEAP_MAX 100

struct HeapProduct {
    int    productID;
    char   productName[50];
    int    salesCount;
    double price;
};

// ─────────────────────────────────────────────
//  MAX HEAP (highest sales at root)
// ─────────────────────────────────────────────
class MaxHeap {
    HeapProduct data[HEAP_MAX];
    int         size;

    void heapifyUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data[parent].salesCount < data[i].salesCount) {
                swap(data[parent], data[i]);
                i = parent;
            } else break;
        }
    }

    void heapifyDown(int i) {
        int largest = i, l = 2*i+1, r = 2*i+2;
        if (l < size && data[l].salesCount > data[largest].salesCount) largest = l;
        if (r < size && data[r].salesCount > data[largest].salesCount) largest = r;
        if (largest != i) { swap(data[i], data[largest]); heapifyDown(largest); }
    }

public:
    MaxHeap() : size(0) {}

    void insert(int id, const char* name, int sales, double price) {
        if (size >= HEAP_MAX) { cout << "[MaxHeap] Full!\n"; return; }
        data[size].productID = id;
        strncpy(data[size].productName, name, 49);
        data[size].salesCount = sales;
        data[size].price = price;
        heapifyUp(size++);
    }

    HeapProduct extractMax() {
        if (size == 0) { cout << "[MaxHeap] Empty!\n"; return {}; }
        HeapProduct maxVal = data[0];
        data[0] = data[--size];
        heapifyDown(0);
        return maxVal;
    }

    void peekMax() const {
        if (size == 0) { cout << "[MaxHeap] Empty.\n"; return; }
        cout << "[MaxHeap] Top seller: [" << data[0].productID << "] "
             << data[0].productName << " | Sales: " << data[0].salesCount << "\n";
    }

    void displayTopN(int n) {
        cout << "\n--- Top " << n << " Best-Selling Products ---\n";
        MaxHeap copy = *this;
        for (int i = 0; i < n && copy.size > 0; i++) {
            HeapProduct p = copy.extractMax();
            cout << "  #" << (i+1) << " [" << p.productID << "] "
                 << p.productName << " | Sales: " << p.salesCount
                 << " | $" << p.price << "\n";
        }
    }

    void buildFromArray(int ids[], const char names[][50], int sales[], double prices[], int n) {
        size = 0;
        for (int i = 0; i < n; i++) insert(ids[i], names[i], sales[i], prices[i]);
    }

    int getSize() const { return size; }
};

// ─────────────────────────────────────────────
//  MIN HEAP (lowest sales at root)
// ─────────────────────────────────────────────
class MinHeap {
    HeapProduct data[HEAP_MAX];
    int         size;

    void heapifyUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data[parent].salesCount > data[i].salesCount) {
                swap(data[parent], data[i]);
                i = parent;
            } else break;
        }
    }

    void heapifyDown(int i) {
        int smallest = i, l = 2*i+1, r = 2*i+2;
        if (l < size && data[l].salesCount < data[smallest].salesCount) smallest = l;
        if (r < size && data[r].salesCount < data[smallest].salesCount) smallest = r;
        if (smallest != i) { swap(data[i], data[smallest]); heapifyDown(smallest); }
    }

public:
    MinHeap() : size(0) {}

    void insert(int id, const char* name, int sales, double price) {
        if (size >= HEAP_MAX) { cout << "[MinHeap] Full!\n"; return; }
        data[size].productID = id;
        strncpy(data[size].productName, name, 49);
        data[size].salesCount = sales;
        data[size].price = price;
        heapifyUp(size++);
    }

    HeapProduct extractMin() {
        if (size == 0) { cout << "[MinHeap] Empty!\n"; return {}; }
        HeapProduct minVal = data[0];
        data[0] = data[--size];
        heapifyDown(0);
        return minVal;
    }

    void displayBottomN(int n) {
        cout << "\n--- Bottom " << n << " Lowest-Selling Products ---\n";
        MinHeap copy = *this;
        for (int i = 0; i < n && copy.size > 0; i++) {
            HeapProduct p = copy.extractMin();
            cout << "  #" << (i+1) << " [" << p.productID << "] "
                 << p.productName << " | Sales: " << p.salesCount
                 << " | $" << p.price << "\n";
        }
    }

    void buildFromArray(int ids[], const char names[][50], int sales[], double prices[], int n) {
        size = 0;
        for (int i = 0; i < n; i++) insert(ids[i], names[i], sales[i], prices[i]);
    }

    int getSize() const { return size; }
};

#endif
