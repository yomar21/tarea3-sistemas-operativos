CC := cc
CPPFLAGS := -Iinclude
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic

TARGET := build/simulador
TEST_TARGET := build/test_disco
SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c,build/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

build/%.o: src/%.c Makefile | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

build:
	mkdir -p build

run: $(TARGET)
	./$(TARGET)

$(TEST_TARGET): tests/test_disco.c build/disco.o include/disco.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) tests/test_disco.c build/disco.o $(LDLIBS) -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f build/*.o build/*.d $(TARGET) $(TEST_TARGET)

-include $(DEPS)
