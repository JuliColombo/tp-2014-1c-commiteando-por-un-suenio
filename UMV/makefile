CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=umv.c umv1 umv2
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=umv

all: 
	cd libs; make
	$(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
