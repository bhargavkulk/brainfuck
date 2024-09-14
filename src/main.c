#include "data.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum Instruction {
    MOVE,      // 1 operand
    ADD,       // 2 operand
    OUT,       // No operands
    IN,        // No operands
    J_FWD,     // 1 operand
    J_BACK,    // 1 operand
    HALT,      // No operands
    SET,       // 1 operand
    MOVE_TO_0, // 1 operand
    MEM_MOVE,  // 1 operand
};

Vector instructions;
Vector operands;
Vector offsets;

#define INSTR_FROM_END(index) instructions.array[instructions.count - (index)]
#define OPERAND_FROM_END(index) operands.array[operands.count - (index)]
#define OFFSET_FROM_END(index) offsets.array[offsets.count - (index)]

#define IS_LAST_INSTR(instr)                                                   \
    ((instructions.count != 0) && (vector_last(&instructions) == (instr)))

//- Instruction Matching -------------------------------------------------------
bool match_set() {
    //[-]
    if (instructions.count >= 2) {
        int instr1 = INSTR_FROM_END(2);
        int instr2 = INSTR_FROM_END(1);
        int op2 = OPERAND_FROM_END(1);
        return instr1 == J_FWD && instr2 == ADD && (op2 == 1 || op2 == -1);
    } else {
        return false;
    }
}

bool match_move_to_zero() {
    //[>]
    if (instructions.count >= 2) {
        int instr1 = INSTR_FROM_END(2);
        int instr2 = INSTR_FROM_END(1);
        return instr1 == J_FWD && instr2 == MOVE;
    } else {
        return false;
    }
}

bool match_multiply() {
    //[->+< ]
    if (instructions.count >= 5) {
        // [
        int instr1 = INSTR_FROM_END(5);

        // -
        int instr2 = INSTR_FROM_END(4);
        int op2 = OPERAND_FROM_END(4);

        // >
        int instr3 = INSTR_FROM_END(3);
        int op3 = OPERAND_FROM_END(3);

        // +
        int instr4 = INSTR_FROM_END(2);
        int op4 = OPERAND_FROM_END(2);

        // <
        int instr5 = INSTR_FROM_END(1);
        int op5 = OPERAND_FROM_END(1);

        return instr1 == J_FWD && instr2 == ADD && instr3 == MOVE &&
               instr4 == ADD && instr5 == MOVE && op2 == -1 && op3 + op5 == 0 &&
               op4 > 0;

    } else {
        return false;
    }
}

Stack stack;
int s_ptr = -1;

void dis_bf() {
    for (int i = 0; i < instructions.count;) {
        switch (instructions.array[i]) {
        case MOVE:
            printf("%d > %d\n", i, operands.array[i]);
            i++;
            break;
        case SET:
            printf("%d SET %d\n", i, operands.array[i]);
            i++;
            break;
        case OUT:
            printf("%d .\n", i);
            i++;
            break;
        case IN:
            printf("%d ,\n", i);
            i++;
            break;
        case ADD:
            printf("%d + %d %d\n", i, operands.array[i], offsets.array[i]);
            i++;
            break;
        case J_FWD:
            printf("%d [ %d\n", i, operands.array[i]);
            i++;
            break;
        case J_BACK:
            printf("%d ] %d\n", i, operands.array[i]);
            i++;
            break;
        case MOVE_TO_0:
            printf("%d >0 %d\n", i, operands.array[i]);
            i++;
            break;
        case HALT:
            printf("%d HALT\n", i);
            i++;
            break;
        case MEM_MOVE:
            printf("%d >! %d\n", i, operands.array[i]);
            i++;
            break;
        }
    }
}

