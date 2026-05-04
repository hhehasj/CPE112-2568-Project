#ifndef UNDO_H
#define UNDO_H

#include "queue.h"

struct Stack {
    task *tasks;
    int top;
    int capacity;
};

void Initialize_Stack(struct Stack *stk);
void push(struct Stack *stk, task newTask);
task pop(struct Stack *stk);
void Free_Stack(struct Stack *stk);

#endif
