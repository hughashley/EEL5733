#include "filter.h"


int main(int argc, char *argv[]){



	char *buf;
	char *token;
	char *action;
	char *subject;
	char *date;
	char *time;
	char *location;
	static char *wsbuf = "        ";
	size_t bufsz;
	ssize_t line_in_size;




	char comp[strlen("Subject:")+1];
	strcpy(comp,"Subject:");


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
			strcpy(subject, token);
			token = strtok(NULL, ",");
			strcpy(date,token);
			token = strtok(NULL, ",");
			strcpy(time,token);
			token = strtok(NULL, ",");
			strcpy(location,token);

//			if(token==NULL){
//				line_in_size = getline(&buf, &bufsz, stdin);
//				continue;
//			};

			strncat(subject, wsbuf, 10-strlen(subject));
			strncat(location, wsbuf, 10-strlen(location));

			printf("%s,%s,%s,%s,%s\n", action, subject, date, time, location);



		line_in_size = getline(&buf, &bufsz, stdin);
	};



	return 0;
};
