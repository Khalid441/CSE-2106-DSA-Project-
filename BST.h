#ifndef BST_H
#define BST_H

// ============================================================
//  Binary Search Tree (BST) — keyed on Product ID
//  Operations: Insert, Search, Delete, Inorder, Pre, Post
//  Time: O(log n) avg, O(n) worst (unbalanced)
// ============================================================

#include <iostream>
#include <cstring>
using namespace std;

struct BSTNode {
    int    productID;
    char   productName[50];
    double price;
    int    salesCount;
    double rating;
    BSTNode* left;
    BSTNode* right;

    BSTNode(int id, const char* name, double p, int s, double r)
        : productID(id), price(p), salesCount(s), rating(r),
          left(nullptr), right(nullptr) {
        strncpy(productName, name, 49);
    }
};

class BST {
    BSTNode* root;

    BSTNode* insertHelper(BSTNode* node, int id, const char* name,
                          double price, int sales, double rating) {
        if (!node) return new BSTNode(id, name, price, sales, rating);
        if (id < node->productID) node->left  = insertHelper(node->left,  id, name, price, sales, rating);
        else if (id > node->productID) node->right = insertHelper(node->right, id, name, price, sales, rating);
        else cout << "[BST] Product ID " << id << " already exists.\n";
        return node;
    }

    BSTNode* searchHelper(BSTNode* node, int id) const {
        if (!node || node->productID == id) return node;
        if (id < node->productID) return searchHelper(node->left, id);
        return searchHelper(node->right, id);
    }

    // Find inorder successor (smallest in right subtree)
    BSTNode* minNode(BSTNode* node) {
        while (node->left) node = node->left;
        return node;
    }

    BSTNode* deleteHelper(BSTNode* node, int id) {
        if (!node) { cout << "[BST] ID " << id << " not found.\n"; return nullptr; }
        if (id < node->productID)      node->left  = deleteHelper(node->left, id);
        else if (id > node->productID) node->right = deleteHelper(node->right, id);
        else {
            // Node found
            if (!node->left) { BSTNode* tmp = node->right; delete node; return tmp; }
            if (!node->right){ BSTNode* tmp = node->left;  delete node; return tmp; }
            // Two children: replace with inorder successor
            BSTNode* succ = minNode(node->right);
            node->productID = succ->productID;
            strncpy(node->productName, succ->productName, 49);
            node->price = succ->price;
            node->salesCount = succ->salesCount;
            node->rating = succ->rating;
            node->right = deleteHelper(node->right, succ->productID);
        }
        return node;
    }

    void inorderHelper(BSTNode* node) const {
        if (!node) return;
        inorderHelper(node->left);
        cout << "  [" << node->productID << "] " << node->productName
             << " | $" << node->price << " | Sales: " << node->salesCount
             << " | Rating: " << node->rating << "\n";
        inorderHelper(node->right);
    }

    void preorderHelper(BSTNode* node) const {
        if (!node) return;
        cout << "  [" << node->productID << "] " << node->productName << "\n";
        preorderHelper(node->left);
        preorderHelper(node->right);
    }

    void postorderHelper(BSTNode* node) const {
        if (!node) return;
        postorderHelper(node->left);
        postorderHelper(node->right);
        cout << "  [" << node->productID << "] " << node->productName << "\n";
    }

    void destroyHelper(BSTNode* node) {
        if (!node) return;
        destroyHelper(node->left);
        destroyHelper(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroyHelper(root); }

    void insert(int id, const char* name, double price, int sales, double rating) {
        root = insertHelper(root, id, name, price, sales, rating);
        cout << "[BST] Inserted: " << name << " (ID: " << id << ")\n";
    }

    void search(int id) const {
        BSTNode* res = searchHelper(root, id);
        if (!res) { cout << "[BST] Product ID " << id << " not found.\n"; return; }
        cout << "[BST] Found -> ID: " << res->productID
             << " | " << res->productName
             << " | $" << res->price
             << " | Sales: " << res->salesCount
             << " | Rating: " << res->rating << "\n";
    }

    void remove(int id) {
        root = deleteHelper(root, id);
        cout << "[BST] Deleted product ID " << id << " (if it existed).\n";
    }

    void inorder() const {
        cout << "\n--- Inorder Traversal (Sorted by ID) ---\n";
        if (!root) { cout << "BST is empty.\n"; return; }
        inorderHelper(root);
    }

    void preorder() const {
        cout << "\n--- Preorder Traversal ---\n";
        if (!root) { cout << "BST is empty.\n"; return; }
        preorderHelper(root);
    }

    void postorder() const {
        cout << "\n--- Postorder Traversal ---\n";
        if (!root) { cout << "BST is empty.\n"; return; }
        postorderHelper(root);
    }

    bool isEmpty() const { return root == nullptr; }
};

#endif
