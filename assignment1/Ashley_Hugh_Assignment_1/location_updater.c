#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
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

			//child process to call email filter receiving input piped from email filter

			//close read side of file descriptor
			close(fd[0]);
			//replace stdout with write side of pipe using dup2.  saves one line of code over closing stdout and using dup.
			dup2(fd[1], 1);
			//close file descriptor
			close (fd[1]);
			//execute email filter program in child to serve data to parent
			execl("./email_filter", "", (char*)NULL);
			//terminate child process
			_exit(EXIT_SUCCESS);
		}

		//else if (pid > 0){

			//parent process to call calendar filter to pipe stdin to calendar filter


			//close write side of file descriptor
			close(fd[1]);
			//replace stdin with read side of pipe using dup2.  saves one line of code over closing stdin and using dup.
			dup2(fd[0], 0);
			//close file descriptor
			close (fd[0]);
			//execute calendar filter in parent to consume from child
			execl("./calendar_filter", "", (char*)NULL);
			//wait for child process to finish
			wait(NULL);
			//terminate process
			exit(EXIT_SUCCESS);

}
