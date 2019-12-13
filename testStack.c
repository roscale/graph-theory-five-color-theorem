#include "Stack.h"
#include "graph.h"
#include <assert.h>
#include <stdbool.h>

int main() {
	// DO NOT dereference these pointers, they contain invalid addresses
	// They just simulate some vertices
	Vertex *a = (Vertex *) 1;
	Vertex *b = (Vertex *) 2;
	Vertex *c = (Vertex *) 3;
	Vertex *d = (Vertex *) 4;
	Vertex *e = (Vertex *) 5;
	Vertex *f = (Vertex *) 6;

	Stack *stack = createStack(3);
	assert(pushStack(stack, a) == true);
	assert(pushStack(stack, b) == true);
	assert(pushStack(stack, c) == true);
	assert(pushStack(stack, d) == true);
	assert(isStackEmpty(stack) == false);

	assert(popStack(stack) == d);
	assert(popStack(stack) == c);
	assert(popStack(stack) == b);
	assert(popStack(stack) == a);
	assert(popStack(stack) == NULL);
	assert(popStack(stack) == NULL);
	assert(isStackEmpty(stack) == true);

	//		------ test refomeElementFromStack
	assert(pushStack(stack, a) == true);
	assert(pushStack(stack, b) == true);
	assert(pushStack(stack, c) == true);
	assert(pushStack(stack, d) == true);
	assert(pushStack(stack, e) == true);
	removeElementFromStack(stack, c);
	assert(popStack(stack) == e);
	assert(popStack(stack) == d);
	assert(popStack(stack) == b);
	removeElementFromStack(stack, f);
	assert(popStack(stack) == a);

	assert(pushStack(stack, e) == true);
	assert(popStack(stack) == e);

	freeStack(stack);
}
