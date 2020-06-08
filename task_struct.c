#include "task_struct.h"


void print_Task(Task t) {
    printf("Task ID: %d\n",t->ID);
    printf("Task status: %d\n",t->status);
    printf("Task commands: %s\n",t->commands);
    printf("Task  maximum execution time (s): %d\n",t->max_execution);
   // char* times = ctime(&t->time_stamp);
   printf("Task timestamp: %d\n",t->time_stamp);
}

Task init_Task() {
    Task result = malloc(sizeof(Task));
    result->ID=1;
    result->status = 0;
    result->commands = "";
    result->max_execution = 0;
    result->time_stamp = (int) time(NULL);
    return result;
}

Task create_Task(int id,int status,char* cmds,int mexec) {
    Task result_task = malloc(sizeof(Task));
    result_task->ID = id;
    result_task->status = status;
    result_task->commands = strdup(cmds);
    result_task->max_execution = mexec;
    result_task->time_stamp = (int) time(NULL);
    return result_task;
}

Task add_commands(Task t,char* cmds) {
    Task result = t;
    result->commands = strdup(cmds);
    return result;
}

Task change_status(Task t, int new_status) {
    Task result = t;
    result->status=new_status;
    return result;
}

Task change_maximum_time(Task t, int maximum) {
    Task result = t;
    result->max_execution = maximum;
    return result;
}

char* parse_status(int stat) {
    char* result = malloc(sizeof(char)*25);
    switch(stat) {
        case 0:
            result = "Executed";
            break;
        case 1:
            result = "In-Execution";
            break;
        case -1:
            result = "Error";
            break;
        case 2:
            result = "Aborted";
            break;   
        default:
            break;
    }

    return result;
}
/*

int main() {

    Task nt = init_Task();
    nt = add_commands(nt,"ls | wc");
    nt = change_maximum_time(nt,20);
    nt = change_status(nt,0);
    print_Task(nt);
    char* stat = parse_status(nt->status);
    printf("Status: %s\n",stat);

    time_t current_time = time(NULL);
    char* c_time_string = ctime(&current_time);
    printf("%s",c_time_string);

    return 0;
}

*/