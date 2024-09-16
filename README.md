# brainfuck.c
An Optimizing Brainfuck implementation in C

## How to run
```bash
make
./bin/bf <file-name>
```

Run benchmarks with `make bench`. This depends on [hyperfine](https://github.com/sharkdp/hyperfine)

## Internal IR
```
ADD <op>                 ; adds <op> to the current tape cell
MOVE <op>                ; moves pointer by <op> cells
OUT                      ; saves input from stdin to current tape cell
IN                       ; outputs curren tape cell to stdout as ASCII character
J_FWD <label>            ; jumps to <label> if 0
J_BACK <label>           ; jumps to <label> if not zero
HALT                     ; stops execution
SET <op>                 ; sets current tape cell to <op>
MOVE_TO_0 <op>           ; keeps moving pointer by <op> cells, until 0
MEM_MOVE <offset> <mult> ; moves data at current tape cell to <offset>
                         ; multiplied by <mult>
```

## Brainfuck mapped to IR
```
+++...n times               -> ADD n
>>>...n times               -> MOVE n
[-] or [+]                  -> SET 0
[{MOVE n}]                  -> MOVE_TO_0 n
[-{MOVE n}{ADD m}{MOVE -n}] -> MEM_MOVE n m
```

## TODO
- [X] interpreter
- [X] threaded interpreter
- [X] basic peephole optimizers
- [ ] profiler
- [ ] compile to arm
- [ ] compile to llvmIR
- [ ] JIT execution with either GNU lightning or llvm
- [ ] Partial evaluator
- [ ] Get rid of tape wherever possible
