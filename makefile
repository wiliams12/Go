CC = gcc
SRC = main.c board.c ui.c game_loops.c hash.c engine_helpers.c
TARGET = go

INCLUDE = -I/opt/homebrew/Cellar/raylib/5.5/include
LIBS = -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib \
       -framework OpenGL -framework Cocoa -framework IOKit

.PHONY: hello build run asan clean

hello:
	@echo "Use 'make build' to compile, 'make run' to execute, or 'make asan' to run with AddressSanitizer"

build: $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(INCLUDE) $(LIBS)

run: build
	./$(TARGET)


clean:
	rm -f $(TARGET)
