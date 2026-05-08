#include "tree.h"
#include <stdlib.h>
// #include <string.h>

void Insert(task *new_task, struct task_tree **root){
    // ------------------------------OLD----------------------------------
    // If the array is full, double its capacity dynamically
    // if(q->size >= q->capacity){
    //     q->capacity *= 2;
    //     q->tasks = (task*)realloc(q->tasks, q->capacity * sizeof(task));

    //     if (q->tasks == NULL) {
    //         printf("Memory reallocation failed!\n");
    //         return;
    //     }
    // }

    // int i = q->size-1;
    // while(i >= 0){
    //     if (new_task.deadline < q->tasks[i].deadline) {
    //         q->tasks[i + 1] = q->tasks[i];
    //         i--;
    //     } else {
    //         break;
    //     }
    // }

    // printf("Task added: %s\n", new_task.name); // Testing
    // q->tasks[i+1] = new_task;
    // q->size++;
    // ----------------------------OLD------------------------------------

    if ( *root == NULL ) {
        struct task_tree *newNode = (struct task_tree *)malloc(sizeof(struct task_tree));

        newNode->tasks = (task *)malloc(sizeof(task)); // Giving memory for task details
        *(newNode->tasks) = *new_task; // Unpacking new_task and putting the values inside newNode->tasks

        newNode->left = NULL;
        newNode->right = NULL;
        *root = newNode;

    } else {
        if ( new_task->deadline < (*root)->tasks->deadline ) {
            Insert(new_task, &(*root)->left);
        } else {
            Insert(new_task, &(*root)->right);
        }
    }

}

struct task_tree* inorder_successor(struct task_tree *root) {
    while ( root->right != NULL ) {
        root = root->left;
    }
    return root;
}

void Deletion(struct task_tree **root, task to_remove) {
    // ------------------------------OLD----------------------------------
    // if ( root->size == 0 ) {
    //     printf("Task Queue Empty\n");
    //     return -1;
    // }

    // task empty_task = {"", -1, UNCAT};

    // for (int i=0; i<root->size; i++ ) {
    //     if ( strcmp(root->tasks[i].name, to_remove.name) == 0 ) {
    //         printf("Task: %s (has been deleted)\n", root->tasks[i].name);

    //         for(int j=i; j<root->size-1; j++) {
    //             root->tasks[j] = root->tasks[j+1];
    //         }

    //         // Clear the last duplicate item and shrink size
    //         root->tasks[root->size - 1] = empty_task;
    //         root->size--;

    //         return 0;
    //     }
    // }
    // return -1;
    // ----------------------------OLD------------------------------------
    if ( *root == NULL ) {
        return;
    }

    if ( to_remove.deadline < (*root)->tasks->deadline ) {
        Deletion(&(*root)->left, to_remove);

    } else if ( to_remove.deadline > (*root)->tasks->deadline ) {
        Deletion(&(*root)->right, to_remove);

    } else {
        if ( !(*root)->left ) {
            struct task_tree *temp = (*root)->right;
            free(*root);
            *root = temp;

        } else if ( !(*root)->right ) {
            struct task_tree *temp = (*root)->left;
            free(*root);
            *root = temp;

        } else {
            struct task_tree *temp = inorder_successor((*root)->right);
            (*root)->tasks = temp->tasks;
            Deletion(&(*root)->right, *(temp->tasks));
        }
    }
}

void Free_Tree(struct task_tree *root) {
    // ------------------------------OLD----------------------------------
    // free(root->tasks);
    // root->tasks = NULL;
    // root->size = 0;
    // root->capacity = 0;
    // ----------------------------OLD------------------------------------
    if ( root == NULL ) {
        return;
    }

    Free_Tree(root->left);
    Free_Tree(root->right);
    free(root->tasks);
    free(root);
}
