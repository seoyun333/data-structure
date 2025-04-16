#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TERMS 100

typedef struct {
    int coef;
    int exp;
} Term;

typedef struct {
    Term terms[MAX_TERMS];
    int size;
} Polynomial;

// 문자열을 파싱하여 다항식을 생성
void parsePolynomial(char* str, Polynomial* poly) {
    poly->size = 0;
    char* token = strtok(str, " + ");
    while (token != NULL) {
        int coef, exp;
        if (sscanf_s(token, "%dx^%d", &coef, &exp) == 2) {
            poly->terms[poly->size].coef = coef;
            poly->terms[poly->size].exp = exp;
        }
        else if (sscanf_s(token, "%dx", &coef) == 1) {
            poly->terms[poly->size].coef = coef;
            poly->terms[poly->size].exp = 1;
        }
        else {
            sscanf_s(token, "%d", &coef);
            poly->terms[poly->size].coef = coef;
            poly->terms[poly->size].exp = 0;
        }
        poly->size++;
        token = strtok(NULL, " + ");
    }
}

// 다항식 출력 함수
void printPolynomial(Polynomial* poly) {
    for (int i = 0; i < poly->size; i++) {
        if (i > 0 && poly->terms[i].coef > 0) {
            printf(" + ");
        }
        if (poly->terms[i].exp == 0) {
            printf("%d", poly->terms[i].coef);
        }
        else if (poly->terms[i].exp == 1) {
            printf("%dx", poly->terms[i].coef);
        }
        else {
            printf("%dx^%d", poly->terms[i].coef, poly->terms[i].exp);
        }
    }
    printf("\n");
}

// 다항식 덧셈
Polynomial addPolynomials(Polynomial* p1, Polynomial* p2) {
    Polynomial result = { .size = 0 };
    int i = 0, j = 0;
    while (i < p1->size && j < p2->size) {
        if (p1->terms[i].exp > p2->terms[j].exp) {
            result.terms[result.size++] = p1->terms[i++];
        }
        else if (p1->terms[i].exp < p2->terms[j].exp) {
            result.terms[result.size++] = p2->terms[j++];
        }
        else {
            result.terms[result.size].coef = p1->terms[i].coef + p2->terms[j].coef;
            result.terms[result.size].exp = p1->terms[i].exp;
            result.size++;
            i++; j++;
        }
    }
    while (i < p1->size) result.terms[result.size++] = p1->terms[i++];
    while (j < p2->size) result.terms[result.size++] = p2->terms[j++];
    return result;
}

// 다항식 곱셈
Polynomial multiplyPolynomials(Polynomial* p1, Polynomial* p2) {
    Polynomial result = { .size = 0 };
    int temp[MAX_TERMS] = { 0 };

    for (int i = 0; i < p1->size; i++) {
        for (int j = 0; j < p2->size; j++) {
            int newExp = p1->terms[i].exp + p2->terms[j].exp;
            temp[newExp] += p1->terms[i].coef * p2->terms[j].coef;
        }
    }

    for (int i = MAX_TERMS - 1; i >= 0; i--) {
        if (temp[i] != 0) {
            result.terms[result.size].coef = temp[i];
            result.terms[result.size].exp = i;
            result.size++;
        }
    }
    return result;
}

int main1(void) {
    FILE* file = fopen("input.txt", "r");
    if (!file) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    int n;
    fscanf_s(file, "%d\n", &n);

    for (int i = 0; i < n; i++) {
        char poly1_str[100], poly2_str[100];
        fgets(poly1_str, sizeof(poly1_str), file);
        fgets(poly2_str, sizeof(poly2_str), file);

        poly1_str[strcspn(poly1_str, "\n")] = '\0';  // 개행 제거
        poly2_str[strcspn(poly2_str, "\n")] = '\0';

        Polynomial poly1, poly2;
        parsePolynomial(poly1_str, &poly1);
        parsePolynomial(poly2_str, &poly2);

        printf("== %d번째 다항식 쌍 ==\n", i + 1);
        printf("첫 번째: ");
        printPolynomial(&poly1);
        printf("두 번째: ");
        printPolynomial(&poly2);

        Polynomial sum = addPolynomials(&poly1, &poly2);
        Polynomial product = multiplyPolynomials(&poly1, &poly2);

        printf("덧셈 결과: ");
        printPolynomial(&sum);
        printf("곱셈 결과: ");
        printPolynomial(&product);
        printf("\n");
    }

    fclose(file);
    return 0;
}
