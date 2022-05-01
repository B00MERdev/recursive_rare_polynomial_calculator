/** @file
  Realizacja struktur danych kalkulatora wielomianów rzadkich wielu zmiennych

  @authors Jakub Krakowiak <jk429351@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/
#include <stdlib.h>
#include "data_structures.h"

PolyStack PolyStackInit() {
    return (PolyStack) {.arr = NULL, .index = 0, .arraySize = 0};
}

bool PolyStackIsEmpty(PolyStack s) {
    return s.index == 0;
}

/**
 * Przedłuża tablicę wielomianów.
 * @param[in] arr : tablica wielomianów @f$arr@f$
 * @param[in] arraySize : długość tablicy wielomianów @f$arraySize@f$
 */
void ExtendPolyArray(Poly **arr, size_t *arraySize) {
    Poly *newArr;
    if (*arraySize == 0) {
        newArr = secureMalloc(STARTING_ARRAY_SIZE * sizeof(Poly));
        *arraySize = STARTING_ARRAY_SIZE;
    } else {
        newArr = secureMalloc((*arraySize) * 2 * sizeof(Poly));

        for (size_t i = 0; i < *arraySize; i++) {
            newArr[i] = (*arr)[i];
        }

        (*arraySize) = (*arraySize) * 2;
        free(*arr);
    }

    (*arr) = newArr;
}

void PolyStackPush(PolyStack *s, Poly p) {

    if ((*s).index == (*s).arraySize) {
        ExtendPolyArray(&((*s).arr), &((*s).arraySize));
    }

    (*s).arr[(*s).index] = p;
    ((*s).index)++;
}

Poly PolyStackPop(PolyStack *s) {
    assert(!PolyStackIsEmpty(*s));
    Poly result = (*s).arr[(*s).index - 1];
    (*s).arr[(*s).index - 1] = PolyZero();
    ((*s).index)--;
    return result;
}

void PolyStackDestroy(PolyStack *s) {
    if (!PolyStackIsEmpty(*s)) {
        for (size_t i = 0; i < (*s).index; i++) {
            PolyDestroy(&((*s).arr[i]));
        }
    }
    free((*s).arr);
}

void WritePolyToArray(Poly p, Poly **arr, size_t *index, size_t
*allocatedSize) {
    if (*index == *allocatedSize) {
        ExtendPolyArray(arr, allocatedSize);
    }

    (*arr)[*index] = p;
    (*index)++;
}

void *secureMalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        exit(1);
    }

    return ptr;
}