#include "Stack.h"
#include "graph.h"
#include "tools.h"
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

Stack *createStack(size_t initialCapacity) {
	Stack *stack = allocateMemory(sizeof(Stack));
	if (stack == NULL) {
		return NULL;
	}

	stack->capacity = initialCapacity;
	stack->size = 0;
	stack->vertices = allocateMemory(initialCapacity * sizeof(Vertex *));

	// Zero out the memory
	memset(stack->vertices, 0, initialCapacity * sizeof(Vertex *));

	return stack;
}

void freeStack(Stack *stack) {
	if (stack != NULL) {
		if (stack->vertices != NULL) {
			free(stack->vertices);
		}
		free(stack);
	}
}

bool pushStack(Stack *stack, Vertex *vertex) {
	assert(stack != NULL && vertex != NULL);

	if (stack->size >= stack->capacity) {
		// Resize array
		Vertex **new_array = realloc(stack->vertices, 2 * stack->capacity *
		                                              sizeof(Vertex *));
		if (new_array != NULL) {
			stack->vertices = new_array;
			stack->capacity *= 2;
		} else {
			return false;
		}
	}
	stack->vertices[stack->size++] = vertex;
	return true;
}

bool isVertexInStack(Stack *stack, Vertex *vertex) {
	assert(stack != NULL && vertex != NULL);
	Vertex **v = stack->vertices;
	for (unsigned i = 0; v[i] != NULL && i < stack->size; ++i) {
		if (v[i]->label == vertex->label) {
			return true;
		}
	}
	return false;
}

Vertex *popStack(Stack *stack) {
	assert(stack != NULL);

	if (stack->size == 0) {
		return NULL;
	}

	return stack->vertices[--stack->size];
}

bool isStackEmpty(Stack *stack) {
	assert(stack != NULL);
	return stack->size == 0;
}


void removeElementFromStack(Stack *stack, Vertex *vertex) {
	assert(stack != NULL && vertex != NULL);
	bool vertex_found = false;

	for (size_t i = 0; i < stack->size; ++i) {
		if (stack->vertices[i] == vertex && !vertex_found) {
			vertex_found = true;
			--stack->size;
		}

		if (vertex_found) {
			stack->vertices[i] = stack->vertices[i + 1];
		}
	}
}
