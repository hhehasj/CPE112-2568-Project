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

// void displaySchedule(){
//     char *readable_time = ctime(&tq.tasks[i].deadline);//For printing the time
// }
void displaySchedule(task_queue *q) {
    printf("TASKS \n"); //TITLE

    for(int i = 0; i < q->size ;i++) {
        char *readable_time = ctime(&q->tasks[i].deadline);
        printf("%s -- %s\n",q->tasks[i].name, readable_time);
    }
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
    struct Stack undo_stk;
    task_queue tq;

    Initialize(&tq);
    Initialize_Stack(&undo_stk);

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

                int input_month;

                printf("--- Enter Deadline ---\n");

                printf("Month (1-12): ");
                scanf("%d", &input_month);
                human_time.tm_mon = input_month - 1;//Jan is 0

                printf("Day (1-31): ");
                scanf("%d", &human_time.tm_mday);

                printf("Hour (0-23): ");
                scanf("%d", &human_time.tm_hour);

                temp.deadline = mktime(&human_time);//Convert

                printf("Select Tag (0:Uncat, 1:Work, 2:Home, 3:Personal, 4:School): ");
                int t;
                scanf("%d", &t);
                temp.tag = (t >= 0 && t <= 4) ? (Tag)t : UNCAT;

                while(getchar() != '\n');

                Insert(temp, &tq);
                push(&undo_stk, temp);

                printf("Task added to schedule!\n");
                break;

            case 2:
                Deletion(&tq, pop(&undo_stk));
                break;

            case 3:
                displaySchedule(&tq);
                break;

            case 4:
                printf("didnt build yet");
                break;

            case 5:
                return 0;

            default:
                printf("Try again.\n");
        }
    }
    return 0;
}
