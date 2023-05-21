#ifndef stack_h
#define stack_h

//lista enlazada para implementar una pila

#include "list.h"
typedef List Stack;

#define createStack() createList()

#define popStack(stack) popFront(stack)

#define pushStack(stack, data) pushFront(stack, data)

#define topStack(stack) firstList(stack)

#define cleanStack(stack) cleanList(stack)

#endif /* List_h */