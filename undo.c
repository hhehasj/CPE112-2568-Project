#include "undo.h"
#include <stdio.h>
#include <stdlib.h>

void Initialize_Stack(struct Stack *stk) {
    stk->top = -1;
    stk->capacity = 10; // Start with a small default capacity
    stk->tasks = (task*)malloc(stk->capacity * sizeof(task));

    if (stk->tasks == NULL) {
        printf("Memory allocation failed!\n");
    }
}

void push(struct Stack *stk, task newTask) {
    // printf("Pushed: %s\n", newTask.name); // Testing

    // If stack is full, double its capacity dynamically
    if ( stk->top == stk->capacity - 1 ) {
        stk->capacity *= 2;
        stk->tasks = (task*)realloc(stk->tasks, stk->capacity * sizeof(task));

        if (stk->tasks == NULL) {
            printf("Memory reallocation failed!\n");
            return;
        }
    }

    stk->top++;
    stk->tasks[stk->top] = newTask;
}

task pop(struct Stack *stk) {
    if ( stk->top == -1 ) {
        printf("Stack Underflow\n");
        task empty = {.deadline = 0, .name = "", .tag = UNCAT};
        return empty;
    }

    task popped = stk->tasks[stk->top];
    stk->top--;
    // printf("Popping from stack: %s\n", popped.name); // Testing
    return popped;
}

void Free_Stack(struct Stack *stk) {
    free(stk->tasks);
    stk->tasks = NULL;
    stk->top = -1;
    stk->capacity = 0;
}
