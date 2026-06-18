#ifndef PRODUCT_H
#define PRODUCT_H

// ============================================================
//  Product Management Module
//  Data Structure: Array (manual, fixed-size)
//  Includes all 6 Sorting Algorithms
// ============================================================

#include <iostream>
#include <cstring>
using namespace std;

#define MAX_PRODUCTS 100

struct Product {
    int    id;
    char   name[50];
    double price;
    int    stock;
    int    salesCount;
    double rating;
    bool   active;   // soft-delete flag
};

// ============================================================
//  SORTING ALGORITHMS (all manual, no STL)
//  Complexity notes in each function
// ============================================================

// Bubble Sort — O(n²) average/worst, O(n) best
void bubbleSortByPrice(Product arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j].price > arr[j+1].price)
                swap(arr[j], arr[j+1]);
}

// Selection Sort — O(n²) all cases
void selectionSortBySales(Product arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j].salesCount < arr[minIdx].salesCount)
                minIdx = j;
        if (minIdx != i) swap(arr[i], arr[minIdx]);
    }
}

// Insertion Sort — O(n²) worst, O(n) best (nearly sorted)
void insertionSortByRating(Product arr[], int n) {
    for (int i = 1; i < n; i++) {
        Product key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].rating < key.rating) {  // descending
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

// Quick Sort helpers — O(n log n) avg, O(n²) worst
int partition(Product arr[], int low, int high, int mode) {
    // mode: 0=price, 1=sales, 2=rating
    double pivot = (mode == 0) ? arr[high].price :
                   (mode == 1) ? arr[high].salesCount :
                                 arr[high].rating;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        double val = (mode == 0) ? arr[j].price :
                     (mode == 1) ? arr[j].salesCount :
                                   arr[j].rating;
        if (val <= pivot) { i++; swap(arr[i], arr[j]); }
    }
    swap(arr[i+1], arr[high]);
    return i + 1;
}

void quickSort(Product arr[], int low, int high, int mode) {
    if (low < high) {
        int pi = partition(arr, low, high, mode);
        quickSort(arr, low, pi - 1, mode);
        quickSort(arr, pi + 1, high, mode);
    }
}

// Merge Sort helpers — O(n log n) all cases
void merge(Product arr[], int l, int m, int r, int mode) {
    int n1 = m - l + 1, n2 = r - m;
    Product* L = new Product[n1];
    Product* R = new Product[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        double lv = (mode == 0) ? L[i].price : (mode == 1) ? L[i].salesCount : L[i].rating;
        double rv = (mode == 0) ? R[j].price : (mode == 1) ? R[j].salesCount : R[j].rating;
        if (lv <= rv) arr[k++] = L[i++];
        else          arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    delete[] L; delete[] R;
}

void mergeSort(Product arr[], int l, int r, int mode) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m, mode);
        mergeSort(arr, m + 1, r, mode);
        merge(arr, l, m, r, mode);
    }
}

// Heap Sort helpers — O(n log n) all cases
void heapify(Product arr[], int n, int i, int mode) {
    int largest = i, l = 2*i+1, r = 2*i+2;
    auto val = [&](int idx) -> double {
        return (mode == 0) ? arr[idx].price :
               (mode == 1) ? arr[idx].salesCount :
                             arr[idx].rating;
    };
    if (l < n && val(l) > val(largest)) largest = l;
    if (r < n && val(r) > val(largest)) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest, mode);
    }
}

void heapSort(Product arr[], int n, int mode) {
    for (int i = n/2 - 1; i >= 0; i--) heapify(arr, n, i, mode);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, mode);
    }
}

// ============================================================
//  BINARY SEARCH — O(log n), requires sorted array
// ============================================================
int binarySearchByID(Product arr[], int n, int targetID) {
    int low = 0, high = n - 1;
    // Sort by ID first (insertion sort for simplicity)
    for (int i = 1; i < n; i++) {
        Product key = arr[i]; int j = i - 1;
        while (j >= 0 && arr[j].id > key.id) { arr[j+1] = arr[j]; j--; }
        arr[j+1] = key;
    }
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].id == targetID) return mid;
        else if (arr[mid].id < targetID) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// ============================================================
//  PRODUCT MANAGER CLASS
// ============================================================
class ProductManager {
    Product products[MAX_PRODUCTS];
    int     count;
    int     nextID;

    void printHeader() const {
        cout << left;
        cout.width(6);  cout << "ID";
        cout.width(22); cout << "Name";
        cout.width(10); cout << "Price";
        cout.width(8);  cout << "Stock";
        cout.width(8);  cout << "Sales";
        cout << "Rating\n";
        cout << string(62, '-') << "\n";
    }

    void printProduct(const Product& p) const {
        cout.width(6);  cout << p.id;
        cout.width(22); cout << p.name;
        cout.width(10); cout << p.price;
        cout.width(8);  cout << p.stock;
        cout.width(8);  cout << p.salesCount;
        cout << p.rating << "\n";
    }

public:
    ProductManager() : count(0), nextID(101) { loadSampleData(); }

