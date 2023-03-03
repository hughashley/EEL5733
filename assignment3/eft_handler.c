#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#define SIZE 1024
struct transfer_data {
	pthread_mutex_t ready_mutex;
	pthread_cond_t ready_cond;
	pthread_cond_t post_cond;
	int posted;
	int ready;
	int accountFrom;
	int accountTo;
	int balance;
	int count;
};
struct account {
	int account_number;
	int balance;
};
int active_threads = 0;
int active_thread_from = 0;
int active_thread_to = 0;
int max_threads;
int active_thread = 0;


static pthread_mutex_t account1_mutex;
static pthread_cond_t account1_cond;

static pthread_mutex_t account2_mutex;
static pthread_cond_t account2_cond;

static struct account accounts[SIZE];
static struct transfer_data thread_manager[SIZE];


void *eft(void *thread_num){

	int thread_no = *((int *)thread_num);


	thread_manager[thread_no].ready = 1;

	//printf("%i\n", thread_no);

	while (1){

	//pthread_mutex_lock(&thread_manager[thread_no].ready_mutex);

	while(thread_manager[thread_no].posted == 0 || thread_manager[thread_no-1].posted == 1 ){
		//printf("thread %i waiting", thread_no);
		if (thread_manager[thread_no].ready == -1)
			return NULL;
		//pthread_cond_wait(&thread_manager[thread_no].post_cond, &thread_manager[thread_no].ready_mutex);

		}
	//pthread_mutex_unlock(&thread_manager[thread_no].ready_mutex);
	thread_manager[thread_no].count = 0;
	thread_manager[thread_no].ready = 0;


	pthread_mutex_lock(&account1_mutex);
	while(active_thread_from != 0 )
		pthread_cond_wait(&account1_cond, &account1_mutex);
	//printf("from locked\n");
	active_thread_from++;
	accounts[thread_manager[thread_no].accountFrom].balance = accounts[thread_manager[thread_no].accountFrom].balance - thread_manager[thread_no].balance;
	//printf("%d withdrawn from account %d by thread %d\n", thread_manager[thread_no].balance, thread_manager[thread_no].accountFrom, thread_no);

	active_thread_from--;
	//printf("from unlocked\n");



	pthread_cond_broadcast(&account1_cond);
	pthread_mutex_unlock(&account1_mutex);


	pthread_mutex_lock(&account2_mutex);
	while(active_thread_to != 0)
		pthread_cond_wait(&account2_cond, &account2_mutex);

	//printf("to locked\n");
	active_thread_to++;
	accounts[thread_manager[thread_no].accountTo].balance = accounts[thread_manager[thread_no].accountTo].balance + thread_manager[thread_no].balance;
	//printf("%d deposited in account %d by thread %d\n", thread_manager[thread_no].balance, thread_manager[thread_no].accountTo, thread_no);
	//printf("to unlocked\n");

	active_thread_to--;
	pthread_cond_signal(&account2_cond);

	pthread_mutex_unlock(&account2_mutex);


	pthread_mutex_lock(&thread_manager[thread_no].ready_mutex);
	thread_manager[thread_no].accountFrom = 0;
	thread_manager[thread_no].accountTo = 0;
	thread_manager[thread_no].balance = 0;
	thread_manager[thread_no].posted = 0;
	thread_manager[thread_no].ready = 1;
	thread_manager[thread_no].count++;
	pthread_cond_broadcast(&thread_manager[thread_no].ready_cond);
	pthread_mutex_unlock(&thread_manager[thread_no].ready_mutex);


	}
	return NULL;
}



int main(int argc, char *argv[]){
	max_threads = atoi(argv[1]);

	char *buf =  malloc(SIZE);
	char *token = malloc(SIZE);
	int num_accounts = 0;
	int account;
	int to;
	int from;
	int bal;
	int run = 1;
	int account_number;
	int threads_created = 0;
	int num_threads = 0;
	int thread_number[max_threads];
	int thread_assigned = 0;
	int transfers;


	pthread_t thread[max_threads];

	size_t bufsz;
	ssize_t in_length;
	pthread_mutex_init(&account1_mutex, NULL);
	pthread_mutex_init(&account2_mutex, NULL);


	//printf("creating threads\n");
	for (int i = 0;i< max_threads; i++){
		thread_number[i]=i;
		pthread_mutex_init(&thread_manager[i].ready_mutex, NULL);
		pthread_create(&thread[i], NULL, eft, (void*)&thread_number[i]);

	}
	sleep(1);
	while (run == 1){


		//get line from stdin
		in_length = getline(&buf, &bufsz, stdin);
		//check for eof
		if (in_length == -1){
				run = 0;
		}
		//tokenize input
		token = strtok(buf, " ");
		//move to next line in stdin if nothing to tokenize
		if(token==NULL)
			continue;
		//check for transfer, else create account
		if (strcmp(token, "Transfer") == 0){
			//printf("starting transfer\n");


			thread_assigned = 0;
			while(thread_assigned == 0){
			printf("%i", transfers);
			for (int i=0; i < max_threads; i++){

				if (thread_manager[i].ready != 1 || thread_manager[i].count > thread_manager[i + 1].count){

					continue;
				}
				else{

					from = atoi(strtok(NULL, " "));
					to = atoi(strtok(NULL, " "));
					bal = atoi(strtok(NULL, ""));
					//printf("%i", i);
				pthread_mutex_lock(&thread_manager[i].ready_mutex);
				printf("amount: %d to: %d from: %d thread: %d\n", bal, to, from, i);
				//while (thread_manager[i].ready != 1)
					//pthread_cond_wait(&thread_manager[i].ready_cond, &thread_manager[i].ready_mutex);

				thread_manager[i].accountFrom = from;
				thread_manager[i].accountTo = to;
				thread_manager[i].balance = bal;
				thread_manager[i].posted = 1;

				while(thread_manager[i].ready != 0){
					pthread_cond_signal(&thread_manager[i].post_cond);
				}

				pthread_mutex_unlock(&thread_manager[i].ready_mutex);
				thread_assigned = 1;
				transfers ++;

				break;
				}

			}
			}




		}
		else{
			//create account
			account = atoi(token);
			bal = atoi(strtok(NULL, " "));
			accounts[account].account_number = account;
			accounts[account].balance = bal;
			//printf("%d %d\n", account, bal);
			num_accounts ++;
		}



	}

	for (int i = 0; i < max_threads; i++){
		while (thread_manager[i].ready != 1){

		}
			thread_manager[i].ready = -1;

	}
	for (int i = 0; i < max_threads; i++){

			pthread_join(thread[i], NULL);

	}

	for (int i = 1; i <= num_accounts ; i++){
			printf("%d %d\n", accounts[i].account_number, accounts[i].balance);
	}
	return 0;


}

