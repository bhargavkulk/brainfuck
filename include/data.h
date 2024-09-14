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
void vector_insert(Vector *vector, int index, int number);
void vector_erase(Vector *vector, int index);
// void vector_replace(Vector *vector, int start, int end, int *arr, int size);
void vector_pop(Vector *vector);
int vector_last(Vector *vector);
void vector_free(Vector *vector);
void stack_push(Stack *stack, int number);
int stack_pop(Stack *stack);

#define IN_VECTOR(_value_, _vector_)                                           \
    for (int *_value_ = (_vector_).array;                                      \
         _value_ != (_vector_).array + (_vector_).count; _value_++)

#endif
