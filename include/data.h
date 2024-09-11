#ifndef VECTOR_H
#define VECTOR_H

#include "util.h"

typedef struct {
    int count;
    int capacity;
    int *array;
} Vector;

typedef Vector Stack;

void vector_init(Vector *vector);
void vector_append(Vector *vector, int number);
void vector_free(Vector *vector);
void stack_push(Stack *vector, int *s_ptr, int number);
int stack_pop(Stack *vector, int *s_ptr);

#endif
