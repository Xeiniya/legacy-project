CC = gcc
CFLAGS = -Iinclude -Wall
LDFLAGS = -lsqlite3
OBJDIR = obj
BINDIR = bin
DATADIR = data
DBDIR = db
SRC = src/main.c src/database.c
OBJS = $(SRC:src/%.c=$(OBJDIR)/%.o)
EXEC = $(BINDIR)/app

all: directories $(EXEC)

directories:
	mkdir -p $(OBJDIR) $(BINDIR) $(DATADIR) $(DBDIR)

$(EXEC): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/*.o $(EXEC)

distclean: clean
	rm -rf $(OBJDIR) $(BINDIR) $(DATADIR)/*

.PHONY: all clean distclean directories
