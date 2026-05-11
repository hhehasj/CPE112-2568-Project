#include "tree.h"
#include <stdlib.h>
#include <string.h>

void Insert(task *new_task, struct task_tree **root){
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
    while ( root && root->left != NULL ) {
        root = root->left;
    }
    return root;
}

void Deletion(struct task_tree **root, task to_remove) {
    if ( *root == NULL ) {
        return;
    }

    if ( to_remove.deadline < (*root)->tasks->deadline ) {
        Deletion(&(*root)->left, to_remove);

    } else if ( to_remove.deadline > (*root)->tasks->deadline ) {
        Deletion(&(*root)->right, to_remove);

    } else {

        if ( strcmp((*root)->tasks->name, to_remove.name) != 0 ) {
            Deletion(&(*root)->right, to_remove);
            return;
        }

        if ( !(*root)->left ) {
            struct task_tree *temp = (*root)->right;
            free((*root)->tasks);
            free(*root);
            *root = temp;

        } else if ( !(*root)->right ) {
            struct task_tree *temp = (*root)->left;
            free((*root)->tasks);
            free(*root);
            *root = temp;

        } else {
            struct task_tree *temp = inorder_successor((*root)->right);
            *((*root)->tasks) = *(temp->tasks);
            Deletion(&(*root)->right, *(temp->tasks));
        }
    }
}

void Free_Tree(struct task_tree *root) {
    if ( root == NULL ) {
        return;
    }

    Free_Tree(root->left);
    Free_Tree(root->right);
    free(root->tasks);
    free(root);
}
