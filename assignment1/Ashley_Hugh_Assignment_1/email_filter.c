#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>


#define SIZE 1024
int main(int argc, char *argv[]){



	char *buf =  malloc(SIZE);
	char *token = malloc(SIZE);
	char *action = malloc(SIZE);
	char *title = malloc(SIZE);
	char *date = malloc(SIZE);
	char *time = malloc(SIZE);
	char *location = malloc(SIZE);
	static char *wsbuf = "        ";
	size_t bufsz;
	ssize_t line_in_size;

	//get line from stdin, get length and store in buffer
	line_in_size = getline(&buf, &bufsz, stdin);

	//check for EOF condition
	while (line_in_size >= 0){

			//printf("%s\n", buf);
			token = strchr(buf, ':');
			//check for white space between "subject:" and action flag
			if (isspace(token[1]) == 0){
				line_in_size = getline(&buf, &bufsz, stdin);
				continue;
			}

			//printf("%s\n", token);
			token = strtok(token, ": ,");
			//printf("%s\n", token);
			//copy current token to action variable
			strcpy(action, token);

			//printf("%s\n", action);
			//printf("%lu\n",strlen(action));
			//make sure subject is a calendar action and not something arbitrary
			if(strlen(action)>1){
				line_in_size = getline(&buf, &bufsz, stdin);
				continue;
			};
			//get next token
			token = strtok(NULL, ",");
			//filter bad inputs
			if(token==NULL){
				line_in_size = getline(&buf, &bufsz, stdin);
				continue;
			};
			//copy current token to title variable
			strcpy(title, token);
			//get next token
			token = strtok(NULL, ",");
			//filter bad inputs
			if(token==NULL){
				line_in_size = getline(&buf, &bufsz, stdin);
				continue;
			};
			//copy current token to date variable
			strcpy(date,token);
			//get next token
			token = strtok(NULL, ",");
			//filter bad inputs
			if(token==NULL){
				//get next line
				line_in_size = getline(&buf, &bufsz, stdin);
				continue;
			};
			//copy current token to time variable
			strcpy(time,token);
			//get next token
			token = strtok(NULL, ",");
			if(token==NULL){
				//get next line
				line_in_size = getline(&buf, &bufsz, stdin);
				continue;
			};
			//copy current token to location variable
			strcpy(location,token);

			//add necessary white space after title to bring field to 10 chars
			if (strlen(title)<10){
			strncat(title, wsbuf, 10-strlen(title));
			};
			//add necessart white space after location to bring field to 10 chars
			if (strlen(location)<10){
			strncat(location, wsbuf, 10-strlen(location));
			};
			//print formatted calendar event to stdout
			printf("\r");
			printf("%s,%s,%s,%s,%s", action, title, date, time, location);


			//get next line and size of line
		line_in_size = getline(&buf, &bufsz, stdin);
	};
	//free variables
	free(buf);
	free(action);
	free(title);
	free(date);
	free(time);
	free(location);

	return 0;
};
