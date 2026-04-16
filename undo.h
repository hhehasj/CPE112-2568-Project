#ifndef UNDO_H
#define UNDO_H

#include "queue.h"
#define MAX 100

struct Stack {
    task tasks[MAX];
    int top;
};

void Initialize_Stack(struct Stack *stk);
void push(struct Stack *stk, task newTask);
task pop(struct Stack *stk);

#endif
