#include "Stack.h"
#include <stdlib.h>
#include <assert.h>

Stack *createStack(size_t initialCapacity) {
	Stack *stack = malloc(sizeof(Stack));
	if (stack == NULL) {
		return NULL;
	}

	stack->capacity = initialCapacity;
	stack->size = 0;
	stack->vertices = malloc(initialCapacity * sizeof(SOMMET *));

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

bool pushStack(Stack *stack, SOMMET *vertex) {
	assert(stack != NULL && vertex != NULL);

	if (stack->size >= stack->capacity) {
		// Resize array
		SOMMET **new_array = realloc(stack->vertices, 2 * stack->capacity *
			  sizeof(SOMMET*));
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

SOMMET *popStack(Stack *stack) {
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
