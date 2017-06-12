CC = gcc
CFLAGS = -Wall -Wno-pointer-sign -c
LDFLAGS = -lncurses -lm
DIRBUILD = build
DIRSRC = src
SOURCES := $(shell ls $(DIRSRC)/*.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = $(DIRBUILD)/emulator

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	mkdir -p $(DIRBUILD)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm $(OBJECTS) $(EXECUTABLE)

