#include "queue.h"
#include <stdio.h>
#include <string.h>

void Initialize(task_queue *q){
    q->size = 0;
}

void Insert(task new_task, task_queue *q){
    printf("Inserting into queue\n"); // For testing

    if(q->size >= 100){
        printf("TASK OVERLOAD");
        return;
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
        task empty_task = {"", -1};
        return empty_task;
    }

    task temp = q->tasks[0];
    for(int i = 0; i < q->size-1; i++){
        q->tasks[i] = q->tasks[i+1];
    }
    q->size--;
    return temp;
}

int Dequeue(task_queue *q, task to_remove) {
    if ( q->size == 0 ) {
        printf("Task Queue Empty");
        return -1;
    }

    task empty_task = {"", -1};

    for (int i=0; i<q->size; i++ ) {
        if ( strcmp(q->tasks[i].name, to_remove.name) == 0 ) {

            q->tasks[i] = empty_task;
            // printf("Task: %s (has been deleted)\n", q->tasks[i].name); // For testing

            for(int j=i; j<q->size-i; j++) {
                q->tasks[j] = q->tasks[j+1];
            }
            q->size--;

            return 0;
        }
    }
    return -1;
}
