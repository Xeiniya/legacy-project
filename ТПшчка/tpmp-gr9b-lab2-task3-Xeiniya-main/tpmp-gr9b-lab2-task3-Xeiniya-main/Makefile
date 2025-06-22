# Project directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -I$(INC_DIR)

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Executable name
EXECUTABLE = $(BIN_DIR)/znak_program

# Default target
all: directories $(EXECUTABLE)

# Create required directories
directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

# Linking rule
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJ_DIR)/*.o $(EXECUTABLE)

# Deep clean
distclean: clean
	rm -rf $(BIN_DIR) $(OBJ_DIR)

# Dependencies
$(OBJ_DIR)/client.o: $(SRC_DIR)/client.c $(INC_DIR)/znakmanager.h $(INC_DIR)/interface.h
$(OBJ_DIR)/server.o: $(SRC_DIR)/znakmanager.c $(INC_DIR)/znakmanager.h $(INC_DIR)/interface.h

# Run the program
run: $(EXECUTABLE)
	$(EXECUTABLE)

.PHONY: all clean distclean run directories
