#include "argus_api.h"

void sig_handler(int signum) {
    if (signum == SIGINT) {
        close(0);
        printf("Fui tratado pelo sig handler!\n");
    }
}

void write_fifo() {
	int fd = open("FIFOs/client_fifo",O_WRONLY,0666);
    if (fd<0) perror("Erro na abertura do pipe!\n");

    char buf[1024];
	int r; 
	
	while ((r=read(0,&buf,1024))>0) {
        write(fd,&buf,r);
    }
}

void write_command_fifo(char* cmd) {
    int fd = open("FIFOs/client_fifo",O_WRONLY,0666);
    if (fd<0) {
        perror("Erro na abertura do pipe!\n");
    }
    char* buf = strdup(cmd);
	write(fd,buf,strlen(buf));
    close(fd);
}
