CC = gcc
CFLAGS = -std=c90 -Wall -Wextra -Iinclude

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

TARGET = $(BIN_DIR)/calculadora.exe

all: dirs $(TARGET)

dirs:
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	del /Q $(OBJ_DIR)\*.o
	del /Q $(TARGET)

run: all
	$(TARGET)

reset:
	rmdir /S /Q $(OBJ_DIR)
	rmdir /S /Q $(BIN_DIR)

.PHONY: all clean run reset dirs
