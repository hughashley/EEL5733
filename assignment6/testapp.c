#include <linux/ioctl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>

#define DEVICE "/dev/a5"

#define CDRV_IOC_MAGIC 'Z'
#define E2_IOCMODE1 _IO(CDRV_IOC_MAGIC, 1)
#define E2_IOCMODE2 _IO(CDRV_IOC_MAGIC, 2)


void *test1(){
	char read_buf[10];
	int fd = open(DEVICE, O_RDWR);

	switch (fd){

	case -1:
		printf("error opening file in mode 1");
	default:
		read(fd, read_buf, sizeof(read_buf));
		printf("%s\n", read_buf);

	}


}

void *test2(){
	char read_buf[10];
	int fd = open(DEVICE, O_RDWR);
	int rc;

	rc = ioctl(fd, E2_IOCMODE1);
	if (rc == -1) {
		perror("\n***error in ioctl***\n");
	}
	rc = ioctl(fd, E2_IOCMODE2);
	if (rc == -1) {
		perror("\n***error in ioctl***\n");
	}
	rc = ioctl(fd, E2_IOCMODE1);
	if (rc == -1) {
		perror("\n***error in ioctl***\n");
	}
	rc = ioctl(fd, E2_IOCMODE2);
	if (rc == -1) {
		perror("\n***error in ioctl***\n");
	}
	rc = ioctl(fd, E2_IOCMODE1);
	if (rc == -1) {
		perror("\n***error in ioctl***\n");
	}
	rc = ioctl(fd, E2_IOCMODE2);
	if (rc == -1) {
		perror("\n***error in ioctl***\n");
	}
	rc = ioctl(fd, E2_IOCMODE1);
	if (rc == -1) {
		perror("\n***error in ioctl***\n");
	}
	rc = ioctl(fd, E2_IOCMODE2);
	if (rc == -1) {
		perror("\n***error in ioctl***\n");
	}
	rc = ioctl(fd, E2_IOCMODE1);
	if (rc == -1) {
		perror("\n***error in ioctl***\n");
	}
	rc = ioctl(fd, E2_IOCMODE2);
	if (rc == -1) {
		perror("\n***error in ioctl***\n");
	}
	rc = ioctl(fd, E2_IOCMODE1);
	if (rc == -1) {
		perror("\n***error in ioctl***\n");
	}


}
void *test3(){
	pid_t pid;
	int rc;
	int fd;
	char read_buf[10];
	pid = fork();
	if(pid==0){

		fd = open(DEVICE, O_RDWR);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("child: %s", read_buf);

	}

		fd = open(DEVICE, O_RDWR);

		read(fd, read_buf, sizeof(read_buf));
		printf("parent: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("parent: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("parent: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("parent: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("parent: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("parent: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("parent: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("parent: %s", read_buf);
		read(fd, read_buf, sizeof(read_buf));
		printf("parent: %s", read_buf);
}

void *test4(){
	pid_t pid;
	int fd;
	char write_buf[100];
	strcpy(write_buf, "test4");
	pid = fork();
	if(pid==0){

		fd = open(DEVICE, O_RDWR);
		strcpy(write_buf, "test4");
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));

	}

		fd = open(DEVICE, O_RDWR);

		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));

}




int main(int argc, char *argv[]) {
	pthread_t t1, t2;
	int rc;
	char dev_path[20];
	int i,fd;
	char ch, write_buf[100], read_buf[10];
	int offset, origin;

	fd = open(DEVICE, O_RDWR);
	if(fd == -1) {
		printf("File %s either does not exist or has been locked by another process\n", DEVICE);
		exit(-1);
	}

	printf(" 1 = deadlock test a\n");
	printf(" 2 = deadlock test b\n");
	printf(" 3 = deadlock test c\n");
	printf(" 4 = deadlock test d\n");
	printf("\n\n enter command:");

	scanf("%c", &ch);
	switch(ch) {
	case '1':

		rc = ioctl(fd, E2_IOCMODE1);
		if (rc == -1) {
			perror("\n***error in ioctl***\n");
			return -1;
		}
		printf("attempting to open another instance of %s in mode 1\n", DEVICE);
		pthread_create(&t1, NULL, test1, NULL);
		strcpy(write_buf, "test1");
		write(fd, write_buf, sizeof(write_buf));

		break;

	case '2':
		printf("\n enter mode 2 \n");
		strcpy(write_buf, "test2");
		write(fd, write_buf, sizeof(write_buf));
		rc = ioctl(fd, E2_IOCMODE2);
		if (rc == -1) {
			perror("\n***error in ioctl***\n");
			return -1;
		}
		pthread_create(&t1, NULL, test2, NULL);
		break;
	case '3':
		printf("\n enter mode 1 \n");
		rc = ioctl(fd, E2_IOCMODE1);
		if (rc == -1) {
			perror("\n***error in ioctl***\n");
			return -1;
		}
		strcpy(write_buf, "test3");
		write(fd, write_buf, sizeof(write_buf));
		pthread_create(&t1, NULL, test3, NULL);
		break;
	case '4':
		printf("\n enter mode 1 \n");
		rc = ioctl(fd, E2_IOCMODE1);
		if (rc == -1) {
			perror("\n***error in ioctl***\n");
			return -1;
		}
		pthread_create(&t1, NULL, test4, NULL);
		break;
	case 'r':
		printf("Origin \n 0 = beginning \n 1 = current \n 2 = end \n\n");
		printf(" enter origin :");
		scanf("%d", &origin);
		printf(" \n enter offset :");
		scanf("%d", &offset);
		lseek(fd, offset, origin);
		if (read(fd, read_buf, sizeof(read_buf)) > 0) {
			printf("\ndevice: %s\n", read_buf);
		} else {
			fprintf(stderr, "Reading failed\n");
		}
		break;

	default:
		printf("Command not recognized\n");
		break;

	}
	pthread_join(t1, NULL);
	sleep(1);
	close(fd);
	return 0;
}
