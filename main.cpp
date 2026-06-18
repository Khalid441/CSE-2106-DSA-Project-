// ============================================================
//  SMART E-COMMERCE MANAGEMENT SYSTEM
//  Using Data Structures and Algorithms (Manual Implementations)
// ============================================================
//
//  PROJECT REPORT
//  ==============
//  Objective:
//    Simulate a real-world e-commerce platform demonstrating
//    practical applications of core DSA concepts.
//
//  DSA Concepts Used:
//    - Array           : Product storage, sorting algorithms
//    - Singly LL       : Shopping cart
//    - Doubly LL       : Order history (forward/backward traversal)
//    - Circular LL     : Recently viewed products
//    - Stack (array)   : Infix→Postfix, discount calculation
//    - Queue (circular): Order processing (FIFO)
//    - BST             : Product search/insert/delete by ID
//    - Max/Min Heap    : Top/Bottom selling products
//    - Fenwick Tree    : Sales prefix/range queries
//    - Graph (Adj List): Product recommendations (BFS/DFS)
//    - DAG + Topo Sort : Delivery pipeline ordering
//    - Sorting Algos   : Bubble, Selection, Insertion,
//                        Quick, Merge, Heap Sort
//    - Binary Search   : O(log n) product/customer lookup
//
//  Real-world Applications:
//    Amazon/Flipkart style inventory, cart, order management,
//    recommendation engine, delivery tracking pipeline.
//
//  Language : C++  |  Type: Console / Menu-driven
// ============================================================

#include <iostream>
#include <cstring>
#include <cstdio>
#include "include/Product.h"
#include "include/Customer.h"
#include "include/Queue.h"
#include "include/BST.h"
#include "include/Heap.h"
#include "include/BIT.h"
#include "include/Graph.h"
#include "include/Topo.h"
#include "include/Discount.h"

using namespace std;

// ─────────────────────────────────────────────────────────
//  Global system objects
// ─────────────────────────────────────────────────────────
ProductManager  productMgr;
CustomerManager customerMgr;
OrderQueue      orderQueue;
BST             bst;
MaxHeap         maxHeap;
MinHeap         minHeap;
BIT             fenwick;
Graph           graph;
TopologicalSort topo;
DiscountCalculator discCalc;

// ─────────────────────────────────────────────────────────
//  Utility helpers
// ─────────────────────────────────────────────────────────
void clearScreen()  { cout << "\n" << string(60, '=') << "\n"; }
void pressEnter()   { cout << "\nPress Enter to continue..."; cin.ignore(); cin.get(); }

int readInt(const char* prompt) {
    int v; cout << prompt; cin >> v;
    while (cin.fail()) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "  Invalid. " << prompt; cin >> v;
    }
    return v;
}

double readDouble(const char* prompt) {
    double v; cout << prompt; cin >> v;
    while (cin.fail()) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "  Invalid. " << prompt; cin >> v;
    }
    return v;
}

// ─────────────────────────────────────────────────────────
//  Initialize BST, Heap, BIT from loaded products
// ─────────────────────────────────────────────────────────
void initDataStructures() {
    Product* arr = productMgr.getArray();
    int n = productMgr.getCount();

    // BST
    for (int i = 0; i < n; i++)
        bst.insert(arr[i].id, arr[i].name, arr[i].price, arr[i].salesCount, arr[i].rating);

    // Heaps
    int ids[MAX_PRODUCTS];
    char names[MAX_PRODUCTS][50];
    int sales[MAX_PRODUCTS];
    double prices[MAX_PRODUCTS];
    for (int i = 0; i < n; i++) {
        ids[i]   = arr[i].id;
        strncpy(names[i], arr[i].name, 49);
        sales[i]  = arr[i].salesCount;
        prices[i] = arr[i].price;
    }
    maxHeap.buildFromArray(ids, names, sales, prices, n);
    minHeap.buildFromArray(ids, names, sales, prices, n);

    // BIT — map product IDs starting from 101
    int pids[MAX_PRODUCTS], sl[MAX_PRODUCTS];
    for (int i = 0; i < n; i++) { pids[i] = arr[i].id; sl[i] = arr[i].salesCount; }
    fenwick.buildFromSales(pids, sl, n, 101);
}

// ─────────────────────────────────────────────────────────
//  MODULE MENUS
// ─────────────────────────────────────────────────────────

