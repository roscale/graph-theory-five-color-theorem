#ifndef FIVE_COLOR_STACK_H
#define FIVE_COLOR_STACK_H

#include "graph.h"
#include <stddef.h>

typedef struct {
   Vertex **vertices;
   size_t capacity;
   size_t size;
} Stack;

Stack *createStack(size_t initialCapacity);

void freeStack(Stack *stack);

bool pushStack(Stack *stack, Vertex *vertex);

Vertex *popStack(Stack *stack);

bool isVertexInStack(Stack *stack, Vertex *vertex);

bool isStackEmpty(Stack *stack);

void removeElementFromStack(Stack *stack, Vertex *vertex);

#endif //FIVE_COLOR_STACK_H
