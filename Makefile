CC=gcc
CFLAGS = -Wall -std=c11 -g  

argus: 
	$(CC) $(CFLAGS) -o argus ./*.c

clean:
	rm ./argus
