CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=funciones_UMV.c log.c stack.c funciones_UMV.h log.h stack.h estructuras_umv.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=umv1

all: 
	cd Sockets; make
	$(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
