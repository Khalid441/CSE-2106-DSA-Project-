#ifndef STACK_H
#define STACK_H

// ============================================================
//  Stack (Array-based) — O(1) push/pop
//  Used by: Discount Calculator (infix→postfix, evaluation)
// ============================================================

#include <iostream>
#include <cstring>
using namespace std;

#define STACK_MAX 100

// --- Character Stack (for operator handling) ---
class CharStack {
    char data[STACK_MAX];
    int top;
public:
    CharStack() : top(-1) {}

    bool isEmpty() const { return top == -1; }
    bool isFull()  const { return top == STACK_MAX - 1; }

    void push(char c) {
        if (isFull()) { cout << "[Stack] Overflow!\n"; return; }
        data[++top] = c;
    }

    char pop() {
        if (isEmpty()) { cout << "[Stack] Underflow!\n"; return '\0'; }
        return data[top--];
    }

    char peek() const {
        if (isEmpty()) return '\0';
        return data[top];
    }
};

// --- Double Stack (for postfix evaluation) ---
class DoubleStack {
    double data[STACK_MAX];
    int top;
public:
    DoubleStack() : top(-1) {}

    bool isEmpty() const { return top == -1; }
    bool isFull()  const { return top == STACK_MAX - 1; }

    void push(double v) {
        if (isFull()) { cout << "[Stack] Overflow!\n"; return; }
        data[++top] = v;
    }

    double pop() {
        if (isEmpty()) { cout << "[Stack] Underflow!\n"; return 0.0; }
        return data[top--];
    }

    double peek() const {
        if (isEmpty()) return 0.0;
        return data[top];
    }
};

#endif
