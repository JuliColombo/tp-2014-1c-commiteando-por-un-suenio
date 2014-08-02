CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=package.c socket.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=umvaux2

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
