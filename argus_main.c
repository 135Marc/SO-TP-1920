#include "./argus_api.h"

Task current;
int bash_mode=0;

void printMenu() {
    char* inact = malloc(100);
    char* maxtime = malloc(100);
    char* execute = malloc(100);
    char* list = malloc(100);
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
    char* string = (char*) malloc(128);
    sprintf(string,"END %d",id);
    write_command_fifo(string); // Mandada como header, o servidor recebe , vê se está em execução e se estiver,mata
}

void list_Tasks() { // Ler do ficheiro de tarefas as que ainda estão em execução
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

void show_History() { // Ler do ficheiro de tarefas terminadas
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
     int tid = get_last_task_ID() + 1;
     printf("nova tarefa #%d\n",tid);
     write_command_fifo(argv[2]);
}

void set_Inactivity_Time(char* argv[]) {
        char* string = (char*) malloc(128);
        sprintf(string,"INACTIVE-TIME %s",argv[2]);
        write_command_fifo(string);
}

void set_Task_Max_Time(char* argv[]) {
        char* string = (char*) malloc(128);
        sprintf(string,"MAX-TIME %s",argv[2]);
        write_command_fifo(string);
}

void parse_options(char* argv[]) {

    if (!strcmp(argv[1],"-h")) printMenu(); // DONE
    if (!strcmp(argv[1],"-t")) terminate_Task(atoi(argv[2])); // AFTER NEXT
    if (!strcmp(argv[1],"-r")) show_History(); // DONE
    if (!strcmp(argv[1],"-l")) list_Tasks(); // DONE
    if (!strcmp(argv[1],"-m")) set_Task_Max_Time(argv); // IN-PROGRESS
    if (!strcmp(argv[1],"-e")) execute_Task(argv); // IN-PROGRESS
    if (!strcmp(argv[1],"-i")) set_Inactivity_Time(argv); // TO-DO


}

void enter_bash() {
    int fifo_fd = open("FIFOs/client_fifo",O_WRONLY,0666);
    int bytes = 0;
    char buf[1024];
    char c = '$';
    char sp = 32;
    char argus[] = "argus";
    write(1,argus,5);
    write(1,&c,1);
    write(1,&sp,1);
    while ((bytes=read(0,buf,1024))>0) {
        if (strchr(buf,'|')) {
            char* taskformat = (char*) malloc(20);
            int written = sprintf(taskformat,"nova tarefa #%d\n",get_last_task_ID()+1);
            write(1,taskformat,written);
        }
        write(1,argus,5);
        write(1,&c,1);
        write(1,&sp,1);
        buf[bytes-1]='\0';
        if (!strcmp(buf,"historico")) show_History();
        else if (!strcmp(buf,"ajuda")) printMenu();
        else write_command_fifo(buf);
    }
}

int main(int argc,char* argv[]) {

    if (argv[1]!=NULL) parse_options(argv);
    else enter_bash();

    return 0;
}
