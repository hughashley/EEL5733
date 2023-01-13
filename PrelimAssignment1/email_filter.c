#import "email_filter.h"


int main(int argc, char *argv[]){
	//file pointer
	FILE *file;

	//input buffer
	char *buf = NULL;
	size_t bufsz;
	ssize_t line_in;
	//regular expression
	char appt[6][1024];
	char comp[strlen("Subject:")+1];
	strcpy(comp,"Subject:");
	//working variable

	file = fopen("test.txt", "r");
	line_in = getline(&buf, &bufsz, file);


	while (line_in >= 0){

		printf("%s", buf);
		line_in = getline(&buf, &bufsz, file);


	};


	fclose(file);
	return 0;
};
