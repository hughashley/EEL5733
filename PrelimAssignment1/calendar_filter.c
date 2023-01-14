#include "filter.h"


int main(int argc, char *argv[]){


	FILE *file;
	char *buf = NULL;
	size_t bufsz;
	ssize_t line_in_size;




	char comp[strlen("Subject:")+1];
	strcpy(comp,"Subject:");

	file = fopen("test.txt", "r");
	line_in_size = getline(&buf, &bufsz, file);


	while (line_in_size >= 0){

		printf("%s", buf);











		line_in_size = getline(&buf, &bufsz, file);
	};


	fclose(file);
	return 0;
};
