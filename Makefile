## Variables
# Tools & flags
CC=gcc
CFLAGS=--std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes -g
LD=gcc
LDFLAGS=

# Files
EXEC=graph_coloring
MODULES=main.c graph.c Stack.c tools.c
OBJECTS=main.o graph.o Stack.o tools.o
## Rules
all: $(EXEC)

graph_coloring: $(OBJECTS)
	$(LD) -o graph_coloring $(OBJECTS) $(LDFLAGS)

main.o: main.c graph.h Stack.h tools.h
	$(CC) -c main.c -o main.o  $(CFLAGS)

graph.o: graph.c graph.h
	$(CC) -c graph.c -o graph.o  $(CFLAGS)

Stack.o: Stack.c Stack.h
	$(CC) -c Stack.c -o Stack.o  $(CFLAGS)

tools.o: tools.c tools.h Stack.h graph.h
	$(CC) -c tools.c -o tools.o  $(CFLAGS)

clean:
	rm -f *.o graph_coloring 