// ── 1. Product Management ────────────────────────────────
void productMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  PRODUCT MANAGEMENT\n" << string(60,'=') << "\n";
        cout << " 1. View All Products\n";
        cout << " 2. Add Product (append)\n";
        cout << " 3. Insert at Beginning\n";
        cout << " 4. Insert at Position\n";
        cout << " 5. Delete Product by ID\n";
        cout << " 6. Delete from Beginning\n";
        cout << " 7. Delete from End\n";
        cout << " 8. Update Product\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        if (ch == 1) {
            productMgr.viewAll();
        }
        else if (ch == 2) {
            char name[50]; double price; int stock;
            cout << "Name: "; cin.ignore(); cin.getline(name,49);
            price = readDouble("Price: $"); stock = readInt("Stock: ");
            productMgr.addProduct(name, price, stock);
        }
        else if (ch == 3) {
            char name[50]; double price; int stock;
            cout << "Name: "; cin.ignore(); cin.getline(name,49);
            price = readDouble("Price: $"); stock = readInt("Stock: ");
            productMgr.insertAtBeginning(name, price, stock);
        }
        else if (ch == 4) {
            int pos;
            char name[50]; double price; int stock;
            pos = readInt("Position (0-indexed): ");
            cout << "Name: "; cin.ignore(); cin.getline(name,49);
            price = readDouble("Price: $"); stock = readInt("Stock: ");
            productMgr.insertAtPosition(pos, name, price, stock);
        }
        else if (ch == 5) {
            int id = readInt("Product ID to delete: ");
            productMgr.deleteProduct(id);
        }
        else if (ch == 6) { productMgr.deleteFromBeginning(); }
        else if (ch == 7) { productMgr.deleteFromEnd(); }
        else if (ch == 8) {
            int id = readInt("Product ID to update: ");
            double np = readDouble("New Price: $");
            int ns    = readInt("New Stock: ");
            productMgr.updateProduct(id, np, ns);
        }

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ── 2. Customer Management ───────────────────────────────
void customerMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  CUSTOMER MANAGEMENT\n" << string(60,'=') << "\n";
        cout << " 1. View All Customers\n";
        cout << " 2. Register New Customer\n";
        cout << " 3. Search Customer by ID\n";
        cout << " 4. View Customer Cart\n";
        cout << " 5. Add Item to Customer Cart\n";
        cout << " 6. Remove Item from Customer Cart\n";
        cout << " 7. View Order History\n";
        cout << " 8. View Recently Viewed Products\n";
        cout << " 9. Mark Product as Viewed\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        if (ch == 1) { customerMgr.viewAll(); }
        else if (ch == 2) {
            char name[50], phone[15], addr[100];
            cout << "Name: "; cin.ignore(); cin.getline(name,49);
            cout << "Phone: "; cin.getline(phone,14);
            cout << "Address: "; cin.getline(addr,99);
            customerMgr.addCustomer(name, phone, addr);
        }
        else if (ch == 3) {
            int id = readInt("Customer ID: ");
            customerMgr.searchByID(id);
        }
        else if (ch == 4) {
            int cid = readInt("Customer ID: ");
            Customer* c = customerMgr.findByID(cid);
            if (!c) cout << "Customer not found.\n";
            else c->cart.viewCart();
        }
        else if (ch == 5) {
            int cid = readInt("Customer ID: ");
            Customer* c = customerMgr.findByID(cid);
            if (!c) { cout << "Customer not found.\n"; }
            else {
                int pid = readInt("Product ID: ");
                Product* p = productMgr.findByID(pid);
                if (!p) { cout << "Product not found.\n"; }
                else {
                    int qty = readInt("Quantity: ");
                    c->cart.addItem(p->id, p->name, qty, p->price);
                    c->recentlyViewed.addProduct(p->id, p->name);
                }
            }
        }
        else if (ch == 6) {
            int cid = readInt("Customer ID: ");
            Customer* c = customerMgr.findByID(cid);
            if (!c) cout << "Customer not found.\n";
            else {
                int pid = readInt("Product ID to remove: ");
                c->cart.removeItem(pid);
            }
        }
        else if (ch == 7) {
            int cid = readInt("Customer ID: ");
            Customer* c = customerMgr.findByID(cid);
            if (!c) cout << "Customer not found.\n";
            else {
                int dir = readInt("View 1=Oldest first  2=Newest first: ");
                if (dir == 1) c->history.viewHistory();
                else          c->history.viewHistoryReverse();
            }
        }
        else if (ch == 8) {
            int cid = readInt("Customer ID: ");
            Customer* c = customerMgr.findByID(cid);
            if (!c) cout << "Customer not found.\n";
            else c->recentlyViewed.display();
        }
        else if (ch == 9) {
            int cid = readInt("Customer ID: ");
            Customer* c = customerMgr.findByID(cid);
            if (!c) { cout << "Customer not found.\n"; }
            else {
                int pid = readInt("Product ID: ");
                Product* p = productMgr.findByID(pid);
                if (!p) cout << "Product not found.\n";
                else c->recentlyViewed.addProduct(p->id, p->name);
            }
        }

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ── 3. Shopping Cart (standalone checkout) ───────────────
void cartCheckoutMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  SHOPPING CART / CHECKOUT\n" << string(60,'=') << "\n";
        cout << " 1. Add Item to Cart\n";
        cout << " 2. Remove Item from Cart\n";
        cout << " 3. View Cart\n";
        cout << " 4. Checkout (Place Order)\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        int cid = 201;  // default to first customer for demo; real app would use session
        if (ch >= 1 && ch <= 4) cid = readInt("Customer ID: ");
        Customer* c = (cid > 0) ? customerMgr.findByID(cid) : nullptr;

        if (!c && ch >= 1 && ch <= 4) { cout << "Customer not found.\n"; pressEnter(); continue; }

        if (ch == 1) {
            int pid = readInt("Product ID: ");
            Product* p = productMgr.findByID(pid);
            if (!p) cout << "Product not found.\n";
            else { int qty = readInt("Qty: "); c->cart.addItem(p->id, p->name, qty, p->price); }
        }
        else if (ch == 2) {
            int pid = readInt("Product ID to remove: ");
            c->cart.removeItem(pid);
        }
        else if (ch == 3) { c->cart.viewCart(); }
        else if (ch == 4) {
            c->cart.viewCart();
            if (c->cart.isEmpty()) { cout << "Cart is empty.\n"; }
            else {
                cout << "\nConfirm checkout? (1=Yes 0=No): ";
                int confirm; cin >> confirm;
                if (confirm == 1) {
                    char prodList[256];
                    c->cart.getProductListString(prodList, 255);
                    double total = c->cart.getTotal();
                    orderQueue.enqueue(cid, prodList, total);
                    // Add to order history
                    static int oid = 5001;
                    c->history.addOrder(oid++, prodList, total, "2026-05-23");
                    c->cart.clearCart();
                    cout << "[Checkout] Order placed! Cart cleared.\n";
                }
            }
        }

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ── 4. Order Processing ──────────────────────────────────
void orderMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  ORDER PROCESSING (FIFO Queue)\n" << string(60,'=') << "\n";
        cout << " 1. View All Pending Orders\n";
        cout << " 2. Process Next Order\n";
        cout << " 3. Peek at Next Order\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        if (ch == 1)      orderQueue.viewAll();
        else if (ch == 2) orderQueue.processNext();
        else if (ch == 3) orderQueue.peekFront();

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ── 5. Sorting Products ──────────────────────────────────
void sortMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  SORTING PRODUCTS\n" << string(60,'=') << "\n";
        cout << " 1. Bubble Sort     — by Price\n";
        cout << " 2. Selection Sort  — by Sales\n";
        cout << " 3. Insertion Sort  — by Rating\n";
        cout << " 4. Quick Sort      — choose field\n";
        cout << " 5. Merge Sort      — choose field\n";
        cout << " 6. Heap Sort       — choose field\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        if (ch >= 1 && ch <= 3) {
            productMgr.sortAndDisplay(ch, ch);   // algo and field aligned for 1-3
        }
        else if (ch >= 4 && ch <= 6) {
            cout << "Sort by: 1=Price  2=Sales  3=Rating: ";
            int field; cin >> field;
            if (field < 1 || field > 3) field = 1;
            productMgr.sortAndDisplay(ch, field);
        }

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ── 6. Searching Products ────────────────────────────────
void searchMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  SEARCHING\n" << string(60,'=') << "\n";
        cout << " 1. Binary Search — Product by ID\n";
        cout << " 2. Binary Search — Customer by ID\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        if (ch == 1) {
            int id = readInt("Product ID: ");
            productMgr.searchByID(id);
        }
        else if (ch == 2) {
            int id = readInt("Customer ID: ");
            customerMgr.searchByID(id);
        }

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ── 7. BST Operations ────────────────────────────────────
void bstMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  BST OPERATIONS\n" << string(60,'=') << "\n";
        cout << " 1. Insert Product into BST\n";
        cout << " 2. Search Product by ID\n";
        cout << " 3. Delete Product from BST\n";
        cout << " 4. Inorder Traversal  (Sorted)\n";
        cout << " 5. Preorder Traversal\n";
        cout << " 6. Postorder Traversal\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        if (ch == 1) {
            int id = readInt("Product ID: "); char name[50];
            cout << "Name: "; cin.ignore(); cin.getline(name, 49);
            double price = readDouble("Price: $");
            int sales    = readInt("Sales Count: ");
            double rating = readDouble("Rating (0-5): ");
            bst.insert(id, name, price, sales, rating);
        }
        else if (ch == 2) { int id = readInt("Product ID: "); bst.search(id); }
        else if (ch == 3) { int id = readInt("Product ID: "); bst.remove(id); }
        else if (ch == 4) { bst.inorder(); }
        else if (ch == 5) { bst.preorder(); }
        else if (ch == 6) { bst.postorder(); }

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ── 8. Heap Operations ───────────────────────────────────
void heapMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  HEAP OPERATIONS\n" << string(60,'=') << "\n";
        cout << " 1. Show Top 5 Best-Selling Products   (Max Heap)\n";
        cout << " 2. Show Top 3 Lowest-Selling Products (Min Heap)\n";
        cout << " 3. Extract Max Sales Product\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        if (ch == 1) { maxHeap.displayTopN(5); }
        else if (ch == 2) { minHeap.displayBottomN(3); }
        else if (ch == 3) {
            MaxHeap copy = maxHeap;
            HeapProduct top = copy.extractMax();
            cout << "\n[Max Heap] Best seller: [" << top.productID << "] "
                 << top.productName << " | Sales: " << top.salesCount << "\n";
        }

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ── 9. Discount Calculator ───────────────────────────────
void discountMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  DISCOUNT CALCULATOR (Stack)\n" << string(60,'=') << "\n";
        cout << " 1. Demo Built-in Expressions\n";
        cout << " 2. Calculate Product Discount\n";
        cout << " 3. Evaluate Custom Expression\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        if (ch == 1)      discCalc.demo();
        else if (ch == 2) {
            double price = readDouble("Product Price: $");
            double disc  = readDouble("Discount (%): ");
            discCalc.calculateProductDiscount(price, disc);
        }
        else if (ch == 3) discCalc.interactiveEvaluator();

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ── 10. Sales Analytics (BIT) ────────────────────────────
void analyticsMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  SALES ANALYTICS (Fenwick/BIT)\n" << string(60,'=') << "\n";
        cout << " 1. View Prefix Sums (first 10 products)\n";
        cout << " 2. Query Sales Range (by Product ID)\n";
        cout << " 3. Update Sales Count for a Product\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        if (ch == 1) { fenwick.displayPrefixSums(10, 101); }
        else if (ch == 2) {
            int from = readInt("From Product ID: ");
            int to   = readInt("To Product ID: ");
            fenwick.querySalesRange(from, to, 101);
        }
        else if (ch == 3) {
            int pid   = readInt("Product ID: ");
            int delta = readInt("Add to sales count: ");
            fenwick.update(pid - 101 + 1, delta);
            cout << "[BIT] Updated sales for product " << pid << " by +" << delta << "\n";
        }

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ── 11. Recommendation System (Graph) ────────────────────
void recommendMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  RECOMMENDATION SYSTEM (Graph)\n" << string(60,'=') << "\n";
        cout << " 1. Display Product Graph\n";
        cout << " 2. BFS Traversal\n";
        cout << " 3. DFS Traversal\n";
        cout << " 4. Get Product Recommendations\n";
        cout << " 5. Add New Product Node\n";
        cout << " 6. Add Product Relationship (Edge)\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        if (ch == 1)      graph.displayGraph();
        else if (ch == 2) { int s = readInt("Start vertex: "); graph.bfs(s); }
        else if (ch == 3) { int s = readInt("Start vertex: "); graph.dfs(s); }
        else if (ch == 4) {
            cout << "Vertices (0=" << graph.getVertexName(0) << ", 1=Mouse, 2=Keyboard, ...)\n";
            int v = readInt("Product vertex: ");
            graph.recommend(v);
        }
        else if (ch == 5) {
            char name[50]; cout << "Product name: "; cin.ignore(); cin.getline(name, 49);
            graph.addVertex(name);
        }
        else if (ch == 6) {
            int u = readInt("Vertex u: "), v = readInt("Vertex v: ");
            graph.addEdge(u, v);
        }

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ── 12. Topological Sort ─────────────────────────────────
void topoMenu() {
    int ch;
    do {
        clearScreen();
        cout << "  TOPOLOGICAL SORT (Delivery Pipeline)\n" << string(60,'=') << "\n";
        cout << " 1. Display DAG (Delivery Pipeline)\n";
        cout << " 2. Perform Topological Sort\n";
        cout << " 3. Add Custom Stage\n";
        cout << " 4. Add Dependency Edge\n";
        cout << " 0. Back\n";
        cout << string(60,'-') << "\n";
        ch = readInt("Choice: ");

        if (ch == 1)      topo.displayDAG();
        else if (ch == 2) topo.performSort();
        else if (ch == 3) {
            char name[50]; cout << "Stage name: "; cin.ignore(); cin.getline(name, 49);
            int v = topo.addNode(name);
            cout << "New node index: " << v << "\n";
        }
        else if (ch == 4) {
            int u = readInt("From stage index: ");
            int v = readInt("To stage index: ");
            topo.addDirectedEdge(u, v);
        }

        if (ch != 0) pressEnter();
    } while (ch != 0);
}

