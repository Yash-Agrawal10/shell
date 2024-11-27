CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
SOURCES = src/wish.c src/parser.c src/executor.c src/builtins.c src/utils.c src/context.c
HEADERS = include/parser.h include/executor.h include/builtins.h include/utils.h include/context.h
TARGET = wish

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
