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
    
}

void list_Tasks() {
    // Listar TODAS as tarefas!
    printf("Comando para listar tarefas invocado\n");
}

void show_History() {
    // Ler do ficheiro de tarefas já executadas
    printf("Histórico vai aqui!\n");
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

    if (!strcmp(argv[1],"-h")) printMenu();
    if (!strcmp(argv[1],"-t")) terminate_Task(atoi(argv[2]));
    if (!strcmp(argv[1],"-r")) show_History();
    if (!strcmp(argv[1],"-l")) list_Tasks();
    if (!strcmp(argv[1],"-m")) set_Task_Max_Time(argv);
    if (!strcmp(argv[1],"-e")) execute_Task(argv);
    if (!strcmp(argv[1],"-i")) set_Inactivity_Time(argv);
    
}

int main(int argc,char* argv[]) {

    current = malloc(sizeof(Task));
    current = init_Task();
    if (argv[1]!=NULL) parse_options(argv);
  //  Task nt = create_Task(1,1,argv[2],10);
  //  print_Task(nt);
   // Task t = init_Task();
   // print_Task(t);
    return 0;
}
