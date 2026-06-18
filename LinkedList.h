#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// ============================================================
//  Linked Lists
//   - Singly Linked List  → Shopping Cart
//   - Doubly Linked List  → Order History
//   - Circular Linked List→ Recently Viewed Products
// ============================================================

#include <iostream>
#include <cstring>
using namespace std;

// ─────────────────────────────────────────────
//  SINGLY LINKED LIST  (Shopping Cart)
// ─────────────────────────────────────────────
struct CartNode {
    int    productID;
    char   productName[50];
    int    quantity;
    double price;
    CartNode* next;

    CartNode(int id, const char* name, int qty, double p)
        : productID(id), quantity(qty), price(p), next(nullptr) {
        strncpy(productName, name, 49);
    }
};

class ShoppingCart {
    CartNode* head;
    int       itemCount;

public:
    ShoppingCart() : head(nullptr), itemCount(0) {}

    ~ShoppingCart() { clearCart(); }

    void addItem(int id, const char* name, int qty, double price) {
        // Check if product already in cart → increase quantity
        CartNode* cur = head;
        while (cur) {
            if (cur->productID == id) {
                cur->quantity += qty;
                cout << "[Cart] Updated quantity for " << name << " -> " << cur->quantity << "\n";
                return;
            }
            cur = cur->next;
        }
        CartNode* node = new CartNode(id, name, qty, price);
        node->next = head;
        head = node;
        itemCount++;
        cout << "[Cart] Added: " << name << " x" << qty << "\n";
    }

    bool removeItem(int id) {
        if (!head) { cout << "[Cart] Cart is empty.\n"; return false; }
        if (head->productID == id) {
            CartNode* tmp = head;
            head = head->next;
            delete tmp; itemCount--;
            cout << "[Cart] Item removed.\n";
            return true;
        }
        CartNode* prev = head, *cur = head->next;
        while (cur) {
            if (cur->productID == id) {
                prev->next = cur->next;
                delete cur; itemCount--;
                cout << "[Cart] Item removed.\n";
                return true;
            }
            prev = cur; cur = cur->next;
        }
        cout << "[Cart] Product ID not found in cart.\n";
        return false;
    }

    void viewCart() const {
        if (!head) { cout << "Your cart is empty.\n"; return; }
        cout << "\n========== SHOPPING CART ==========\n";
        cout << left;
        cout.width(6);  cout << "ID";
        cout.width(22); cout << "Product";
        cout.width(8);  cout << "Qty";
        cout.width(10); cout << "Price";
        cout << "Subtotal\n";
        cout << string(55, '-') << "\n";
        double total = 0;
        CartNode* cur = head;
        while (cur) {
            double sub = cur->quantity * cur->price;
            total += sub;
            cout.width(6);  cout << cur->productID;
            cout.width(22); cout << cur->productName;
            cout.width(8);  cout << cur->quantity;
            cout.width(10); cout << cur->price;
            cout << "$" << sub << "\n";
            cur = cur->next;
        }
        cout << string(55, '-') << "\n";
        cout << "Total: $" << total << "\n";
    }

    double getTotal() const {
        double total = 0;
        CartNode* cur = head;
        while (cur) { total += cur->quantity * cur->price; cur = cur->next; }
        return total;
    }

    // Build a product list string for order record
    void getProductListString(char* buf, int bufSize) const {
        buf[0] = '\0';
        CartNode* cur = head;
        while (cur) {
            strncat(buf, cur->productName, bufSize - strlen(buf) - 1);
            if (cur->next) strncat(buf, ",", bufSize - strlen(buf) - 1);
            cur = cur->next;
        }
    }

    void clearCart() {
        while (head) {
            CartNode* tmp = head;
            head = head->next;
            delete tmp;
        }
        itemCount = 0;
    }

    bool isEmpty() const { return head == nullptr; }
    int  getItemCount() const { return itemCount; }
};

