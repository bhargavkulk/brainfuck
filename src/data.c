#include "data.h"
#include "util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, newCount)                                    \
    (type *)reallocate(pointer, sizeof(type) * (newCount))

void *reallocate(void *pointer, usize new_capacity) {
    void *result = realloc(pointer, new_capacity);
    if (result == NULL) {
        perror("realloc");
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
        vector->array = GROW_ARRAY(int, vector->array, vector->capacity);
    }

    vector->array[vector->count] = number;
    vector->count++;
}

void vector_free(Vector *vector) { free(vector->array); }

// *s_ptr starts at -1!!!!
void stack_push(Stack *vector, int *s_ptr, int number) {
    if (*s_ptr == vector->count - 1) {
        vector_append(vector, number);
        *s_ptr += 1;
    } else {
        *s_ptr += 1;
        vector->array[*s_ptr] = number;
    }
}

int stack_pop(Stack *vector, int *s_ptr) {
    if (*s_ptr == -1) {
        fprintf(stderr, "stack_pop: underflow");
        exit(1);
    } else {
        int thing = vector->array[*s_ptr];
        *s_ptr -= 1;
        return thing;
    }
}
