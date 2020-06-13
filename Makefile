CC=gcc
CFLAGS = -Wall -std=c11 -g  

FILES = task_struct.c argus_api.c argus_main.c argus_client.c 
SERVER_FILES = task_struct.c argus_server.c argus_api.c


argus: 
	$(CC) $(CFLAGS) -o argus $(FILES)

server:
	$(CC) $(CCFLAGS) -o server $(SERVER_FILES) 

clean:
	rm ./argus ./server
