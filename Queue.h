#ifndef QUEUE_H
#define QUEUE_H

// ============================================================
//  Queue (Array-based, Circular) — O(1) enqueue/dequeue
//  Used by: Order Processing System (FIFO)
// ============================================================

#include <iostream>
#include <cstring>
using namespace std;

#define QUEUE_MAX 200

struct Order {
    int    orderID;
    int    customerID;
    char   productList[256];   // comma-separated product names
    double totalPrice;
    char   status[20];         // "Pending", "Processing", "Shipped", "Delivered"
};

class OrderQueue {
    Order data[QUEUE_MAX];
    int   front, rear, count;
    int   nextOrderID;

public:
    OrderQueue() : front(0), rear(-1), count(0), nextOrderID(1001) {}

    bool isEmpty() const { return count == 0; }
    bool isFull()  const { return count == QUEUE_MAX; }
    int  size()    const { return count; }

    // Enqueue a new order
    bool enqueue(int custID, const char* products, double price) {
        if (isFull()) { cout << "[Queue] Order queue full!\n"; return false; }
        rear = (rear + 1) % QUEUE_MAX;
        data[rear].orderID    = nextOrderID++;
        data[rear].customerID = custID;
        strncpy(data[rear].productList, products, 255);
        data[rear].totalPrice = price;
        strncpy(data[rear].status, "Pending", 19);
        count++;
        cout << "[Order] Order #" << data[rear].orderID << " placed successfully!\n";
        return true;
    }

    // Process (dequeue) front order
    bool processNext() {
        if (isEmpty()) { cout << "[Queue] No pending orders!\n"; return false; }
        Order& o = data[front];
        cout << "\n--- Processing Order #" << o.orderID << " ---\n";
        cout << "Customer ID : " << o.customerID << "\n";
        cout << "Products    : " << o.productList << "\n";
        cout << "Total Price : $" << o.totalPrice << "\n";
        strncpy(o.status, "Processing", 19);
        cout << "Status updated to: Processing\n";
        front = (front + 1) % QUEUE_MAX;
        count--;
        return true;
    }

    // Peek at front order without removing
    void peekFront() const {
        if (isEmpty()) { cout << "[Queue] No orders in queue.\n"; return; }
        const Order& o = data[front];
        cout << "Next Order -> #" << o.orderID
             << " | Customer: " << o.customerID
             << " | $" << o.totalPrice
             << " | " << o.status << "\n";
    }

    // View all pending orders
    void viewAll() const {
        if (isEmpty()) { cout << "No pending orders.\n"; return; }
        cout << "\n========== PENDING ORDERS ==========\n";
        cout << left;
        cout.width(8);  cout << "OrderID";
        cout.width(12); cout << "CustID";
        cout.width(30); cout << "Products";
        cout.width(12); cout << "Price";
        cout << "Status\n";
        cout << string(75, '-') << "\n";
        for (int i = 0; i < count; i++) {
            const Order& o = data[(front + i) % QUEUE_MAX];
            cout.width(8);  cout << o.orderID;
            cout.width(12); cout << o.customerID;
            cout.width(30); cout << o.productList;
            cout.width(12); cout << o.totalPrice;
            cout << o.status << "\n";
        }
    }
};

#endif
