#include "undo.h"
#include <stdio.h>
#include "queue.h"

#define MAX 100

void Initialize_Stack(struct Stack *stk) {
    stk->top = -1;
}

void push(struct Stack *stk, task newTask) {
    // printf("Push to the Undo Stack\n\n"); For Testing

    if ( stk->top == MAX-1 ) {
        printf("Stack Overflow\n");
        return;
    }

    stk->top++;
    stk->tasks[stk->top] = newTask;
}

task pop(struct Stack *stk) {
    if ( stk->top == -1 ) {
        printf("Stack Underflow\n");
        task empty = {.deadline = 0, .name = ""};
        return empty;
    }

    task popped = stk->tasks[stk->top];
    stk->top--;
    // printf("Popping: %s\n", popped.name); For testing
    return popped;
}