int compile_bf(FILE *fp) {
    int i_ptr = 0;
    char c;
    int target, operand;
    int match_mult = 0;
    while ((c = getc(fp)) != EOF) {
        switch (c) {
        case '>':
            if (IS_LAST_INSTR(MOVE)) {
                operands.array[i_ptr - 1] += 1;
                if (operands.array[i_ptr - 1] == 0) {
                    vector_pop(&instructions);
                    vector_pop(&operands);
                    vector_pop(&offsets);
                    i_ptr--;
                }
            } else {
                vector_insert(&instructions, i_ptr, MOVE);
                vector_insert(&operands, i_ptr, 1);
                vector_insert(&offsets, i_ptr, 0);
                i_ptr++;
            }
            break;
        case '<':
            if (IS_LAST_INSTR(MOVE)) {
                operands.array[i_ptr - 1] -= 1;
                if (operands.array[i_ptr - 1] == 0) {
                    vector_pop(&instructions);
                    vector_pop(&operands);
                    vector_pop(&offsets);
                    i_ptr--;
                }
            } else {
                vector_insert(&instructions, i_ptr, MOVE);
                vector_insert(&operands, i_ptr, -1);
                vector_insert(&offsets, i_ptr, 0);
                i_ptr++;
            }
            break;
        case '+':
            if (IS_LAST_INSTR(ADD)) {
                operands.array[i_ptr - 1] += 1;
                if (operands.array[i_ptr - 1] == 0) {
                    vector_pop(&instructions);
                    vector_pop(&operands);
                    vector_pop(&offsets);
                    i_ptr--;
                }
            } else {
                vector_insert(&instructions, i_ptr, ADD);
                vector_insert(&operands, i_ptr, 1);
                vector_insert(&offsets, i_ptr, 0);
                i_ptr++;
            }
            break;
        case '-':
            if (IS_LAST_INSTR(ADD)) {
                operands.array[i_ptr - 1] -= 1;
                if (operands.array[i_ptr - 1] == 0) {
                    vector_pop(&instructions);
                    vector_pop(&operands);
                    vector_pop(&offsets);
                    i_ptr--;
                }
            } else {
                vector_insert(&instructions, i_ptr, ADD);
                vector_insert(&operands, i_ptr, -1);
                vector_insert(&offsets, i_ptr, 0);
                i_ptr++;
            }
            break;
        case ',':
            vector_insert(&instructions, i_ptr, IN);
            vector_insert(&operands, i_ptr, 0);
            vector_insert(&offsets, i_ptr, 0);
            i_ptr++;
            break;
        case '.':
            vector_insert(&instructions, i_ptr, OUT);
            vector_insert(&operands, i_ptr, 0);
            vector_insert(&offsets, i_ptr, 0);
            i_ptr++;
            break;
        case '[':
            vector_insert(&instructions, i_ptr, J_FWD);
            vector_insert(&operands, i_ptr, 0);
            vector_insert(&offsets, i_ptr, 0);
            stack_push(&stack, i_ptr);
            i_ptr++;
            break;
        case ']':
            target = stack_pop(&stack);
            if (match_set()) {
                vector_pop(&instructions);
                vector_pop(&instructions);
                vector_pop(&operands);
                vector_pop(&operands);
                vector_pop(&offsets);
                vector_pop(&offsets);
                vector_insert(&instructions, i_ptr - 2, SET);
                vector_insert(&operands, i_ptr - 2, 0);
                vector_insert(&offsets, i_ptr - 2, 0);
                i_ptr--;
            } else if (match_move_to_zero()) {
                vector_pop(&instructions);
                vector_pop(&instructions);
                operand = vector_last(&operands);
                vector_pop(&operands);
                vector_pop(&operands);
                vector_pop(&offsets);
                vector_pop(&offsets);
                vector_insert(&instructions, i_ptr - 2, MOVE_TO_0);
                vector_insert(&operands, i_ptr - 2, operand);
                vector_insert(&offsets, i_ptr - 2, 0);
                i_ptr--;
            } else if (match_multiply()) {
                match_mult++;
                operand = OPERAND_FROM_END(3);
                vector_pop(&instructions);
                vector_pop(&instructions);
                vector_pop(&instructions);
                vector_pop(&instructions);
                vector_pop(&instructions);
                vector_pop(&operands);
                vector_pop(&operands);
                vector_pop(&operands);
                vector_pop(&operands);
                vector_pop(&operands);
                vector_pop(&offsets);
                vector_pop(&offsets);
                vector_pop(&offsets);
                vector_pop(&offsets);
                vector_pop(&offsets);
                i_ptr -= 5;
                vector_insert(&instructions, i_ptr, MEM_MOVE);
                vector_insert(&operands, i_ptr, operand);
                vector_insert(&offsets, i_ptr, 0);
                i_ptr++;
            } else {
                vector_insert(&instructions, i_ptr, J_BACK);
                vector_insert(&operands, i_ptr, target);
                vector_insert(&offsets, i_ptr, 0);
                operands.array[target] = i_ptr;
                i_ptr++;
            }
        }
    }

    vector_append(&instructions, HALT);
    vector_append(&operands, 0);
    vector_append(&offsets, 0);

    printf("match mult == %d\n", match_mult);

    return 0;
}

