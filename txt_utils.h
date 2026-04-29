#ifndef TXT_UTILS_H
#define TXT_UTILS_H
#include "queue.h"

void save_task(task new_task);
void remove_task(task task_to_remove);
void load_tasks(task_queue *q);

#endif
