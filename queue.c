#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Required for malloc/realloc/free

void Initialize(task_queue *q){
    q->size = 0;
    q->capacity = 10; // Start with a small default capacity
    q->tasks = (task*)malloc(q->capacity * sizeof(task));
    
    if (q->tasks == NULL) {
        printf("Memory allocation failed!\n");
    }
}

void Insert(task new_task, task_queue *q){
    // If the array is full, double its capacity dynamically
    if(q->size >= q->capacity){
        q->capacity *= 2;
        q->tasks = (task*)realloc(q->tasks, q->capacity * sizeof(task));
        
        if (q->tasks == NULL) {
            printf("Memory reallocation failed!\n");
            return;
        }
    }

    int i = q->size-1;
    while(i >= 0){
        if (new_task.deadline < q->tasks[i].deadline) {
            q->tasks[i + 1] = q->tasks[i];
            i--;
        } else {
            break;
        }
    }

    q->tasks[i+1] = new_task;
    q->size++;
}

task Extract(task_queue *q) {
    if (q->size == 0) {
        task empty_task = {"", -1, UNCAT};
        return empty_task;
    }

    task temp = q->tasks[0];
    for(int i = 0; i < q->size-1; i++){
        q->tasks[i] = q->tasks[i+1];
    }
    q->size--;
    return temp;
}

int Deletion(task_queue *q, task to_remove) {
    if ( q->size == 0 ) {
        printf("Task Queue Empty\n");
        return -1;
    }

    task empty_task = {"", -1, UNCAT};

    for (int i=0; i<q->size; i++ ) {
        if ( strcmp(q->tasks[i].name, to_remove.name) == 0 ) {
            printf("Task: %s (has been deleted)\n", q->tasks[i].name); 
            
            for(int j=i; j<q->size-1; j++) {
                q->tasks[j] = q->tasks[j+1];
            }
            
            // Clear the last duplicate item and shrink size
            q->tasks[q->size - 1] = empty_task;
            q->size--;

            return 0;
        }
    }
    return -1;
}

void Free_Queue(task_queue *q) {
    free(q->tasks);
    q->tasks = NULL;
    q->size = 0;
    q->capacity = 0;
}