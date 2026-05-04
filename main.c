#include <stdio.h>
#include <string.h>
#include <time.h> // Added because ctime() is used in display()
#include "queue.h"
#include "undo.h"
#include "txt_utils.h"

#define VERSION "0.2.0-tagging"

void print_usage(char *prog_name) {
    printf("Usage: %s [OPTIONS]\n", prog_name);
    printf("Options:\n");
    printf("  -h, --help    Show this help message\n");
    printf("  -v, --version  Show version information\n");
}

void print_version() {
    printf("Task Scheduler Version: %s\n", VERSION);
}

void display(task_queue *q, Tag filter) {
    if (filter == -1) {
            printf("\n--- Full Task Schedule ---\n");
        } else {
            const char* tagNames[] = {"Uncategorized", "Work", "Home", "Personal", "School"};
            printf("\n--- Task Filter View (Category: %s) ---\n", tagNames[filter]);
        };

    int found = 0;
    for (int i = 0; i < q->size; i++) {
        char *readable_time = ctime(&q->tasks[i].deadline);
        // If filter is -1, show all tasks
        if (filter == -1 || q->tasks[i].tag == filter) {
            printf("%s - Due: %s\n", q->tasks[i].name, readable_time);
            found = 1;
        }
    }
    if (!found) printf("No tasks found for this category.\n");
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            // Check for help or version flags
            if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                print_usage(argv[0]);
                return 0;
            } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
                print_version();
                return 0;
            } else {
                printf("Unknown option: %s\n", argv[i]);
                print_usage(argv[0]);
                return 1;
            }
        }
    }

    int choice;
    task temp, task_to_delete;
    struct Stack undo_stk;
    task_queue tq;

    Initialize(&tq);
    Initialize_Stack(&undo_stk);
    load_tasks(&tq);

    while (1) {
        printf("\n--- Task Management System ---\n");
        printf("1. Add Task\n2. Undo\n3. Show Schedule\n4. Search by Tag\n5. Exit\nChoice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while(getchar() != '\n');
            continue;
        }
        getchar();

        switch (choice) {

            case 1:
                printf("Enter task name: ");
                fgets(temp.name, 50, stdin);
                temp.name[strcspn(temp.name, "\n")] = 0;

                time_t now = time(NULL);
                struct tm *current_time = localtime(&now);

                struct tm human_time = {0};
                human_time.tm_year = current_time->tm_year;

                int input_month, valid = 0;
                printf("--- Enter Deadline ---\n");

                while (!valid) {
                    do {

                        printf("Month (1-12): ");
                        if ( scanf("%d", &input_month) != 1 || input_month < 1 || input_month > 12 ) {
                            while(getchar() != '\n'); // Clear buffer
                        } else {
                            human_time.tm_mon = input_month - 1;
                            break;
                        }

                    } while (1);

                    do {

                        printf("Day (1-31): ");
                        if (scanf("%d", &human_time.tm_mday) != 1 || human_time.tm_mday < 1 || human_time.tm_mday > 31) {
                            while(getchar() != '\n');
                        } else {
                            break;
                        }

                    } while (1);

                    do {

                        printf("Hour (0-23): ");
                        if (scanf("%d", &human_time.tm_hour) != 1 || human_time.tm_hour < 0 || human_time.tm_hour > 23) {
                            while(getchar() != '\n');
                        } else {
                            break;
                        }

                    } while (1);

                    temp.deadline = mktime(&human_time);

                    if (temp.deadline == -1) {
                        printf("Error: The date entered is invalid. Please try again.\n");
                    } else {
                        valid = 1; // Exit the main loop
                    }
                }

                int tag_input;
                printf("Select Tag (0. Uncategorized, 1. Work, 2. Home, 3. Personal, 4. School) :");
                scanf("%d", &tag_input);
                temp.tag = (tag_input >= 0 && tag_input <= 4) ? (Tag)tag_input : UNCAT;

                while(getchar() != '\n');

                Insert(temp, &tq);
                printf("Task added to schedule!\n");

                push(&undo_stk, temp);
                save_task(temp);

                break;

            case 2:
                task_to_delete = pop(&undo_stk);
                Deletion(&tq, task_to_delete);
                remove_task(task_to_delete);
                break;

            case 3:
                display(&tq, -1);
                break;

            case 4:
                printf("Filter by Tag [0. Uncategorized, 1. Work, 2. Home, 3. Personal, 4. School]: ");
                int filter;
                while ( scanf("%d", &filter) != 1 || filter < 0 || filter > 4 ) {
                    printf("Filter by Tag [0. Uncategorized, 1. Work, 2. Home, 3. Personal, 4. School]: ");
                    while ( getchar() != '\n' );
                }
                display(&tq, filter);
                break;

            case 5:
                Free_Queue(&tq);
                Free_Stack(&undo_stk);
                return 0;

            default:
                printf("Please try again.\n");
        }
    }

    // In case the loop breaks unexpectedly
    Free_Queue(&tq);
    Free_Stack(&undo_stk);
    return 0;
}
