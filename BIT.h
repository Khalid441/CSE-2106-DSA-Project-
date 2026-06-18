#ifndef BIT_H
#define BIT_H

// ============================================================
//  Binary Indexed Tree / Fenwick Tree
//  Purpose: Efficient prefix sum queries and point updates
//  Use Case: Sales analytics — query total sales in ID range
//  Time: O(log n) update and query, O(n log n) build
// ============================================================

#include <iostream>
using namespace std;

#define BIT_SIZE 512   // supports product IDs up to 512

class BIT {
    int tree[BIT_SIZE + 1];
    int n;

    // Extract lowest set bit
    int lowbit(int i) const { return i & (-i); }

public:
    BIT(int maxN = BIT_SIZE) : n(maxN) {
        for (int i = 0; i <= n; i++) tree[i] = 0;
    }

    // Point update: add delta to position i (1-indexed)
    void update(int i, int delta) {
        if (i <= 0 || i > n) { cout << "[BIT] Index out of range.\n"; return; }
        for (; i <= n; i += lowbit(i))
            tree[i] += delta;
    }

    // Prefix sum: sum of [1..i]
    int prefixSum(int i) const {
        if (i <= 0) return 0;
        if (i > n) i = n;
        int s = 0;
        for (; i > 0; i -= lowbit(i))
            s += tree[i];
        return s;
    }

    // Range sum: sum of [l..r]
    int rangeSum(int l, int r) const {
        if (l > r) return 0;
        return prefixSum(r) - prefixSum(l - 1);
    }

    // Initialize from a sales array (1-indexed mapping from product IDs)
    // productIDs are mapped to [1..n] by offset
    void buildFromSales(int productIDs[], int salesArr[], int count, int idOffset) {
        // Reset
        for (int i = 0; i <= n; i++) tree[i] = 0;
        for (int i = 0; i < count; i++) {
            int pos = productIDs[i] - idOffset + 1;
            if (pos > 0 && pos <= n) update(pos, salesArr[i]);
        }
    }

    // Display prefix sums for first k entries
    void displayPrefixSums(int k, int idOffset) const {
        cout << "\n--- BIT Prefix Sums (Sales) ---\n";
        for (int i = 1; i <= k && i <= n; i++) {
            cout << "  Total sales for product IDs up to "
                 << (i + idOffset - 1) << ": " << prefixSum(i) << "\n";
        }
    }

    void querySalesRange(int fromID, int toID, int idOffset) const {
        int l = fromID - idOffset + 1;
        int r = toID   - idOffset + 1;
        int result = rangeSum(l, r);
        cout << "\n[BIT] Total sales for product IDs "
             << fromID << " to " << toID << ": " << result << "\n";
    }
};

#endif
