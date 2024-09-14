FLAGS:= -Wall -Wextra -Iinclude -O3

SRC:= $(wildcard src/*.c)
HDR:= $(wildcard include/*.h)
OBJ:= $(patsubst src/%.c, build/%.o, $(SRC))
BIN:= bin/bf

COMMA:= ,
EMPTY:=
SPACE:= $(EMPTY) $(EMPTY)

BENCHS:= $(subst $(SPACE),$(COMMA),$(wildcard bfbenchmarks/benches/*.b))

$(BIN): $(OBJ)
	mkdir -p bin
	clang $^ -o $@

build/%.o: src/%.c
	mkdir -p build
	clang $(FLAGS) -c $^ -o $@

.PHONY: clean fmt bench
clean:
	rm -rf build bin summary.md

fmt:
	clang-format -style=file -i $(SRC) $(HDR)

bench:
	@hyperfine --export-markdown summary.md -L benchmark $(BENCHS) --output null -N './bin/bf {benchmark}'
