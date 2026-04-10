#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void Initialize(task_queue *q){
    q->size = 0;
}

void Insert(task new_task, task_queue *q){
    if(q->size >= 100){
        printf("TASK OVERLOAD");
        return;
    }
    int i = q->size-1;
    while(i >= 0){
        if(new_task.deadline < q->tasks[i].deadline){
            q->tasks[i + 1] = q->tasks[i];
            i--;
        }
        else{
            break;
        }
    }
    q->tasks[i+1] = new_task;
    q->size++;

}

task Extract(task_queue *q){
if(q->size == 0){
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
