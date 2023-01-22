#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


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


	line_in_size = getline(&buf, &bufsz, stdin);


	while (line_in_size >= 0){


			token = strchr(buf, ':');
			//printf("%s\n", token[-1]);
			token = strtok(token, ": ,");
			//printf("%s\n", token);
			strcpy(action, token);
			//printf("%s\n", action);
			//printf("%lu\n",strlen(action));
			if(strlen(action)>1){
				line_in_size = getline(&buf, &bufsz, stdin);
				continue;
			};
			token = strtok(NULL, ",");
			if(token==NULL){
				line_in_size = getline(&buf, &bufsz, stdin);
				continue;
			};
			strcpy(title, token);
			token = strtok(NULL, ",");
			if(token==NULL){
				line_in_size = getline(&buf, &bufsz, stdin);
				continue;
			};
			strcpy(date,token);
			token = strtok(NULL, ",");
			if(token==NULL){
				line_in_size = getline(&buf, &bufsz, stdin);
				continue;
			};
			strcpy(time,token);
			token = strtok(NULL, ",");
			if(token==NULL){
				line_in_size = getline(&buf, &bufsz, stdin);
				continue;
			};
			strcpy(location,token);


			if (strlen(title)<10){
			strncat(title, wsbuf, 10-strlen(title));
			};
			if (strlen(location)<10){
			strncat(location, wsbuf, 10-strlen(location));
			};

			printf("%s,%s,%s,%s,%s", action, title, date, time, location);



		line_in_size = getline(&buf, &bufsz, stdin);
	};



	return 0;
};
