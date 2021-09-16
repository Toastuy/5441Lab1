#include "utils.h"

queue input, work;
pthread_mutex_t input_lock, work_lock;
workItem* output[MAX_SIZE];
pthread_t first_consumer;

int input_finish = 0;
int produce_finish = 0;

int main(int argc, char const *argv[])
{
	initializeQueue(&input);
	initializeQueue(&work);
	pthread_t reader_t, writer_t, consumer_manager_t;
	pthread_t producer_t[4];

	// reader
	pthread_create(&reader_t, NULL, reader, NULL);
	
	// producer
	for (int i = 0; i < 4; i++)
		pthread_create(producer_t + i, NULL, producer, NULL);

	// consumer management

	pthread_create(&consumer_manager_t, NULL, consumer_manager, NULL);

	// // writer
	// pthread_create(&writer_t, NULL, writer, NULL);

	pthread_exit(NULL);
	return 0;
}