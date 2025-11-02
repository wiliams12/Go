CC = gcc
SRC = main.c board.c
TARGET = go

INCLUDE = -I/opt/homebrew/include/SDL3
LIBS = -L/opt/homebrew/lib -lSDL3


.PHONY: hello build run clean

hello:
	@echo "Use 'make build' to compile the program, 'make run' to execute it"

build: $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(INCLUDE) $(LIBS)

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET)
