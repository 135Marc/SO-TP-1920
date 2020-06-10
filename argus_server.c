#include "argus_api.h"

int fd;

void sig_handler1(int signum) {
    if (signum == SIGINT) {
        printf("Fui tratado pelo sig handler!\n");
        close(1);
        close(fd);
    }
}


// TA ESCAXADA!!!
char** separate_commands(char* cmds) {
    char* pipe_number = strchr(cmds,'|');
    int size = strlen(pipe_number);
    char** result = malloc(sizeof(char*)*(size+1)); 
    char* temp = strdup(cmds);
    char* args = malloc(sizeof(char*));
    int i=0;
    while ((args = strsep(&temp,"|"))!=NULL) {
            result[i] = strdup(args);
            printf("SEPARATED: %s\n",result[i]);
           i++;
        }
    return &result;
}

void read_fifo() {
    fd = open("FIFOs/client_fifo",O_RDWR,0666);
	if(fd<0) {
       // perror("Erro na abertura do pipe!\n");
        int make = mkfifo("FIFOs/client_fifo",0666);
        if(make<0) perror("Erro na criação do pipe!\n");
    }
	char buf[1024];
	int r,status;
    char command[1024];
	while ((r=read(fd,buf,1024))>0) {
        write(1,buf,r);
        buf[r]='\0';
        memcpy(command,buf,r-1);
        command[r-1]='\0';
     //   printf("BUFFER: %s",buf);
        char* pipe_number = strchr(buf,'|');
        char** results;
        if (pipe_number!=NULL) {
            results = separate_commands(buf);
            if (results) printf("RESULTS: %s",results);
        }   
      /*  
        int f = fork();
        if (!f) {
            
            printf("COMMAND CONTENT: %s\n COMMAND SIZE: %d\n",command,strlen(command));
            execlp(command,"",NULL);
            _exit(0);
        }
        else wait(&status);
    }
    */
    }
}


int main () {
    signal(SIGINT,sig_handler1);
	read_fifo();
	return 0;
}
