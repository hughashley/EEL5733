#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


#define SIZE 1024

struct event {
	char date[SIZE];
	struct tm time;
	char location[SIZE];
	char title [SIZE];

};

int main(int argc, char *argv[]){

	struct event events[30];

	char *buf = malloc(SIZE);
	char *token = malloc(SIZE);
	char *action = malloc(SIZE);
	char *title = malloc(SIZE);
	char *date = malloc(SIZE);
	char *time = malloc(SIZE);
	char *location = malloc(SIZE);
	int year;
	int month;
	int day;
	int hour;
	int min;
	int count = 0;
	size_t bufsz;
	ssize_t line_in_size;

	line_in_size = getline(&buf, &bufsz, stdin);


	while (line_in_size >= 0){




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
			//printf("%d,%d\n", hour, min);
			token = strtok(NULL, ",");
			strcpy(location,token);
			if (strrchr(action, 'C')){
				strcpy(events[count].title, title);
				events[count].time.tm_hour = hour;
				events[count].time.tm_min = min;
				events[count].time.tm_year = year;
				events[count].time.tm_mon = month-1;
				events[count].time.tm_mday = day;
				events[count].time.tm_isdst = 0;
				strcpy(events[count].location, location);
				for (int i = 0; i <= count; i++){
					if (difftime(mktime(&events[count].time), mktime(&events[i].time)) == 0 && i != count){
						strftime(time, SIZE, "%m/%d/%g,%H:%M", &events[count].time);
						printf("%s,%s\n", time, events[i].location);
						//printf("equal %f", difftime(mktime(&events[count].time), mktime(&events[i].time)));
						count ++;
						break;
					}
					else if (difftime(mktime(&events[count].time), mktime(&events[i].time)) < 0|count == 0){
						strftime(time, SIZE, "%m/%d/%g,%H:%M", &events[count].time);
						//printf("less than %f", difftime(mktime(&events[count].time), mktime(&events[i].time)));
						printf("%s,%s\n", time, events[count].location);

						break;
					}
					else if (difftime(mktime(&events[count].time), mktime(&events[i].time)) > 0){

						break;
						//printf("greater than %f", difftime(mktime(&events[count].time), mktime(&events[i].time)));
						//strftime(time, SIZE, "%m/%d/%g,%H:%M", &events[i].time);
						//printf("%s,%s\n", time, events[i].location);
					}

				}
				//printf("%s,%i,%s", events[count].date, events[count].time, events[count].location);
				count ++;
			}
			else if (strrchr(action, 'X')){
				for (int i = 0; i <= count; i++){
				if (strcmp(events[i].title, title) == 0){
					strcpy(events[i].date, date);
					strcpy(events[i].location, location);
					events[i].time.tm_hour = hour;
					events[count].time.tm_min = min;
					strftime(time, SIZE, "%m/%d/%g,%H:%M", &events[i].time);
					printf("%s,%s\n", time, events[i].location);

				}
				}
				count ++;
			}
			else if (strrchr(action, 'D')){
				strcpy(events[count].title, title);
				events[count].time.tm_hour = hour;
				events[count].time.tm_min = min;
				events[count].time.tm_year = year;
				events[count].time.tm_mon = month-1;
				events[count].time.tm_mday = day;
				events[count].time.tm_isdst = 0;
				for (int i = 0; i <= count; i++){
					if (difftime(mktime(&events[count].time), mktime(&events[i].time)) == 0 && i != count){
						strftime(time, SIZE, "%m/%d/%g", &events[count].time);
						printf("%s,--:--,NA\n",  time);
						//printf("equal %f", difftime(mktime(&events[count].time), mktime(&events[i].time)));
						break;
					}
					else if (difftime(mktime(&events[count].time), mktime(&events[i].time)) < 0|count == 0){
						strftime(time, SIZE, "%m/%d/%g", &events[count].time);
						//printf("less than %f", difftime(mktime(&events[count].time), mktime(&events[i].time)));
						printf("%s,--:--,NA%i\n",  time,count);
						break;
					}
					else if (difftime(mktime(&events[count].time), mktime(&events[i].time)) > 0){
						break;
						//printf("greater than %f", difftime(mktime(&events[count].time), mktime(&events[i].time)));
						//strftime(time, SIZE, "%m/%d/%g,%H:%M", &events[i].time);
						//printf("%s,%s\n", time, events[i].location);
					}

				}
				count ++;
			}




		line_in_size = getline(&buf, &bufsz, stdin);

	}

	free(buf);
	free(action);
	free(title);
	free(date);
	free(time);
	free(location);
	return 0;
};
