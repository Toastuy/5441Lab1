#include "utils.h"

queue input, work;
pthread_mutex_t input_lock, work_lock, output_lock;
workItem* output[MAX_SIZE];
pthread_t first_consumer;

int input_finish = 0;
int produce_finish = 0;
int consume_finish = 0;
int total_num = 0;
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

	for (int i = 0; i < MAX_SIZE; i++)
		output[i] = NULL;

	// reader
	pthread_create(&reader_t, NULL, reader, NULL);
	
	// producer
	for (int i = 0; i < 4; i++)
		pthread_create(producer_t + i, NULL, producer, NULL);

	// consumer management

	pthread_create(&consumer_manager_t, NULL, consumer_manager, NULL);

	// // writer
	pthread_create(&writer_t, NULL, writer, NULL);

	pthread_join(reader_t, NULL);
	
	for (int i = 0;i < 4; i++)
		pthread_join(producer_t[i], NULL);

	pthread_join(consumer_manager_t, NULL);
	pthread_join(writer_t, NULL);
	time(&end);
	fprintf(stderr, "total run time of producer: %ld s\n", producer_time);
	fprintf(stderr, "total run time of consumer: %ld s\n", consumer_time);
	fprintf(stderr, "total run time of program: %ld s\n", end - start);

	return 0;
}