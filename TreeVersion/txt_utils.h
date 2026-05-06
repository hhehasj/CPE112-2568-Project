#ifndef TXT_UTILS_H
#define TXT_UTILS_H
#include "tree.h"
#include <stdio.h>

void save_task(task new_task);
void remove_task(task task_to_remove);
void load_tasks(struct task_tree **root);
void create_txtFile(FILE *fileptr);

#endif
