#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_EXPR_LEN 256
#define MAX_STACK_SIZE 256

typedef struct {
    char items[MAX_STACK_SIZE];
    int top;
} CharStack;

typedef struct {
    double items[MAX_STACK_SIZE];
    int top;
} DoubleStack;

void initCharStack(CharStack* s) {
    s->top = -1;
}
int isCharStackEmpty(CharStack* s) {
    return s->top == -1;
}
char peekChar(CharStack* s) {
    return s->items[s->top];
}
void pushChar(CharStack* s, char item) {
    s->items[++s->top] = item;
}
char popChar(CharStack* s) {
    return s->items[s->top--];
}

void initDoubleStack(DoubleStack* s) {
    s->top = -1;
}
void pushDouble(DoubleStack* s, double item) {
    s->items[++s->top] = item;
}
double popDouble(DoubleStack* s) {
    return s->items[s->top--];
}

int precedence(char op) {
    switch (op) {
    case '^': return 3;
    case '*': case '/': return 2;
    case '+': case '-': return 1;
    default: return 0;
    }
}

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// 중위 수식을 후위 표기로 변환
void infixToPostfix(const char* expr, char* output) {
    CharStack opStack;
    initCharStack(&opStack);
    int i = 0, j = 0;
    while (expr[i]) {
        if (isspace(expr[i])) {
            i++;
            continue;
        }

        if (isdigit(expr[i]) || expr[i] == '.' || (expr[i] == '-' && (i == 0 || expr[i - 1] == '('))) {
            if (j > 0) output[j++] = ' ';
            if (expr[i] == '-') output[j++] = expr[i++];
            while (isdigit(expr[i]) || expr[i] == '.' || tolower(expr[i]) == 'e' || expr[i] == '+' || expr[i] == '-') {
                output[j++] = expr[i++];
                if (tolower(expr[i - 1]) == 'e' && (expr[i] == '+' || expr[i] == '-')) output[j++] = expr[i++];
            }
        }
        else if (expr[i] == '(') {
            pushChar(&opStack, expr[i++]);
        }
        else if (expr[i] == ')') {
            while (!isCharStackEmpty(&opStack) && peekChar(&opStack) != '(') {
                output[j++] = ' ';
                output[j++] = popChar(&opStack);
            }
            popChar(&opStack);
            i++;
        }
        else if (isOperator(expr[i])) {
            while (!isCharStackEmpty(&opStack) && precedence(peekChar(&opStack)) >= precedence(expr[i])) {
                output[j++] = ' ';
                output[j++] = popChar(&opStack);
            }
            pushChar(&opStack, expr[i++]);
        }
        else {
            i++;
        }
    }

    while (!isCharStackEmpty(&opStack)) {
        output[j++] = ' ';
        output[j++] = popChar(&opStack);
    }

    output[j] = '\0';
}

// 후위 수식 계산
double evaluatePostfix(const char* postfix) {
    DoubleStack numStack;
    initDoubleStack(&numStack);

    int i = 0;
    char token[MAX_EXPR_LEN];
    while (postfix[i]) {
        if (isspace(postfix[i])) {
            i++;
            continue;
        }

        if (isdigit(postfix[i]) || postfix[i] == '.' || postfix[i] == '-') {
            int j = 0;
            if (postfix[i] == '-') token[j++] = postfix[i++];
            while (isdigit(postfix[i]) || postfix[i] == '.' || tolower(postfix[i]) == 'e' || postfix[i] == '+' || postfix[i] == '-') {
                token[j++] = postfix[i++];
                if (tolower(postfix[i - 1]) == 'e' && (postfix[i] == '+' || postfix[i] == '-')) token[j++] = postfix[i++];
            }
            token[j] = '\0';
            pushDouble(&numStack, atof(token));
        }
        else if (isOperator(postfix[i])) {
            double b = popDouble(&numStack);
            double a = popDouble(&numStack);
            switch (postfix[i]) {
            case '+': pushDouble(&numStack, a + b); break;
            case '-': pushDouble(&numStack, a - b); break;
            case '*': pushDouble(&numStack, a * b); break;
            case '/': pushDouble(&numStack, a / b); break;
            case '^': pushDouble(&numStack, pow(a, b)); break;
            }
            i++;
        }
        else {
            i++;
        }
    }

    return popDouble(&numStack);
}

int main() {
    FILE* input = fopen("input.txt", "r");
    if (!input) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    int n;
    fscanf(input, "%d\n", &n);
    char expr[MAX_EXPR_LEN];
    char postfix[MAX_EXPR_LEN];

    for (int i = 0; i < n; i++) {
        fgets(expr, MAX_EXPR_LEN, input);
        expr[strcspn(expr, "\n")] = '\0';

        infixToPostfix(expr, postfix);
        double result = evaluatePostfix(postfix);

        printf("중위: %s\n후위: %s\n결과: %.6f\n\n", expr, postfix, result);
    }

    fclose(input);
    return 0;
}
