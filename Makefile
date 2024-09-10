FLAGS:= -Wall -Wextra -Iinclude

SRC:= $(wildcard src/*.c)
OBJ:= $(patsubst src/%.c, build/%.o, $(SRC))
BIN:= bin/bf

$(BIN): $(OBJ)
	mkdir -p bin
	clang $^ -o $@

build/%.o: src/%.c
	mkdir -p build
	clang $(FLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -rf build bin
