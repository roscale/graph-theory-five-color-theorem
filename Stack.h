#ifndef FIVE_COLOR_STACK_H
#define FIVE_COLOR_STACK_H

#include "graph.h"
#include <stddef.h>

typedef struct {
   SOMMET **vertices;
   size_t capacity;
   size_t size;
} Stack;

Stack *createStack(size_t initialCapacity);

void freeStack(Stack *stack);

bool pushStack(Stack *stack, SOMMET *vertex);

SOMMET *popStack(Stack *stack);

bool isVertexInStack(Stack *stack, SOMMET *vertex);

bool isStackEmpty(Stack *stack);

#endif //FIVE_COLOR_STACK_H
