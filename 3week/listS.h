#ifndef LISTS_H
#define LISTS_H

#define MAX 10

int insertElement(int L[], int size, int x);
int deleteElement(int L[], int size, int x);

int insertElement(int L[], int size, int x) {
    int i, move = 0;
    for (i = 0; i < size; i++) {
        if (L[i] > x) break;
    }
    for (int j = size; j > i; j--) {
        L[j] = L[j - 1];
        move++;
    }
    L[i] = x;
    return move;
}

int deleteElement(int L[], int size, int x) {
    int i, move = 0;
    for (i = 0; i < size; i++) {
        if (L[i] == x) break;
    }
    if (i == size) return size;

    for (int j = i; j < size - 1; j++) {
        L[j] = L[j + 1];
        move++;
    }
    return move;
}

#endif
