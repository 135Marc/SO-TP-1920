#include "./argus_api.h"

Task current;

void printMenu() {
    printf("-i t1 \t\t\t communication inactivity time, in seconds\n");
    printf("-m t1 \t\t\t maximum task execution time, in seconds\n");
    printf("-e \"cmd1 | cmd2 | cmd3\" \t\texecutes commands, in pipeline fashion and creates respective tasks\n");
    printf("-l \t\t\t list tasks\n");
    printf("-t taskID \t\t terminates a task, given its ID\n");
    printf("-r \t\t\t task history\n");
    printf("-h \t\t\t show help\n");
    return;
}

void terminate_Task(int id) {
    printf("Recebi o ID %d para terminar a tarefa!\n",id);
    // NEXT

    
}

void list_Tasks() {
    int task_fd = open("Logs/task.log",O_RDONLY,0666);
    char buffer[1024];
    char* temp;
    char* args = malloc(sizeof(char*));
    int r=0;
    while ((r=read(task_fd,&buffer,1024))>0) {
        temp = strdup(buffer);
        while ((args = strsep(&temp,"\n"))!=NULL) {
            if (args[0]>=48 && args[0]<=57) {
                current = read_task(args);
                if (current->status==1) print_Task(current);
                free(current);
            }
        }
    }
}

void show_History() {
    // Ler do ficheiro de tarefas já executadas
    int task_fd = open("Logs/task.log",O_RDONLY,0666);
    char buffer[1024];
    char* temp;
    char* args = malloc(sizeof(char*));
    int r=0;
    while ((r=read(task_fd,&buffer,1024))>0) {
        temp = strdup(buffer);
        while ((args = strsep(&temp,"\n"))!=NULL) {
            if (args[0]>=48 && args[0]<=57) {
                current = read_task(args);
                int stat = current->status;
                if (stat==0||stat==-1||stat==2) print_Task(current);
                free(current);
            }
        }
    }


}

void execute_Task(char* argv[]) {
   // printf("Recebi uma tarefa com os comandos -> %s\n",argv[2]);
    if (current!=NULL) {
        // Vai ter outras coisas aqui (...)
        current = change_status(current,1);
        current = add_commands(current,argv[2]);
        print_Task(current);
    }
}

void set_Inactivity_Time(char* argv[]) {
    printf("Tempo de inactividade de comunicação : %d\n",atoi(argv[2]));
}

void set_Task_Max_Time(char* argv[]) {

    if (current!=NULL) {
        // Vai ter outras coisas aqui (...)
        current = change_maximum_time(current,atoi(argv[2]));
        print_Task(current);
    }
}

void parse_options(char* argv[]) {

    if (!strcmp(argv[1],"-h")) printMenu(); // DONE
    if (!strcmp(argv[1],"-t")) terminate_Task(atoi(argv[2])); // TO-DO
    if (!strcmp(argv[1],"-r")) show_History(); // DONE
    if (!strcmp(argv[1],"-l")) list_Tasks(); // DONE
    if (!strcmp(argv[1],"-m")) set_Task_Max_Time(argv); // TO-DO
    if (!strcmp(argv[1],"-e")) execute_Task(argv); // TO-DO
    if (!strcmp(argv[1],"-i")) set_Inactivity_Time(argv); // TO-DO
    
}

int main(int argc,char* argv[]) {

   // current = malloc(sizeof(Task));
   // current = init_Task();
   // current = add_commands(current,"ps | ifconfig");
   // export_task(current);
    if (argv[1]!=NULL) parse_options(argv);
    
    return 0;
}
