#ifndef UNDO_H
#define UNDO_H

#include "queue.h"

struct Stack {
    task *tasks;  // Changed from fixed array to dynamic pointer
    int top;
    int capacity; // Added to track allocated memory size
};

void Initialize_Stack(struct Stack *stk);
void push(struct Stack *stk, task newTask);
task pop(struct Stack *stk);
void Free_Stack(struct Stack *stk); // Call this before program ends

#endif