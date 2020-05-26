#include "./argus_api.h"


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

int main(int argc,char* argv[]) {
    if (!strcmp(argv[1],"-h")) printMenu();
    return 0;
}
