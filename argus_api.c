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


char* task_To_String(Task t) {
    char* results = (char*) malloc(512);
    int id = t->ID;
    int stats = t->status;
    int execution = t->max_execution;
    int tstamp = t->time_stamp;
    char* cmds = strdup(t->commands);
    int written = sprintf(results,"%d;%d;%s;%d;%d\n",id,stats,cmds,execution,tstamp);
    return results;
}

int get_last_task_ID() {
    int last_id=0;
    int tasks_fd = open("Logs/task.log",O_RDONLY,0666);
    int bytes_read;
    int acc=0;
    char buf[1024];
    int filesize = lseek(tasks_fd, (off_t) 0, SEEK_END); //filesize is lastby +offset

    for (int i = filesize - 1; i >= 0; i--) { //read byte by byte from end
        
        lseek(tasks_fd, (off_t) i, SEEK_SET);
        int n = read(tasks_fd, buf, 1);
 
        if (n != 1) break;

        else {
            buf[n]='\0';
            if (!strcmp(buf,";")) acc+=1;
            else if (acc==4) {
                last_id = atoi(buf);
                break;
            }
        }
    }
    close(tasks_fd);
    return last_id;
}

void update_task_status(int task_id,int status) {
    int tasks_fd = open("Logs/task.log",O_RDWR,0666);
    int bytes_read=0;
    int acc=0;
    int collon_found=0;
    int id_found = 0;
    char buffer[512];
    while ((bytes_read=read(tasks_fd,buffer,1))) {
        
        if (!strcmp(buffer,";")) collon_found+=1;
        else if (!strcmp(buffer,"\n")) collon_found = 0;
        else if (atoi(buffer) == task_id && !collon_found) {
            id_found=1;
           
        }
        else if (collon_found && id_found) {
          //  printf("CURRENT TASK STATUS: %s REQUEST CHANGE: %d\n",buffer,status);
            write(tasks_fd,&status,1);
            break;
        }

        buffer[bytes_read] = '\0';
    }
}