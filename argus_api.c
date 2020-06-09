#include "argus_api.h"

void export_task(Task t) {
    int out_fd = open("Logs/task.log",O_CREAT | O_APPEND | O_RDWR,0666);
    char buffer[128];
    int id = t->ID;
    int stats = t->status;
    int execution = t->max_execution;
    int tstamp = t->time_stamp;
    char* cmds = strdup(t->commands);
    int written = sprintf(buffer,"%d;%d;%s;%d;%d\n",id,stats,cmds,execution,tstamp);
    write(out_fd,&buffer,written);
    close(out_fd);
}

Task read_task(char* buffer) {
    Task res = malloc(sizeof(Task));
    char* temp = strdup(buffer);
    char* args = malloc(sizeof(char*));
    int i=0;
    while ((args = strsep(&temp,";"))!=NULL) {
        switch(i) {
            case 0:
            res->ID = atoi(args);
            break;

            case 1:
            res->status = atoi(args);
            break;
            
            case 2:
            res->commands = strdup(args);
            break;

            case 3:
            res->max_execution = atoi(args);
            break;

            case 4:
            res->time_stamp = atoi(args);
            break;
            
            default:
            break;
        }
        i++;
    }
    return res;
}
