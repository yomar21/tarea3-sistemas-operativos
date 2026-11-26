CC := cc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic

TARGET := build/simulador

.PHONY: all run clean

all: $(TARGET)

$(TARGET): src/main.c Makefile | build
	$(CC) $(CFLAGS) src/main.c -o $(TARGET)

build:
	mkdir -p build

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)