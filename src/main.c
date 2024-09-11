#include "data.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

enum Instruction {
    LEFT,
    RIGHT,
    INC,
    DEC,
    OUT,
    IN,
    J_FWD,
    J_BACK,
};

Vector instructions;
Stack stack;
int s_ptr = -1;

int dis_bf() {
    for (int i = 0; i < instructions.count;) {
        switch (instructions.array[i]) {
        case LEFT:
            printf("%d >\n", i);
            i++;
            break;
        case RIGHT:
            printf("%d <\n", i);
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
        case INC:
            printf("%d +\n", i);
            i++;
            break;
        case DEC:
            printf("%d -\n", i);
            i++;
            break;
        case J_FWD:
            printf("%d [ %d\n", i, instructions.array[i + 1]);
            i += 2;
            break;
        case J_BACK:
            printf("%d ] %d\n", i, instructions.array[i + 1]);
            i += 2;
            break;
        }
    }
}

int compile_bf(FILE *fp) {
    int i_ptr = 0;
    char c;
    int target;
    while ((c = getc(fp)) != EOF) {
        switch (c) {
        case '>':
            vector_append(&instructions, LEFT);
            i_ptr++;
            break;
        case '<':
            vector_append(&instructions, RIGHT);
            i_ptr++;
            break;
        case '+':
            vector_append(&instructions, INC);
            i_ptr++;
            break;
        case '-':
            vector_append(&instructions, DEC);
            i_ptr++;
            break;
        case ',':
            vector_append(&instructions, IN);
            i_ptr++;
            break;
        case '.':
            vector_append(&instructions, OUT);
            i_ptr++;
            break;
        case '[':
            vector_append(&instructions, J_FWD);
            vector_append(&instructions, 0);
            stack_push(&stack, &s_ptr, i_ptr);
            i_ptr += 2;
            break;
        case ']':
            target = stack_pop(&stack, &s_ptr);
            vector_append(&instructions, J_BACK);
            vector_append(&instructions, target);
            instructions.array[target + 1] = i_ptr;
            i_ptr += 2;
        }
    }

    return 0;
}

void interpret_bf() {
    int i_ptr = 0;
    int t_ptr = 8192 / 2;
    int *tape = (int *)calloc(8192, sizeof(int));
    while (i_ptr < instructions.count) {
        switch (instructions.array[i_ptr]) {
        case LEFT:
            t_ptr++;
            i_ptr++;
            break;
        case RIGHT:
            t_ptr--;
            i_ptr++;
            break;
        case INC:
            tape[t_ptr]++;
            i_ptr++;
            break;
        case DEC:
            tape[t_ptr]--;
            i_ptr++;
            break;
        case OUT:
            putchar(tape[t_ptr]);
            i_ptr++;
            break;
        case IN:
            tape[t_ptr] = getchar();
            i_ptr++;
            break;
        case J_FWD:
            i_ptr = tape[t_ptr] ? i_ptr + 2 : instructions.array[i_ptr + 1];
            break;
        case J_BACK:
            i_ptr = tape[t_ptr] ? instructions.array[i_ptr + 1] : i_ptr + 2;
            break;
        }
    }
}

void interpret_bf_threaded() {
    static void *dispatch_table[] = {&&do_left, &&do_right, &&do_inc,
                                     &&do_dec,  &&do_out,   &&do_in,
                                     &&do_fwd,  &&do_back};
#define DISPATCH() goto *dispatch_table[instructions.array[i_ptr]]
    int i_ptr = 0;
    int t_ptr = 8192 / 2;
    int *tape = (int *)calloc(8192, sizeof(int));
    DISPATCH();
    while (i_ptr < instructions.count) {
    do_left:
        t_ptr++;
        i_ptr++;
        DISPATCH();
    do_right:
        t_ptr--;
        i_ptr++;
        DISPATCH();
    do_inc:
        tape[t_ptr]++;
        i_ptr++;
        DISPATCH();
    do_dec:
        tape[t_ptr]--;
        i_ptr++;
        DISPATCH();
    do_out:
        putchar(tape[t_ptr]);
        i_ptr++;
        DISPATCH();
    do_in:
        tape[t_ptr] = getchar();
        i_ptr++;
        DISPATCH();
    do_fwd:
        i_ptr = tape[t_ptr] ? i_ptr + 2 : instructions.array[i_ptr + 1];
        DISPATCH();
    do_back:
        i_ptr = tape[t_ptr] ? instructions.array[i_ptr + 1] : i_ptr + 2;
        DISPATCH();
    }
}

int main(int argc, char *argv[]) {
    vector_init(&instructions);
    vector_init(&stack);
    int ret;
    if (argc != 2) {
        fprintf(stderr, "Usage: bf <file>");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    ret = compile_bf(fp);
    interpret_bf_threaded();

    vector_free(&instructions);
    vector_free(&stack);

    fclose(fp);
}
