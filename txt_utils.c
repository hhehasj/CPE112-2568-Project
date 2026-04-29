#include <stdio.h>
#include "queue.h"
#include <string.h>
#include <time.h>

const char *TASKS_FILE = "tasks.txt";


void save_task(task new_task) {
    FILE *fileptr = fopen(TASKS_FILE, "a");

    if ( !fileptr ) {
        printf("This file doesn't exist\n");
        return;
    }

    printf("Saving to file: %s\n", new_task.name);
    fprintf(fileptr, "%s,%ld,%d\n", new_task.name, (long)new_task.deadline, new_task.tag);

    fclose(fileptr);
}


void remove_task(task task_to_remove) {
    FILE *original = fopen(TASKS_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if ( !original || !temp ) {
        printf("Either or both of these files do not exist.");
        return;
    }

    char line[1024];
    while ( fgets(line, sizeof(line), original) ) {
        char task_name[500];
        long timestamp;
        int tag_num;

        if ( sscanf(line, "%499[^,],%ld,%d", task_name, &timestamp, &tag_num) == 3 ) {
            if ( strcmp(task_name, task_to_remove.name) != 0 ) {
                fprintf(temp, "%s,%ld,%d\n", task_name, (long)timestamp, tag_num);
            } else {
                printf("Removed from file: %s\n", task_name);
            }
        }
    }

    fclose(original);
    fclose(temp);

    remove(TASKS_FILE);
    rename("temp.txt", TASKS_FILE);

}


void load_tasks(task_queue *q) {
    FILE *fileptr = fopen(TASKS_FILE, "r");

    if ( fileptr == NULL ) {
        printf("This file doesn't exists\n");
        return;
    }

    printf("Loading tasks from file\n");

    char line[1024];
    while ( fgets(line, sizeof(line), fileptr ) ) {

        task inserted_task;
        long timestamp;
        int tag_num, index = 0;

        if ( sscanf(line, "%499[^,],%ld,%d", inserted_task.name, &inserted_task.deadline, &inserted_task.tag) == 3 ) {
            Insert(inserted_task, q);
        }
    }

    fclose(fileptr);
}
