#ifndef TREE_H
#define TREE_H
#include <time.h>

typedef enum { UNCAT = 0, WORK, HOME, PERSONAL, SCHOOL } Tag;

typedef struct {
    char name[50];
    time_t deadline;
    Tag tag;
} task;

struct task_tree {
    task *tasks;
    struct task_tree *left;
    struct task_tree *right;
};

void Insert(task *new_task, struct task_tree **root);
void Deletion(struct task_tree **root, task to_remove);
void Free_Tree(struct task_tree *root);

#endif
