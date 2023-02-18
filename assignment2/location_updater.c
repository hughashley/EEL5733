#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <semaphore.h>
#include "email_filter.h"
#include "calendar_filter.h"

#define SIZE 1024
int bufsize = 0;
static sem_t *sem;


struct event {
	char date[SIZE];
	struct tm time;
	char location[SIZE];
	char title [SIZE];

};

static void *email_filter(void *voidData){
	int value;
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


while (sem_getvalue(sem, &value)<10){






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

		sem_post(sem);


}
		return NULL;
}
static void *calendar_filter(void *voidData){


	struct event events[30];
	struct event earliest_event;
	struct event earliest_event1;
	struct tm working_tm;
	char *buf = malloc(SIZE);
	char *token = malloc(SIZE);
	char *action = malloc(SIZE);
	char *title = malloc(SIZE);
	char *date = malloc(SIZE);
	char *time = malloc(SIZE);
	char *time1 = malloc(SIZE);
	char *location = malloc(SIZE);
	int year = 0000;
	int month = 00;
	int day = 00;
	int hour = 00;
	int min = 00;
	int count = 0;
	size_t bufsz;
	ssize_t line_in_size;

while(1){
	sem_wait(sem);
		//get line and store size
		line_in_size = getline(&buf, &bufsz, stdin);

		//check for EOF condition
		while (line_in_size >= 0){



				//tokenize line from stdin and store in corresponding variables
				token = strtok(buf, ",");
				strcpy(action, token);
				token = strtok(NULL, ",");
				strcpy(title, token);
				token = strtok(NULL, ",");
				strcpy(date,token);
				sscanf(date, "%d/%d/%d", &month, &day, &year);
				token = strtok(NULL, ",");
				strcpy(time,token);
				sscanf(time, "%d:%d", &hour, &min);
				token = strtok(NULL, ",");
				strcpy(location,token);






				//check for calendar action
				if (strrchr(action, 'C')){
					//populate event struct with incoming event
					strcpy(events[count].title, title);
					events[count].time.tm_hour = hour;
					events[count].time.tm_min = min;
					events[count].time.tm_year = year-1900;
					events[count].time.tm_mon = month-1;
					events[count].time.tm_mday = day;
					events[count].time.tm_sec = 0;
					events[count].time.tm_isdst = 0;
					strcpy(events[count].location, location);
					//loop to determine which event to print
					for (int i = 0; i <= count; i++){
						//create string representing date of event
						strftime(time, SIZE, "%m/%d", &events[count].time);
						strftime(time1, SIZE, "%m/%d", &events[i].time);
						//printf("date 1: %s date 2: %s, i: %i, count: %i \n", time, time1, i, count);
						//conditions for printing event based on events already created and time of new event
						if (count == 0){
							strftime(time, SIZE, "%m/%d/%Y,%H:%M", &events[count].time);
							printf("%s,%s", time, events[count].location);

							break;

						}
						else if (strcmp(time, time1) != 0 && i > 0 || count == i || strcmp(earliest_event.location , "NA\n")==0 ){
							strftime(time, SIZE, "%m/%d/%Y,%H:%M", &events[count].time);
							printf("%s,%s", time, events[count].location);

							break;


						}
						else if (strcmp(time, time1) == 0 && difftime(mktime(&events[count].time), mktime(&events[i].time)) > 0 ){
	//						strftime(time, SIZE, "%m/%d/%Y,%H:%M", &events[count].time);
	//						printf("%s,%s", time, events[count].location);


							break;


						}
					}
					//increase count of stored events
					count ++;
				}
				//change event actions
				else if (strrchr(action, 'X')){
					for (int i = 0; i <= count; i++){
					if (strcmp(events[i].title, title) == 0){
						strcpy(events[i].date, date);
						strcpy(events[i].location, location);
						events[i].time.tm_hour = hour;
						events[i].time.tm_min = min;
						strftime(time, SIZE, "%m/%d/%Y,%H:%M", &events[i].time);
						printf("%s,%s", time, events[i].location);

					}
					}
				}
				//delete event actions
				else if (strrchr(action, 'D')){
					working_tm.tm_hour = hour;
					working_tm.tm_min = min;
					working_tm.tm_year = year-1900;
					working_tm.tm_mon = month-1;
					working_tm.tm_mday = day;
					working_tm.tm_sec = 0;
					working_tm.tm_isdst = 0;
					//loop to determine what to delete from calendar
					for (int i = 0; i <= count; i++){
						//isolate dates into string variables
						strftime(time, SIZE, "%m/%d", &working_tm);
						strftime(time1, SIZE, "%m/%d", &events[i].time);
						//printf("date 1: %s date 2: %s\n", time, time1);
						//find event to be deleted
						if (difftime(mktime(&working_tm), mktime(&events[i].time)) == 0 && strcmp(events[i].title, title) == 0){
							//insert flag to show event is deleted
							strcpy(events[i].location, "NA\n");
							//make current event the earliest event
							earliest_event.time = working_tm;

						}
					}
					//loop to determine what to print
					for (int i = 0; i <= count; i++){
						//isolate dates
						//printf ("%i\n", i);
						strftime(time, SIZE, "%m/%d", &earliest_event.time);
						strftime(time1, SIZE, "%m/%d", &events[i].time);
						//printf("date 1: %s date 2: %s\n", time, time1);
						//check for date match, then check to see if the new event or the loop event is earlier
						if(strcmp(time, time1) == 0 && difftime(mktime(&earliest_event.time), mktime(&events[i].time)) == 0 && earliest_event1.time.tm_year == 0){
							earliest_event = events[i];
							//strftime(time, SIZE, "%m/%d/%Y,%H:%M", &earliest_event.time);
							//printf("Earliest Event: %s,%s", time, earliest_event.location);
							break;
						}
						else if (strcmp(time, time1) == 0 && difftime(mktime(&earliest_event.time), mktime(&events[i].time)) < 0 && strcmp(events[i].location, "NA\n") != 0){
						//store earlier event
							earliest_event = events[i];
							//strftime(time, SIZE, "%m/%d/%Y,%H:%M", &earliest_event.time);
							//printf("Earliest Event: %s,%s", time, earliest_event.location);
							break;

						}
						else if (strcmp(time, time1) == 0 && difftime(mktime(&earliest_event.time), mktime(&events[i].time)) < 0 && strcmp(events[i].location, "NA\n") == 0){
							earliest_event = events[i];
							//strftime(time, SIZE, "%m/%d/%Y,%H:%M", &earliest_event.time);
							//printf("Earliest Event: %s,%s", time, earliest_event.location);
							break;
						}
					}




						//if time of earliest event and time of deleted event match, print dashes
						if (difftime(mktime(&earliest_event1.time), mktime(&working_tm)) == 0 || strcmp(earliest_event.location, "NA\n")==0){
							earliest_event1 = earliest_event;
							strftime(time, SIZE, "%m/%d/%Y", &earliest_event1.time);
							printf("%s,--:--,NA        \n",  time);

						} //else if the
						else if(difftime(mktime(&earliest_event.time), mktime(&earliest_event1.time)) < 0 || strcmp(earliest_event1.location, earliest_event.location) != 0){
							earliest_event1 = earliest_event;
							strftime(time, SIZE, "%m/%d/%Y,%H:%M", &earliest_event.time);
							printf("%s,%s", time, earliest_event.location);

						}

							}







			//get new line
			line_in_size = getline(&buf, &bufsz, stdin);

		}

}
		return NULL;
}


int main(int argc, char *argv[]){


pthread_t t1, t2;
sem = sem_open(argv[1], 0);
char *buffer[bufsize];


pthread_create(&t1, NULL, email_filter, NULL);
pthread_create(&t2, NULL, calendar_filter, NULL);


pthread_join(t1, NULL);
pthread_join(t2, NULL);


return 0;
}
