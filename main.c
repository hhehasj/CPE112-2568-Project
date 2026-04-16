#include <stdio.h>
#include <string.h>
#include "queue.h"
#include "undo.h"

#define VERSION "1.2.0-priority"

void print_usage(char *prog_name) {
    printf("Usage: %s [OPTIONS]\n", prog_name);
    printf("Options:\n");
    printf("  -h, --help     Show this help message\n");
    printf("  -v, --version  Show version information\n");
}

void print_version() {
    printf("Task Scheduler Version: %s\n", VERSION);
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
    task temp;
    struct Stack stk;
    task_queue tq;

    Initialize(&tq);
    Initialize_Stack(&stk);

    while (1) {
        printf("\n--- Task Management System ---\n");
        printf("1. Add Task\n2. Undo\n3. Show Schedule\n4. Exit\nChoice: ");

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

                printf("Priority (1-5): ");
                scanf("%d", &temp.deadline);

                Insert( temp, &tq );
                push(&stk, temp);

                break;

            case 2:
                pop(&stk);
                break;

            // case 3:
            //     displaySchedule();
            //     break;

            case 4:
                return 0;

            default:
                printf("Try again.\n");
        }
    }
    return 0;
}
