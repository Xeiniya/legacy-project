CC = gcc
CFLAGS = -Wall -Wextra -std=c11
OBJDIR = obj
BINDIR = bin
SRCDIR = src
INCDIR = include

SRC = $(SRCDIR)/main.c $(SRCDIR)/filemanager.c $(SRCDIR)/client.c
OBJ = $(OBJDIR)/main.o $(OBJDIR)/filemanager.o $(OBJDIR)/client.o
BIN = $(BINDIR)/car_owners

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/*.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
