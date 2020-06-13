#include "argus_api.h"

int fd;
Task current_Task;

int max_task_time=-1;
int max_inactivity_time=-1;
int task_timer=0;
int inactivity_timer=0;

int active_processes=0;

void sig_handler1(int signum) {
    if (signum == SIGINT) {
        printf("Fui tratado pelo sig handler!\n");
        close(1);
        close(fd);
    }
    
    if (signum == SIGALRM) {
        // Verificar se todos os processos terminaram, continuar em loop a contar o tempo
    }

    if (signum == SIGCHLD) {
        // Contar todos os processos-filho terminados!
        if (active_processes > 0) active_processes--;
        else {
            printf("TODOS OS PROCESSOS FORAM TERMINADOS!\n");
            active_processes=0;
        }
    }

    if (signum == SIGPIPE) {
        // Último a ser tratado
    }


}

int elem_string(char key,char* string) {
    char* temp = strdup(string); // Será necessário?
    int occur=0;
    for (int i=0;temp[i]!=NULL;i++) (temp[i]==key) ? occur+=1 : occur;
    return occur;

}


// Reformulada!
void process_pipes(char* cmds) {
    int size = elem_string('|',cmds);
    char* temp = strdup(cmds);
    char* args = malloc(sizeof(char*));
    int current_pipe=0;
    int index=0;
    int ind =0;
    int status;
    while ((args = strsep(&temp," "))!=NULL) {
            index+=1;
            if (size==1) { // Apenas 1 pipe
                char* cmd_vector[1024];
                char* arg_vector[1024];
                if (strcmp(args,"|")!=0) {
                    if (elem_string('-',args)) {
                        arg_vector[ind] = strdup(args);
                    }
                    else cmd_vector[current_pipe] = strdup(args);
                    
                }
                else current_pipe+=1;
                if (index==(2*size)+1) { // Se tiver argumentos nos comandos, escaxa
                int pipes[2];
                pipe(pipes);
                int f1 = fork();
                    if (!f1) {
                        active_processes++;
                        int f2=fork();
                        if (!f2) {
                            active_processes++;
                            close(pipes[0]);
                            dup2(pipes[1],1);
                            close(pipes[1]);
                            execlp(cmd_vector[0],"",NULL);
                        }
                        close(pipes[1]);
                        dup2(pipes[0],0);
                        close(pipes[0]);
                        execlp(cmd_vector[1],"",NULL);   
                    }
                    else {
                            close(pipes[1]);
                            close(pipes[0]);
                            wait(&status);
                        }     
                 }
            }
                // FIM DO PROCESSAMENTO DE 1 PIPE */
            
            else { // PIPE > 1
                char *comands_vector[1024];
                if (strcmp(args,"|")!=0) comands_vector[current_pipe] = strdup(args);                    
                else current_pipe+=1;

                if(index==(size*2)+1) { // AQUI JÁ ESTÁ NO FIM!
                      int multi_pipe[size][2];
                      int var =0;
                      for(int c =0;c<size;c++) pipe(multi_pipe[c]);
                            for(int z=0;z<size;z++) {
                                int f_1 = fork();
                                if (!f_1)  {                            
                                
                                if (z!=size-1) { // Não lançamos este processo caso estejamos no último pipe, apenas lemos o output anterior!
                                  int f_2 = fork();
                                  if (!f_2) {
                                  
                                    if (z==0) { // Se estivermos na primeira iteração, escrevemos o output para o extremo de escrita do 1º pipe
                                        close(multi_pipe[z][0]);
                                        dup2(multi_pipe[z][1],1);
                                        close(multi_pipe[z][1]);
                                        execlp(comands_vector[z],"",NULL); }
                            
                                  
                                  else { // Caso contrário redirecionamos o stdout para o extremo de escrita do proximo pipe, lemos do stdin o output gerado anteriormente
                                        dup2(multi_pipe[z+1][1],1);
                                        close(multi_pipe[z+1][1]);
                                        dup2(0,multi_pipe[z][1]);
                                        dup2(multi_pipe[z][0],0);
                                        execlp(comands_vector[z],"",NULL);}
                                    }

                                }

                                if (z==0) { // Iteração 0: Escrevemos o output atual para o extremo de escrita do pipe seguinte 
                                    close(multi_pipe[z][1]);
                                    dup2(multi_pipe[z+1][1],1);
                                    dup2(multi_pipe[z][0],0);
                                    close(multi_pipe[z][0]);
                                    execlp(comands_vector[z+1],"",NULL);
                                }
                                
                                else if (z==size-1) { // Iteração final: Lemos o output do encadeamento, executamos o último programa
                                        dup2(0,multi_pipe[z][1]);
                                        dup2(multi_pipe[z][0],0);
                                        close(multi_pipe[z][0]);
                                        execlp(comands_vector[z+1],"",NULL);
                                }

                                else { // Iteração 0 até ao fim:  Ler (stdin) do extremo de escrita do pipe seguinte, preencher o extremo de escrito do pipe atual
                                        close(multi_pipe[z][0]);
                                        dup2(0,multi_pipe[z+1][1]);
                                        dup2(multi_pipe[z][1],1);
                                        close(multi_pipe[z][1]);
                                        execlp(comands_vector[z+1],"",NULL);
                                }

                            }
                            else {
                                // Fechar descritores de escrita e leitura do pipe atual, no processo-pai
                                close(multi_pipe[z][1]);
                                close(multi_pipe[z][0]);
                            }                             
                        }

                        for(int c1=0;c1<size;c1++) { // Fechar todos os descritores
                            close(multi_pipe[c1][1]);
                            close(multi_pipe[c1][0]);}
             }                    
         } 
     }
}


void process_headers(char* cmd_headers) {
    char* temp = strdup(cmd_headers);
    char* args = malloc(sizeof(char*));
    int is_max=0;
    int is_inact=0;
     while ((args = strsep(&temp," "))!=NULL) { 
         if (!strcmp("MAX-TIME",args)) is_max=1;
         if (!strcmp("INACTIVE-TIME",args)) is_inact=1;
         if (is_max) max_task_time = atoi(args);
         if (is_inact) max_inactivity_time = atoi(args);
    }
    printf("\nMAX TASK TIME: %d MAX INACTIVITY TIME: %d\n",max_task_time,max_inactivity_time);

}


void read_fifo() {
    fd = open("FIFOs/client_fifo",O_RDWR,0666);
	if(fd<0) {
        int make = mkfifo("FIFOs/client_fifo",0666);
        if(make<0) perror("Erro na criação do pipe!\n");
    }
	char *buf = (char*) malloc(1024);
	int r,status;
	while ((r=read(fd,buf,1024))>0) {
        // PENSAR QUANDO O BUFFER ENCHER, REALOCAR
        write(1,buf,r);
        buf[r]='\0';
        if(strstr(buf,"MAX-TIME") || strstr(buf,"INACTIVE-TIME")) process_headers(buf);
        int pipe_number = elem_string('|',buf);
        if (pipe_number>=1) process_pipes(buf);
        else {   
            int f = fork();
            // PENSAR NISTO COM ARGUMENTOS!!
            (!f) ? execlp(buf,"",NULL) : wait(&status);}
    }
}


int main () {
    signal(SIGINT,sig_handler1);
    signal(SIGCHLD,sig_handler1);
	read_fifo();
	return 0;
}
