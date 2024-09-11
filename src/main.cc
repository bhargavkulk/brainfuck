#include "util.hh"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

#define MAX_SIZE (user_size ? user_size : 2048)
int user_size = 0;
int *tape;
int tp = 0;

enum Instruction { LEFT, RIGHT, OUT, IN, INC, DEC, J_START, J_END };
std::vector<usize> instructions;
std::vector<usize> stack;

void print_instructions() {
    for (usize i = 0; i < instructions.size();) {
        switch (instructions[i]) {
        case LEFT:
            std::cout << ">\n";
            i++;
            break;
        case RIGHT:
            std::cout << "<\n";
            i++;
            break;
        case OUT:
            std::cout << ".\n";
            i++;
            break;
        case IN:
            std::cout << ",\n";
            i++;
            break;
        case INC:
            std::cout << "+\n";
            i++;
            break;
        case DEC:
            std::cout << "-\n";
            i++;
            break;
        case J_START:
            std::cout << "[ " << instructions[i + 1] << '\n';
            i += 2;
            break;
        case J_END:
            std::cout << "] " << instructions[i + 1] << '\n';
            i += 2;
            break;
        }
    }
}

void compile(char *file_path) {
    std::ifstream fin(file_path);

    char ch;
    int instr_ptr = 0;
    while (fin.get(ch)) {
        switch (ch) {
        case '>':
            instructions.push_back(LEFT);
            instr_ptr++;
            break;
        case '<':
            instructions.push_back(RIGHT);
            instr_ptr++;
            break;
        case '+':
            instructions.push_back(INC);
            instr_ptr++;
            break;
        case '-':
            instructions.push_back(DEC);
            instr_ptr++;
            break;
        case '.':
            instructions.push_back(OUT);
            instr_ptr++;
            break;
        case ',':
            instructions.push_back(IN);
            instr_ptr++;
            break;
        case '[':
            instructions.push_back(J_START);
            instructions.push_back(0);
            stack.push_back(instr_ptr);
            instr_ptr += 2;
            break;
        case ']':
            instructions.push_back(J_END);
            usize jump_back = stack.back();
            stack.pop_back();
            instructions[jump_back + 1] = instr_ptr;
            instructions.push_back(jump_back);
            instr_ptr += 2;
            break;
        }
    }

    fin.close();
}

void interpret() {
    usize target = 0;
    for (usize i = 0; i < instructions.size();) {
        switch (instructions[i]) {
        case LEFT:
            tp++;
            i++;
            break;
        case RIGHT:
            tp--;
            i++;
            break;
        case OUT:
            putchar(tape[tp]);
            i++;
            break;
        case IN:
            tape[tp] = getchar();
            i++;
            break;
        case INC:
            tape[tp]++;
            i++;
            break;
        case DEC:
            tape[tp]--;
            i++;
            break;
        case J_START:
            target = instructions[i + 1];
            i = tape[tp] ? i + 2 : target;
            break;
        case J_END:
            target = instructions[i + 1];
            i = tape[tp] ? target : i + 2;
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    tape = new int[MAX_SIZE];
    tp = MAX_SIZE / 2;

    std::cout << "File name: " << argv[argc - 1] << std::endl;
    compile(argv[argc - 1]);
    interpret();

    return 0;
}
