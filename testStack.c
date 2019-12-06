#include "Stack.h"
#include "graph.h"
#include <assert.h>
#include <stdbool.h>

int main() {
	SOMMET a = {0, 0, NULL, NULL};
	SOMMET b = {0, 0, NULL, NULL};
	SOMMET c = {0, 0, NULL, NULL};
	SOMMET d = {0, 0, NULL, NULL};
	SOMMET e = {0, 0, NULL, NULL};

	Stack *stack = createStack(3);
	assert(pushStack(stack, &a) == true);
	assert(pushStack(stack, &b) == true);
	assert(pushStack(stack, &c) == true);
	assert(pushStack(stack, &d) == true);
	assert(isStackEmpty(stack) == false);

	assert(popStack(stack) == &d);
	assert(popStack(stack) == &c);
	assert(popStack(stack) == &b);
	assert(popStack(stack) == &a);
	assert(popStack(stack) == NULL);
	assert(popStack(stack) == NULL);
	assert(isStackEmpty(stack) == true);

	assert(pushStack(stack, &e) == true);
	assert(popStack(stack) == &e);

	freeStack(stack);
}