void interpret_bf_threaded() {
    static void *dispatch_table[] = {&&do_move, &&do_add, &&do_out,
                                     &&do_in,   &&do_fwd, &&do_back,
                                     &&do_halt, &&do_set, &&do_move_to_0, &&do_mem_move};
#define DISPATCH() goto *dispatch_table[instructions.array[i_ptr]]
    int i_ptr = 0;
    int t_ptr = 8192 / 2;
    uint8_t *tape = (uint8_t *)calloc(8192, sizeof(uint8_t));
    // printf("start\n");
    // getchar();
    DISPATCH();
    while (i_ptr < instructions.count) {
    do_move:
        t_ptr += operands.array[i_ptr];
        i_ptr++;
        // printf("left\n");
        // getchar();
        DISPATCH();
    do_add:
        tape[t_ptr + offsets.array[i_ptr]] += operands.array[i_ptr];
        i_ptr++;
        // printf("add\n");
        // getchar();
        DISPATCH();
    do_out:
        putchar(tape[t_ptr]);
        i_ptr++;
        // printf("out\n");
        // getchar();
        DISPATCH();
    do_in:
        tape[t_ptr] = getchar();
        i_ptr++;
        // printf("in\n");
        // getchar();
        DISPATCH();
    do_fwd:
        i_ptr = tape[t_ptr] ? i_ptr + 1 : operands.array[i_ptr];
        // printf("fwd\n");
        // getchar();
        DISPATCH();
    do_back:
        i_ptr = tape[t_ptr] ? operands.array[i_ptr] : i_ptr + 1;
        // printf("back\n");
        // getchar();
        DISPATCH();
    do_set:
        tape[t_ptr] = operands.array[i_ptr];
        i_ptr++;
        // printf("set\n");
        // getchar();
        DISPATCH();
    do_move_to_0:
        while (tape[t_ptr]) {
            t_ptr += operands.array[i_ptr];
        }
        i_ptr++;
        // printf("move0\n");
        // getchar();
        DISPATCH();
    do_mem_move:
        if (tape[t_ptr]) {
            int move_to_ptr = t_ptr + operands.array[i_ptr];
            tape[move_to_ptr] += tape[t_ptr];
            tape[t_ptr] = 0;
        }
        i_ptr++;
        DISPATCH();
    do_halt:
        break;
    }
}

int main(int argc, char *argv[]) {
    vector_init(&instructions);
    vector_init(&operands);
    vector_init(&offsets);
    vector_init(&stack);

    if (argc != 2) {
        fprintf(stderr, "Usage: bf <file>");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    compile_bf(fp);
    // dis_bf();

    // interpret_bf_threaded();

    vector_free(&instructions);
    vector_free(&operands);
    vector_free(&offsets);
    vector_free(&stack);

    fclose(fp);
}
