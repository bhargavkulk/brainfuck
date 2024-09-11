FLAGS:= -Wall -Wextra -Iinclude -O3

SRC:= $(wildcard src/*.c)
HDR:= $(wildcard include/*.h)
OBJ:= $(patsubst src/%.c, build/%.o, $(SRC))
BIN:= bin/bf

$(BIN): $(OBJ)
	mkdir -p bin
	clang $^ -o $@

build/%.o: src/%.c
	mkdir -p build
	clang $(FLAGS) -c $^ -o $@

.PHONY: clean fmt
clean:
	rm -rf build bin

fmt:
	clang-format -style=file -i $(SRC) $(HDR)
