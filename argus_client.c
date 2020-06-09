#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>


void write_fifo() {
    int make = mkfifo("FIFOS/client_fifo",0666);
	if(make<0) perror("Erro na criação do pipe!\n");
	int fd = open("FIFOS/client_fifo",O_RDWR,0666); 
	if (fd<0) perror("Erro na abertura do pipe!\n");

    void* buf = malloc(1024);
	int r = read(fd,buf,1024); 

	
	while ((r=read(0,buf,1024))>0) {
        write(fd,buf,r);
        free(buf);
    }
}

int main() {
    write_fifo();
    return 0;
}