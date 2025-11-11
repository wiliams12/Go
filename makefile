CC = gcc
SRC = main.c board.c ui.c game_loops.c hash.c
TARGET = go

INCLUDE = -I/opt/homebrew/Cellar/raylib/5.5/include
LIBS = -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib \
       -framework OpenGL -framework Cocoa -framework IOKit


.PHONY: hello build run clean

hello:
	@echo "Use 'make build' to compile the program, 'make run' to execute it"

build: $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(INCLUDE) $(LIBS)

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET)
