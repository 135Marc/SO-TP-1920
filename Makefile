CC=gcc
CFLAGS = -Wall -std=c11 -g  

FILES = task_struct.c argus_api.c argus_main.c argus_client.c 

argus: 
	$(CC) $(CFLAGS) -o argus $(FILES)

clean:
	rm ./argus
