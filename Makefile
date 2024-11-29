CC = gcc
CFLAGS = -Wall -Wextra -I include
SRCDIR = src
BINDIR = bin
INCDIR = include
UTILS = utils

SOURCES = $(SRCDIR)/main.c $(SRCDIR)/file_operations.c $(SRCDIR)/directory_ops.c $(SRCDIR)/permissions.c $(SRCDIR)/logger.c $(UTILS)/logo.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = file_manager

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^

%.o: %.c $(INCDIR)/file_manager.h $(INCDIR)/file_manager_logo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o $(BINDIR)/$(EXECUTABLE)
