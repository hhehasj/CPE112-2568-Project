#ifndef QUEUE_H
#define QUEUE_H
#include <time.h>

typedef enum { UNCAT = 0, WORK, HOME, PERSONAL, SCHOOL } Tag;

typedef struct {
    char name[50];
    time_t deadline;
    Tag tag;
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
