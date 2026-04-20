#ifndef QUEUE_H
#define QUEUE_H

typedef struct {
    char name[50];
    int deadline;
} task;

typedef struct {
    task tasks[100];
    int size;
} task_queue;


void Initialize(task_queue *q);
void Insert(task new_task, task_queue *q);
task Extract(task_queue *q);
int Deletion(task_queue *q, task to_remove);
#endif
