FLAGS:= -Wall -Wextra -Iinclude -std=c++11 -O3

SRC:= $(wildcard src/*.cc)
HDR:= $(wildcard include/*.hh)
OBJ:= $(patsubst src/%.cc, build/%.o, $(SRC))
BIN:= bin/bf

$(BIN): $(OBJ)
	mkdir -p bin
	clang++ $^ -o $@

build/%.o: src/%.cc
	mkdir -p build
	clang++ $(FLAGS) -c $^ -o $@

.PHONY: clean fmt
clean:
	rm -rf build bin

fmt:
	clang-format -style=file -i $(SRC) $(HDR)
