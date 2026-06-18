#ifndef CUSTOMER_H
#define CUSTOMER_H

// ============================================================
//  Customer Management Module
//  Each Customer owns: ShoppingCart, OrderHistory, RecentlyViewed
// ============================================================

#include <iostream>
#include <cstring>
#include "LinkedList.h"
using namespace std;

#define MAX_CUSTOMERS 50

struct Customer {
    int           id;
    char          name[50];
    char          phone[15];
    char          address[100];
    bool          active;
    ShoppingCart  cart;
    OrderHistory  history;
    RecentlyViewed recentlyViewed;
};

class CustomerManager {
    Customer customers[MAX_CUSTOMERS];
    int      count;
    int      nextID;

    void printHeader() const {
        cout << left;
        cout.width(6);  cout << "ID";
        cout.width(20); cout << "Name";
        cout.width(15); cout << "Phone";
        cout << "Address\n";
        cout << string(65, '-') << "\n";
    }

    void printCustomer(const Customer& c) const {
        cout.width(6);  cout << c.id;
        cout.width(20); cout << c.name;
        cout.width(15); cout << c.phone;
        cout << c.address << "\n";
    }

public:
    CustomerManager() : count(0), nextID(201) { loadSampleData(); }

    void loadSampleData() {
        const char* names[]   = {"Alice Johnson","Bob Smith","Carol Davis","David Wilson","Eve Martinez"};
        const char* phones[]  = {"555-0101","555-0202","555-0303","555-0404","555-0505"};
        const char* addrs[]   = {"123 Main St","456 Oak Ave","789 Pine Rd","321 Elm Blvd","654 Maple Dr"};
        for (int i = 0; i < 5; i++) {
            Customer& c = customers[count++];
            c.id = nextID++;
            strncpy(c.name, names[i], 49);
            strncpy(c.phone, phones[i], 14);
            strncpy(c.address, addrs[i], 99);
            c.active = true;
        }
    }

    bool addCustomer(const char* name, const char* phone, const char* addr) {
        if (count >= MAX_CUSTOMERS) { cout << "Customer storage full!\n"; return false; }
        Customer& c = customers[count++];
        c.id = nextID++;
        strncpy(c.name, name, 49);
        strncpy(c.phone, phone, 14);
        strncpy(c.address, addr, 99);
        c.active = true;
        cout << "[Customer] Registered: " << name << " (ID: " << c.id << ")\n";
        return true;
    }

    void viewAll() const {
        cout << "\n========== CUSTOMERS ==========\n";
        printHeader();
        for (int i = 0; i < count; i++)
            if (customers[i].active) printCustomer(customers[i]);
    }

    Customer* findByID(int id) {
        for (int i = 0; i < count; i++)
            if (customers[i].id == id && customers[i].active) return &customers[i];
        return nullptr;
    }

    // Binary search for customer by ID
    void searchByID(int id) {
        // Sort a temp array of IDs for binary search
        int ids[MAX_CUSTOMERS], idx_map[MAX_CUSTOMERS];
        int n = 0;
        for (int i = 0; i < count; i++) {
            if (customers[i].active) { ids[n] = customers[i].id; idx_map[n] = i; n++; }
        }
        // Insertion sort by ID
        for (int i = 1; i < n; i++) {
            int kid = ids[i], km = idx_map[i]; int j = i - 1;
            while (j >= 0 && ids[j] > kid) { ids[j+1]=ids[j]; idx_map[j+1]=idx_map[j]; j--; }
            ids[j+1] = kid; idx_map[j+1] = km;
        }
        // Binary search
        int lo = 0, hi = n - 1, found = -1;
        while (lo <= hi) {
            int mid = (lo+hi)/2;
            if (ids[mid] == id) { found = idx_map[mid]; break; }
            else if (ids[mid] < id) lo = mid + 1;
            else hi = mid - 1;
        }
        if (found == -1) { cout << "[Search] Customer ID " << id << " not found.\n"; return; }
        cout << "\n[Search] Found:\n";
        printHeader(); printCustomer(customers[found]);
    }

    int getCount() const { return count; }
};

#endif
