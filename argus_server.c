#include "argus_api.h"

int fd; 

void sig_handler1(int signum) {
    if (signum == SIGINT) {
        printf("Fui tratado pelo sig handler!\n");
        close(1);
        close(fd);
    }
}

void read_fifo() {
    fd = open("FIFOs/client_fifo",O_RDWR,0666);
	if(fd<0) {
       // perror("Erro na abertura do pipe!\n");
        int make = mkfifo("FIFOs/client_fifo",0666);
        if(make<0) perror("Erro na criação do pipe!\n");
    }
	char buf[1024];
	int r;
    int acc=0;
	while ((r=read(fd,&buf,1024))>0) {
        write(1,buf,r);
    }
  //  close(1);
 //   close(fd);

}


int main () {
    signal(SIGINT,sig_handler1);
	read_fifo();
	return 0;
}
