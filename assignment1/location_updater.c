#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>







int main(int argc, char *argv[]){
//array for file descriptor used in read/write
	int fd[2];
//process identifier
	pid_t pid;
//init pipe with file descriptor array for communication between processes
	pipe(fd);
//split into parent/child processes
	pid = fork();


		if(pid==0){

			//child process to call calendar filter receiving input piped from email filter
			//close write side of file descriptor
			close(fd[1]);
			//replace stdin with read side of pipe using dup2.  saves one line of code over closing stdin and using dup.
			dup2(fd[0], 0);
			close (fd[0]);
			execl("./calendar_filter", (char*)NULL);
			wait(NULL);
		}

		else if (pid > 0){

			//parent process to call email filter to pipe stdin to calendar filter
			//close read side of file descriptor
			close(fd[0]);
			//replace stdout with write side of pipe using dup2.  saves one line of code over closing stdout and using dup.
			dup2(fd[1], 1);
			close (fd[1]);
			execl("./email_filter", (char*)NULL);
			wait(NULL);
		}

	return 0;
}
