#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/mman.h>




#define SIZE 1024





typedef struct  {

char strbuff[SIZE][SIZE];//[SIZE];
int status[SIZE];
int bufval;
int producer;
int consumer;
int bufsize;
pthread_mutex_t mutex;
pthread_cond_t cond;

}shared;


static shared *data_buf;


//struct for events, only used by calendar filter.

struct event {
	char date[SIZE];
	struct tm time;
	char location[SIZE];
	char title [SIZE];

};

static void *email_filter(){
	data_buf->producer = 1;
	int value;
	int buf_selected = 0;
	char *buf =  malloc(SIZE);
	char *token = malloc(SIZE);
	char *action = malloc(SIZE);
	char *title = malloc(SIZE);
	char *date = malloc(SIZE);
	char *time = malloc(SIZE);
	char *location = malloc(SIZE);
	static char *wsbuf = "          ";
	size_t bufsz;
	ssize_t line_in_size;
line_in_size = getline(&buf, &bufsz, stdin);

while (line_in_size >= 0){
pthread_mutex_lock(&data_buf->mutex);
while(data_buf->bufval > 0)
	pthread_cond_wait(&data_buf->cond, &data_buf->mutex);
for (int i = 0; i < data_buf->bufsize; i++){
	data_buf->bufval++;
			if (line_in_size == -1){
					data_buf->bufsize = data_buf->bufval-1;
					pthread_cond_broadcast(&data_buf->cond);
					pthread_mutex_unlock(&data_buf->mutex);
					data_buf->producer = 0;
					exit(EXIT_SUCCESS);

				}
				//printf("%s\n", buf);
				token = strchr(buf, ':');
				//check for white space between "subject:" and action flag
				if (isspace(token[1]) == 0){
					line_in_size = getline(&buf, &bufsz, stdin);

					//snprintf(strbuff[i] ,SIZE ,"");

					continue;
				}

				//printf("%s\n", token);
				token = strtok(token, ": ,");
				//printf("%s\n", token);
				//copy current token to action variable
				strcpy(action, token);

//				printf("%s\n", action);
//				printf("%lu\n",strlen(action));
				//make sure subject is a calendar action and not something arbitrary
				if(strlen(action)>1){
					line_in_size = getline(&buf, &bufsz, stdin);
					//snprintf(strbuff[i] ,SIZE ,"\r");

					continue;
				};
				//get next token
				token = strtok(NULL, ",");
				//filter bad inputs
				if(token==NULL){
					line_in_size = getline(&buf, &bufsz, stdin);
					//snprintf(strbuff[i] ,SIZE ,"\r");

					continue;
				};
				//copy current token to title variable
				strcpy(title, token);
				//get next token
				token = strtok(NULL, ",");
				//filter bad inputs
				if(token==NULL){
					line_in_size = getline(&buf, &bufsz, stdin);
					//snprintf(strbuff[i] ,SIZE ,"\r");

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
					//snprintf(strbuff[i] ,SIZE ,"\r");

					continue;
				};
				//copy current token to time variable
				strcpy(time,token);
				//get next token
				token = strtok(NULL, ",");
				if(token==NULL){
					//get next line
					line_in_size = getline(&buf, &bufsz, stdin);
					//snprintf(strbuff[i] ,SIZE ,"\r");

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






					//printf("mutex locked by email\n");




						//printf("writing to buffer position: %i\n",i);
						snprintf(data_buf->strbuff[i] ,SIZE ,"%s,%s,%s,%s,%s", action, title, date, time, location);
						data_buf->status[i] = 1;

						//printf("mutex unlocked by email\n");




					//printf("mutex unlocked by email\n");

						//get line from stdin, get length and store in buffer






				//printf("%s", data_buf->strbuff[i]);
				//get next line and size of line

			line_in_size = getline(&buf, &bufsz, stdin);
}


		pthread_cond_broadcast(&data_buf->cond);
		pthread_mutex_unlock(&data_buf->mutex);




}
data_buf->producer = 0;
//printf("%s","email ending\n");
exit(EXIT_SUCCESS);
}

static void *calendar_filter(){
	data_buf->consumer = 1;

	struct event events[365];
	struct event earliest_event;
	struct event earliest_events[31];
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
	int value;
	int buf_selected = 0;
	size_t bufsz;
	int line_in_size;
while(1){

pthread_mutex_lock(&data_buf->mutex);
value = data_buf->bufval;
//while(producer == 1){
//	printf("calendar waiting\n");
//pthread_cond_wait(&cond, &mutex);
//}


while (data_buf->bufval < data_buf->bufsize){

	if (data_buf->producer == 0 && count > 0){
			data_buf->consumer = 0;
			//printf("Calendar ending\n");
			exit(EXIT_SUCCESS);
	}
	pthread_cond_wait(&data_buf->cond, &data_buf->mutex);
}
for (int i = 0; i < value;i++){
data_buf->bufval--;
		//get line and store size


		//printf("checking buffer\n");
		//printf("%s\n", strbuff[i]);

//	while(data_buf[i]->producer == 1){
//	pthread_cond_wait(&data_buf[i]->cond, &data_buf[i]->mutex);
//	}
	//printf("mutex locked by calendar\n");

		if (data_buf->status[i] == 1){
			strcpy(buf, data_buf->strbuff[i]);
			//printf("reading from buffer position: %i\n", i);
			buf_selected = 1;
			data_buf->status[i] = 0;

		}
		else continue;

		//printf("mutex unlocked by calendar\n");

		//printf("gottem\n");

		//printf("%i\n", line_in_size);
		//check for EOF condition






		//printf("buffer contents:%s", buf);

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
							//strftime(time, SIZE, "%m/%d/%Y,%H:%M", &events[count].time);
							//printf("%s,%s", time, events[count].location);


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
							memset(events[i].location,0,SIZE);
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
						if(strcmp(time, time1) == 0 && difftime(mktime(&earliest_event.time), mktime(&events[i].time)) == 0 && earliest_events[day].time.tm_year == 0){
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


						//printf("ey yo bitch\n");

						//if time of earliest event and time of deleted event match, print dashes
						if(difftime(mktime(&earliest_events[day].time), mktime(&earliest_event.time)) > 0 || strcmp(earliest_events[day].location, earliest_event.location) == 0){
							continue;
						}
						else if (difftime(mktime(&earliest_events[day].time), mktime(&earliest_event.time)) == 0 && strcmp(earliest_events[day].location, "NA\n")!=0){
							earliest_events[day] = earliest_event;
							strftime(time, SIZE, "%m/%d/%Y", &earliest_events[day].time);
							printf("%s,--:--,NA        \n",  time);

						}
						else if(difftime(mktime(&earliest_event.time), mktime(&earliest_events[day].time)) < 0 || strcmp(earliest_events[day].location, earliest_event.location) != 0){
							earliest_events[day] = earliest_event;
							strftime(time, SIZE, "%m/%d/%Y,%H:%M", &earliest_events[day].time);
							printf("%s,%s", time, earliest_event.location);

						}

//						else{
//							strftime(time, SIZE, "%m/%d/%Y,%H:%M", &earliest_event.time);
//							if (strcmp(events[i].location, "NA\n")==0){
//							printf("%s,--:--,NA        \n",  time);
//							}else{
//
//							}
//						}


				}


				//printf("c2\n");





				}
				pthread_cond_broadcast(&data_buf->cond);
				pthread_mutex_unlock(&data_buf->mutex);

			//get new line






}
data_buf->consumer = 0;
//printf("calendar ending\n");
exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){

pthread_mutexattr_t attr;
pthread_condattr_t condattr;
//printf("initializing mutex attributes\n");
pthread_mutexattr_init(&attr);
pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
pthread_condattr_init(&condattr);
pthread_condattr_setpshared(&condattr, PTHREAD_PROCESS_SHARED);
//printf("allocating mem\n");







data_buf = mmap(0, sizeof(shared), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, 0, 0);
assert(data_buf);
data_buf->bufsize = atoi(argv[1]);
for (int i=0; i<data_buf->bufsize; i++){
data_buf->status[i] = 0;
}
pthread_mutex_init(&data_buf->mutex, &attr);
pthread_cond_init(&data_buf->cond, &condattr);


//printf("mutexes good to go\n");


//printf("forkinggggg\n");

pid_t pid = fork();

	if (pid == 0){
		//printf("in child\n");
		email_filter();


	}
	else if(pid >= 0){
		//printf("in parent\n");

		calendar_filter();


	}


exit (EXIT_SUCCESS);
}