// ─────────────────────────────────────────────
//  DOUBLY LINKED LIST  (Order History)
// ─────────────────────────────────────────────
struct HistoryNode {
    int    orderID;
    char   productList[256];
    double totalPrice;
    char   date[20];
    HistoryNode* prev;
    HistoryNode* next;

    HistoryNode(int oid, const char* pl, double tp, const char* dt)
        : orderID(oid), totalPrice(tp), prev(nullptr), next(nullptr) {
        strncpy(productList, pl, 255);
        strncpy(date, dt, 19);
    }
};

class OrderHistory {
    HistoryNode* head;
    HistoryNode* tail;
    int          count;

public:
    OrderHistory() : head(nullptr), tail(nullptr), count(0) {}

    ~OrderHistory() {
        HistoryNode* cur = head;
        while (cur) { HistoryNode* tmp = cur->next; delete cur; cur = tmp; }
    }

    void addOrder(int oid, const char* pl, double tp, const char* date) {
        HistoryNode* node = new HistoryNode(oid, pl, tp, date);
        if (!tail) { head = tail = node; }
        else { tail->next = node; node->prev = tail; tail = node; }
        count++;
    }

    void viewHistory() const {
        if (!head) { cout << "No order history.\n"; return; }
        cout << "\n===== ORDER HISTORY (newest last) =====\n";
        HistoryNode* cur = head;
        while (cur) {
            cout << "Order #" << cur->orderID
                 << " | Date: " << cur->date
                 << " | Products: " << cur->productList
                 << " | Total: $" << cur->totalPrice << "\n";
            cur = cur->next;
        }
    }

    void viewHistoryReverse() const {
        if (!tail) { cout << "No order history.\n"; return; }
        cout << "\n===== ORDER HISTORY (newest first) =====\n";
        HistoryNode* cur = tail;
        while (cur) {
            cout << "Order #" << cur->orderID
                 << " | Date: " << cur->date
                 << " | Products: " << cur->productList
                 << " | Total: $" << cur->totalPrice << "\n";
            cur = cur->prev;
        }
    }

    int getCount() const { return count; }
};

// ─────────────────────────────────────────────
//  CIRCULAR LINKED LIST  (Recently Viewed)
// ─────────────────────────────────────────────
#define RECENT_LIMIT 5   // store last 5 viewed products

struct RecentNode {
    int  productID;
    char productName[50];
    RecentNode* next;

    RecentNode(int id, const char* name) : productID(id), next(nullptr) {
        strncpy(productName, name, 49);
    }
};

class RecentlyViewed {
    RecentNode* tail;   // tail->next = head (circular)
    int         count;

public:
    RecentlyViewed() : tail(nullptr), count(0) {}

    ~RecentlyViewed() {
        if (!tail) return;
        RecentNode* head = tail->next;
        tail->next = nullptr;
        RecentNode* cur = head;
        while (cur) { RecentNode* tmp = cur->next; delete cur; cur = tmp; }
    }

    void addProduct(int id, const char* name) {
        // Avoid duplicates — search and skip
        if (tail) {
            RecentNode* cur = tail->next;
            do {
                if (cur->productID == id) return;
                cur = cur->next;
            } while (cur != tail->next);
        }

        RecentNode* node = new RecentNode(id, name);
        if (!tail) {
            tail = node;
            tail->next = tail;  // point to itself
        } else {
            node->next = tail->next;   // new node points to head
            tail->next = node;         // old tail points to new node
            tail = node;               // update tail
        }
        count++;

        // Remove oldest if over limit
        if (count > RECENT_LIMIT) {
            RecentNode* oldHead = tail->next;
            tail->next = oldHead->next;
            delete oldHead;
            count--;
        }
    }

    void display() const {
        if (!tail) { cout << "No recently viewed products.\n"; return; }
        cout << "\n--- Recently Viewed Products ---\n";
        RecentNode* cur = tail->next;  // start from head
        for (int i = 0; i < count; i++) {
            cout << "  [" << cur->productID << "] " << cur->productName << "\n";
            cur = cur->next;
        }
    }

    int getCount() const { return count; }
};

#endif
