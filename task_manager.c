#include "argus_api.h"

typedef struct task_list{
    int position;
    Task current_task;
    struct task_list* prox;
} *Task_List;

void add_task_to_list(Task t) {
    // TO-DO
    Task_List result = malloc(sizeof(Task_List));

}

void remove_task_from_list(Task t) {
    // TO-DO
}

void print_task_history(Task t) {
    // TO-DO
}