// ─────────────────────────────────────────────────────────
//  MAIN MENU
// ─────────────────────────────────────────────────────────
void printBanner() {
    cout << "\n";
    cout << "  ╔══════════════════════════════════════════════════════╗\n";
    cout << "  ║     SMART E-COMMERCE MANAGEMENT SYSTEM              ║\n";
    cout << "  ║     Using Data Structures and Algorithms            ║\n";
    cout << "  ╚══════════════════════════════════════════════════════╝\n";
    cout << "\n";
    cout << "   1.  Product Management      (Array, Sorting)\n";
    cout << "   2.  Customer Management     (Linked Lists)\n";
    cout << "   3.  Shopping Cart/Checkout  (Singly LL + Queue)\n";
    cout << "   4.  Order Processing        (Queue - FIFO)\n";
    cout << "   5.  Sort Products           (6 Algorithms)\n";
    cout << "   6.  Search Products/Customers (Binary Search)\n";
    cout << "   7.  BST Operations\n";
    cout << "   8.  Heap Operations         (Max/Min Heap)\n";
    cout << "   9.  Discount Calculator     (Stack - Infix/Postfix)\n";
    cout << "  10.  Sales Analytics         (Fenwick/BIT Tree)\n";
    cout << "  11.  Recommendation System   (Graph BFS/DFS)\n";
    cout << "  12.  Topological Sort        (DAG - Delivery Pipeline)\n";
    cout << "   0.  Exit\n";
    cout << "\n";
    cout << string(60, '-') << "\n";
}

int main() {
    cout << "\n[System] Initializing Smart E-Commerce System...\n";
    initDataStructures();
    cout << "[System] All data structures loaded with sample data.\n";

    int choice;
    do {
        printBanner();
        choice = readInt("Main Menu Choice: ");

        switch (choice) {
            case  1: productMenu();     break;
            case  2: customerMenu();    break;
            case  3: cartCheckoutMenu();break;
            case  4: orderMenu();       break;
            case  5: sortMenu();        break;
            case  6: searchMenu();      break;
            case  7: bstMenu();         break;
            case  8: heapMenu();        break;
            case  9: discountMenu();    break;
            case 10: analyticsMenu();   break;
            case 11: recommendMenu();   break;
            case 12: topoMenu();        break;
            case  0: cout << "\nThank you for using Smart E-Commerce System. Goodbye!\n\n"; break;
            default: cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
