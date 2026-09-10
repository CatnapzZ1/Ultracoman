CC ?= cc

CPPFLAGS += -D_DEFAULT_SOURCE -Iinclude

CFLAGS += \
						-std=c11 \
						-O1 \
						-Wall \
						-Wextra \
						-Wpedantic

TARGET := build/ultracoman

SOURCES := \
					 src/main.c \
					 src/app.c \
					 src/cli.c \
					 src/serial.c \
					 src/terminal.c \
					 src/event_loop.c \
					 src/io.c

OBJECTS := $(SOURCES:src/%.c=build/%.o)

.PHONY: all clean

all:$(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

build/%.o: src/%.c | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build

