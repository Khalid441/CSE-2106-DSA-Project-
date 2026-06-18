#ifndef DISCOUNT_H
#define DISCOUNT_H

// ============================================================
//  Discount Calculator
//  Uses Stack for:
//    1. Infix to Postfix conversion
//    2. Postfix expression evaluation
//  Supports: +  -  *  /  (  )  and numeric operands
// ============================================================

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "Stack.h"
using namespace std;

class DiscountCalculator {
    // Operator precedence
    int precedence(char op) const {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    bool isOperator(char c) const {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    // Convert infix string to postfix
    void infixToPostfix(const char* infix, char* postfix) const {
        CharStack ops;
        int j = 0;
        postfix[0] = '\0';

        for (int i = 0; infix[i] != '\0'; i++) {
            char c = infix[i];

            if (c == ' ') continue;  // skip spaces

            // Multi-digit number or decimal
            if (isdigit(c) || c == '.') {
                while (infix[i] && (isdigit(infix[i]) || infix[i] == '.')) {
                    postfix[j++] = infix[i++];
                }
                i--;  // step back one (loop will increment)
                postfix[j++] = ' ';
            }
            else if (c == '(') {
                ops.push(c);
            }
            else if (c == ')') {
                while (!ops.isEmpty() && ops.peek() != '(') {
                    postfix[j++] = ops.pop();
                    postfix[j++] = ' ';
                }
                ops.pop();  // remove '('
            }
            else if (isOperator(c)) {
                while (!ops.isEmpty() && ops.peek() != '(' &&
                       precedence(ops.peek()) >= precedence(c)) {
                    postfix[j++] = ops.pop();
                    postfix[j++] = ' ';
                }
                ops.push(c);
            }
        }

        while (!ops.isEmpty()) {
            postfix[j++] = ops.pop();
            postfix[j++] = ' ';
        }
        postfix[j] = '\0';
    }

    // Evaluate postfix expression
    double evaluatePostfix(const char* postfix) const {
        DoubleStack stk;
        int i = 0;

        while (postfix[i] != '\0') {
            char c = postfix[i];

            if (c == ' ') { i++; continue; }

            if (isdigit(c) || c == '.') {
                double num = 0, frac = 0, div = 1;
                bool hasDot = false;
                while (postfix[i] && (isdigit(postfix[i]) || postfix[i] == '.')) {
                    if (postfix[i] == '.') { hasDot = true; i++; continue; }
                    if (!hasDot) num = num * 10 + (postfix[i] - '0');
                    else { frac = frac * 10 + (postfix[i] - '0'); div *= 10; }
                    i++;
                }
                stk.push(num + frac / div);
                continue;
            }

            if (isOperator(c)) {
                double b = stk.pop(), a = stk.pop();
                double res = 0;
                switch (c) {
                    case '+': res = a + b; break;
                    case '-': res = a - b; break;
                    case '*': res = a * b; break;
                    case '/':
                        if (b == 0) { cout << "[Calc] Division by zero!\n"; return -1; }
                        res = a / b; break;
                }
                stk.push(res);
            }
            i++;
        }
        return stk.pop();
    }

public:
    // Evaluate an infix expression and return result
    double evaluate(const char* infix) const {
        char postfix[512];
        infixToPostfix(infix, postfix);
        cout << "\n[Discount Calc]\n";
        cout << "  Infix   : " << infix << "\n";
        cout << "  Postfix : " << postfix << "\n";
        double result = evaluatePostfix(postfix);
        cout << "  Result  : " << result << "\n";
        return result;
    }

    // Calculate discounted price for a product
    void calculateProductDiscount(double price, double discountPct) {
        cout << "\n--- Discount Calculation ---\n";
        cout << "Original Price : $" << price << "\n";
        cout << "Discount       : " << discountPct << "%\n";

        // Build expression string dynamically
        char expr[100];
        // Expression: price * (1 - discountPct/100)
        sprintf(expr, "%.2f * %.4f", price, (1.0 - discountPct / 100.0));
        double discounted = evaluate(expr);
        double savings    = price - discounted;
        cout << "You Save       : $" << savings << "\n";
        cout << "Final Price    : $" << discounted << "\n";
    }

    // Interactive discount expression evaluator
    void interactiveEvaluator() {
        cout << "\n=== Discount Expression Evaluator ===\n";
        cout << "Enter an infix expression (numbers and +-*/ operators).\n";
        cout << "Example: (1299.99 * 0.9) + 15.0\n";
        cout << "Expression: ";
        char expr[256];
        cin.ignore();
        cin.getline(expr, 255);
        double result = evaluate(expr);
        (void)result;  // result already printed in evaluate()
    }

    // Demo with built-in expressions
    void demo() {
        cout << "\n=== Discount Calculator Demo ===\n";
        const char* expressions[] = {
            "1299.99 * 0.9",
            "(1299.99 - 100) * 0.95",
            "((899.99 * 0.85) + 20) * 0.98",
            "149.99 * 0.7 + 5.99"
        };
        const char* descs[] = {
            "10% off Laptop Pro",
            "$100 off then 5% off Laptop",
            "15% off + tax then loyalty 2%",
            "30% off Headphones + shipping"
        };
        for (int i = 0; i < 4; i++) {
            cout << "\n[" << descs[i] << "]\n";
            evaluate(expressions[i]);
        }
    }
};

#endif
