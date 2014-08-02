CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=package.c package.h socket.c socket.h estructuras_socket.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=umv

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
