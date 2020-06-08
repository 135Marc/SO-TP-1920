#ifndef TASK_STRUCT_H_

#define TASK_STRUCT_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

typedef struct task *Task;

typedef struct task {
    int ID;
    int status;
    char* commands;
    int max_execution;
    int time_stamp;
} task;

Task init_Task();
Task create_Task(int id,int status,char* cmds,int mexec);
Task add_commands(Task t,char* cmds);
Task change_status(Task t, int new_status);
Task change_maximum_time(Task t, int maximum);
char* parse_status(int stat);
void print_Task(Task t);


#endif