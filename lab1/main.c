#include "utils.h"

queue input, work;
pthread_mutex_t input_lock, work_lock;
workItem* output[MAX_SIZE];
pthread_t first_consumer;

int input_finish = 0;
int produce_finish = 0;
time_t producer_time, consumer_time;

int main(int argc, char const *argv[])
{
	time_t start, end;
	producer_time = consumer_time = 0;
	time(&start);
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

	pthread_join(reader_t, NULL);
	
	for (int i = 0;i < 4; i++)
		pthread_join(producer_t[i], NULL);

	pthread_join(consumer_manager_t, NULL);
	// pthread_join(writer_t, NULL);
	time(&end);
	printf("total run time of producer: %d s\n", producer_time);
	printf("total run time of consumer: %d s\n", consumer_time);
	printf("total run time of program: %d s\n", end - start);

	return 0;
}