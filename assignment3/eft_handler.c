#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define SIZE 1024
struct transfer_data {
	int accountFrom;
	int accountTo;
	int balance;
};
struct account {
	int account_number;
	int balance;
};
int active_threads = 0;
int active_thread_from = 0;
int active_thread_to = 0;
int max_threads;
pthread_mutex_t account1_mutex;
pthread_cond_t account1_cond;
pthread_barrier_t account1_barrier;
pthread_mutex_t account2_mutex;
pthread_cond_t account2_cond;
pthread_barrier_t account2_barrier;
static struct account accounts[SIZE];
static struct transfer_data thread_manager;


void *eft(void *transfer_args){

	struct transfer_data *transfer;
	transfer = (struct transfer_data *) transfer_args;

	pthread_mutex_lock(&account1_mutex);
	while(active_thread_from != 0)
		pthread_cond_wait(&account1_cond, &account1_mutex);

	active_thread_from++;
	accounts[transfer->accountFrom-1].balance = accounts[transfer->accountFrom-1].balance - transfer->balance;
	active_thread_from--;

	pthread_cond_signal(&account1_cond);
	pthread_mutex_unlock(&account1_mutex);
	//printf("%d %d %d\n", transfer->accountFrom, transfer->accountTo, transfer->balance);


	pthread_mutex_lock(&account2_mutex);
	while(active_thread_to != 0)
		pthread_cond_wait(&account2_cond, &account2_mutex);

	active_thread_to++;
	accounts[transfer->accountTo-1].balance = accounts[transfer->accountTo-1].balance + transfer->balance;
	active_thread_to--;

	pthread_cond_signal(&account2_cond);
	pthread_mutex_unlock(&account2_mutex);

	active_threads --;
	return NULL;
}



int main(int argc, char *argv[]){
	char *buf =  malloc(SIZE);

	char *token = malloc(SIZE);
	void *argpointer = NULL;
	int num_accounts = 0;
	int run = 1;
	int account_number;


	max_threads = atoi(argv[1]);

	pthread_t thread;
	pthread_barrier_init(&account1_barrier, NULL, max_threads);
	pthread_barrier_init(&account2_barrier, NULL, max_threads);
	size_t bufsz;
	ssize_t in_length;


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
			//pause if at active thread limit
			while (active_threads == max_threads){

			}

			thread_manager.accountFrom = atoi(strtok(NULL, " "));
			thread_manager.accountTo = atoi(strtok(NULL, " "));
			thread_manager.balance = atoi(strtok(NULL, ""));




			pthread_create(&thread, NULL, eft, (void*) &thread_manager);
			pthread_detach(thread);


			active_threads ++;







		}
		else{
			//create account
			accounts[num_accounts].account_number = atoi(token);
			accounts[num_accounts].balance = atoi(strtok(NULL, " "));
			//printf("%d %d\n", accounts[num_accounts].account_number, accounts[num_accounts].balance);
			num_accounts ++;
		}



	}



	for (int i = 0; i < num_accounts ; i++){
			printf("%d %d\n", accounts[i].account_number, accounts[i].balance);
	}
	return 0;


}

