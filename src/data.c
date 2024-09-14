#include "data.h"
#include "util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, newCount)                                    \
    (type *)reallocate(pointer, sizeof(type) * (newCount))

void *reallocate(void *pointer, usize new_capacity) {
    void *result = realloc(pointer, new_capacity);
    if (result == NULL) {
        eprintf("reallocate: failed to realloc");
        exit(1);
    }
    return result;
}

void vector_init(Vector *vector) {
    vector->count = 0;
    vector->capacity = 0;
    vector->array = NULL;
}

void vector_append(Vector *vector, int number) {
    if (vector->capacity < vector->count + 1) {
        int old_capacity = vector->capacity;
        vector->capacity = GROW_CAPACITY(old_capacity);
        // GROW_ARRAY may fail
        vector->array = GROW_ARRAY(int, vector->array, vector->capacity);
    }

    vector->array[vector->count] = number;
    vector->count++;
}

void vector_insert(Vector *vector, int index, int number) {
    if (vector->capacity < vector->count + 1) {
        int old_capacity = vector->capacity;
        vector->capacity = GROW_CAPACITY(old_capacity);
        // GROW_ARRAY may fail
        vector->array = GROW_ARRAY(int, vector->array, vector->capacity);
    }

    if (index > vector->count) {
        eprintf("vector_insert: index out of bounds");
        exit(1);
    }

    if (index == vector->count) {
        vector_append(vector, number);
    } else {
        memmove(vector->array + index + 1, vector->array + index,
                sizeof(int) * (vector->count - index));
        vector->array[index] = number;
        vector->count++;
    }
}

void vector_erase(Vector *vector, int index) {
    if (index >= vector->count) {
        eprintf("vector_erase: index out of bounds");
        exit(1);
    }

    memmove(vector->array + index, vector->array + index + 1,
            sizeof(int) * (vector->count - index - 1));
    vector->count--;
}

// void vector_replace(Vector *vector, int start, int end, int *arr, int size)
// {}

int vector_last(Vector *vector) {
    if (vector->count == 0) {
        eprintf("vector_last: empty vector");
    }
    return vector->array[vector->count - 1];
}

void vector_free(Vector *vector) { free(vector->array); }

// stack.count is the stack pointer
inline void stack_push(Stack *stack, int number) {
    vector_append(stack, number);
}

int stack_pop(Stack *stack) {
    if (stack->count == 0) {
        // empty stack
        eprintf("stack_pop: stack underflow");
        exit(1);
    }

    stack->count--;
    return stack->array[stack->count];
}

void vector_pop(Vector *vector) {
    if (vector->count == 0) {
        // empty stack
        eprintf("vector_pop: empty vector");
        exit(1);
    }

    vector->count--;
}