    void loadSampleData() {
        const char* names[] = {
            "Laptop Pro 15", "Wireless Mouse", "Mechanical Keyboard",
            "USB-C Hub", "Monitor 27\"", "Webcam HD", "Headphones BT",
            "SSD 1TB", "RAM 16GB", "Smartphone X"
        };
        double prices[] = {1299.99, 29.99, 89.99, 49.99, 399.99, 79.99, 149.99, 119.99, 79.99, 899.99};
        int stocks[]    = {15, 80, 45, 60, 20, 35, 50, 40, 30, 25};
        int sales[]     = {120, 540, 210, 380, 95, 160, 290, 450, 320, 180};
        double ratings[]= {4.8, 4.5, 4.7, 4.3, 4.6, 4.2, 4.4, 4.9, 4.6, 4.7};

        for (int i = 0; i < 10; i++) {
            Product& p = products[count++];
            p.id = nextID++;
            strncpy(p.name, names[i], 49);
            p.price = prices[i]; p.stock = stocks[i];
            p.salesCount = sales[i]; p.rating = ratings[i];
            p.active = true;
        }
    }

    // Add product
    bool addProduct(const char* name, double price, int stock) {
        if (count >= MAX_PRODUCTS) { cout << "Product storage full!\n"; return false; }
        Product& p = products[count++];
        p.id = nextID++;
        strncpy(p.name, name, 49);
        p.price = price; p.stock = stock;
        p.salesCount = 0; p.rating = 0.0; p.active = true;
        cout << "[Product] Added: " << name << " (ID: " << p.id << ")\n";
        return true;
    }

    // Insert at beginning (shift right)
    bool insertAtBeginning(const char* name, double price, int stock) {
        if (count >= MAX_PRODUCTS) return false;
        for (int i = count; i > 0; i--) products[i] = products[i-1];
        Product& p = products[0];
        p.id = nextID++; strncpy(p.name, name, 49);
        p.price = price; p.stock = stock;
        p.salesCount = 0; p.rating = 0.0; p.active = true;
        count++;
        cout << "[Product] Inserted at beginning: " << name << "\n";
        return true;
    }

    // Insert at position (0-indexed)
    bool insertAtPosition(int pos, const char* name, double price, int stock) {
        if (count >= MAX_PRODUCTS || pos < 0 || pos > count) return false;
        for (int i = count; i > pos; i--) products[i] = products[i-1];
        Product& p = products[pos];
        p.id = nextID++; strncpy(p.name, name, 49);
        p.price = price; p.stock = stock;
        p.salesCount = 0; p.rating = 0.0; p.active = true;
        count++;
        cout << "[Product] Inserted at position " << pos << ": " << name << "\n";
        return true;
    }

    // Delete by ID
    bool deleteProduct(int id) {
        for (int i = 0; i < count; i++) {
            if (products[i].id == id) {
                for (int j = i; j < count - 1; j++) products[j] = products[j+1];
                count--;
                cout << "[Product] Deleted product ID " << id << "\n";
                return true;
            }
        }
        cout << "[Product] ID not found.\n"; return false;
    }

    // Delete from beginning
    bool deleteFromBeginning() {
        if (count == 0) { cout << "No products.\n"; return false; }
        cout << "[Product] Deleted: " << products[0].name << "\n";
        for (int i = 0; i < count - 1; i++) products[i] = products[i+1];
        count--; return true;
    }

    // Delete from end
    bool deleteFromEnd() {
        if (count == 0) { cout << "No products.\n"; return false; }
        cout << "[Product] Deleted: " << products[count-1].name << "\n";
        count--; return true;
    }

    // Update product
    bool updateProduct(int id, double newPrice, int newStock) {
        for (int i = 0; i < count; i++) {
            if (products[i].id == id) {
                products[i].price = newPrice;
                products[i].stock = newStock;
                cout << "[Product] Updated ID " << id << "\n";
                return true;
            }
        }
        cout << "[Product] ID not found.\n"; return false;
    }

    // View all
    void viewAll() const {
        if (count == 0) { cout << "No products available.\n"; return; }
        cout << "\n========== ALL PRODUCTS ==========\n";
        printHeader();
        for (int i = 0; i < count; i++)
            if (products[i].active) printProduct(products[i]);
    }

    // Find by ID (returns pointer, nullptr if not found)
    Product* findByID(int id) {
        for (int i = 0; i < count; i++)
            if (products[i].id == id && products[i].active) return &products[i];
        return nullptr;
    }

    // ── Sorting wrappers ──────────────────────────────────
    void sortAndDisplay(int algo, int field) {
        // Build a working copy
        int n = count;
        Product temp[MAX_PRODUCTS];
        for (int i = 0; i < n; i++) temp[i] = products[i];

        const char* algoName[] = {"Bubble","Selection","Insertion","Quick","Merge","Heap"};
        const char* fieldName[]= {"Price","Sales Count","Rating"};
        cout << "\n[Sort] " << algoName[algo-1] << " Sort by " << fieldName[field-1] << ":\n";

        switch (algo) {
            case 1: bubbleSortByPrice(temp, n);              break;
            case 2: selectionSortBySales(temp, n);           break;
            case 3: insertionSortByRating(temp, n);          break;
            case 4: quickSort(temp, 0, n-1, field-1);        break;
            case 5: mergeSort(temp, 0, n-1, field-1);        break;
            case 6: heapSort(temp, n, field-1);              break;
        }
        printHeader();
        for (int i = 0; i < n; i++) printProduct(temp[i]);
    }

    // ── Binary Search ─────────────────────────────────────
    void searchByID(int id) {
        Product temp[MAX_PRODUCTS];
        int n = count;
        for (int i = 0; i < n; i++) temp[i] = products[i];
        int idx = binarySearchByID(temp, n, id);
        if (idx == -1) { cout << "[Search] Product ID " << id << " not found.\n"; return; }
        cout << "\n[Search] Found:\n";
        printHeader(); printProduct(temp[idx]);
    }

    // Provide raw array access for BST/Heap/BIT initialization
    int      getCount() const { return count; }
    Product* getArray()       { return products; }
};

#endif
