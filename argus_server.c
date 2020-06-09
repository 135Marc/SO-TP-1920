#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

void read_fifo() {
    int fd = open("FIFOS/client_fifo",O_RDONLY,0666);
	if(fd<0) perror("Erro na abertura do pipe!\n");

	void* buf = malloc(1024);
	int r;
	
	while ((r=read(0,buf,1024))>0) {
        printf("%s\n",(char*) buf); // Só para imprimir as mensagens que chegam do cliente, para já        
        //write(fd,buf,r);
    }
}


int main () {
	read_fifo();
	return 0;
}