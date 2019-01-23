/**
*  Example program demonstrating UNIX pipes.
*
*  Figures 3.25 & 3.26
*
*  @author Silberschatz, Galvin, and Gagne
*  Operating System Concepts  - Ninth Edition
*  Copyright John Wiley & Sons - 2013
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int wordcount(char ch[]){
	int count = 0;
	for(int i = 0; i < BUFFER_SIZE; i++){
		if (ch[i] == ' ') count++;
	}
	printf("Word Count: %i\n", count);

	return (count == 0) ? 0 : count; 
}

int main(int argc, char *argv[])
{
	char write_msg[BUFFER_SIZE] = "1 2 3 4 5 6 7 8 9 10";
	char read_msg[BUFFER_SIZE];
	pid_t pid;
	int fd[2];
	

	/* argc must be 2 for correct execution */    
	if ( argc != 2 ) {
        /* Assume that argv[0] is the program name */
        printf( "usage: %s, <file_name>", argv[0]);
        return 0; 
    }

	/* create the pipe */
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
	
	/* now fork a child process */
	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}
	
	if (pid > 0) {  /* parent process */
		/* close the unused end of the pipe */
		close(fd[READ_END]);
		
		/* write to the pipe */
		write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
		
		/* close the write end of the pipe */
		close(fd[WRITE_END]);
	}
	else { /* child process */
		/* close the unused end of the pipe */
		close(fd[WRITE_END]);
		
		/* read from the pipe */
		read(fd[READ_END], read_msg, BUFFER_SIZE);
		printf("child read %s\n",read_msg);
		
		/* close the write end of the pipe */
		close(fd[READ_END]);
	}
	
	wordcount(write_msg);

	return 0;
}