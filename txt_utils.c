#include "txt_utils.h"
#include <stdio.h>
#include "queue.h"
#include <string.h>
#include <time.h>

const char *TASKS_FILE = "tasks.txt";


void save_task(task new_task) {
    FILE *fileptr = fopen(TASKS_FILE, "a");

    if ( !fileptr ) {
        create_txtFile(fileptr);
        return;
    }

    printf("Saved to file: %s\n", new_task.name);
    fprintf(fileptr, "%s,%ld,%d\n", new_task.name, (long)new_task.deadline, new_task.tag);

    fclose(fileptr);
}


void remove_task(task task_to_remove) {
    FILE *original = fopen(TASKS_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if ( !original ) {
        create_txtFile(original);
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

    if ( !fileptr ) {
        create_txtFile(fileptr);
        return;
    }

    char line[1024];
    while ( fgets(line, sizeof(line), fileptr ) ) {

        task inserted_task;
        long temp_deadline;
        int tag_num, index = 0;

        if ( sscanf(line, "%499[^,],%ld,%d", inserted_task.name, &temp_deadline, &inserted_task.tag) == 3 ) {
            inserted_task.deadline = (time_t)temp_deadline;
            Insert(inserted_task, q);
            printf("Tasks from file successfully loaded\n");
        }
    }

    fclose(fileptr);
}

void create_txtFile(FILE *fileptr) {
    printf("%s doesn't exist\n", TASKS_FILE);

    char choice;
    int created = 0;

    while ( !created ) {
        printf("Would you like to create a new one? (y/n): ");
        scanf("%c", &choice);
        if ( choice == 'y' || choice == 'Y' )  {

            fileptr = fopen(TASKS_FILE, "w");
            if ( fileptr ) {
                printf("Successfully created file.\n");
                created = 1;
                fclose(fileptr);
            }

        } else if ( choice == 'n' || choice == 'N' ) {
            printf("File was not created.\n");
            created = 1;
        } else {
            printf("Invalid.\n");
        }
    }
}
