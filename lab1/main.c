#include "utils.h"

queue input, work, output;
pthread_mutex_t input_lock, work_lock, output_lock, consumer_info_lock;
consumerInfo consumer_info;

int main(int argc, char const *argv[])
{
	pthread_t reader, writer, consumer_manager;
	pthread_t producer[4];

	// reader
	pthread_create(&reader, NULL, reader, NULL);
	
	// producer
	for (int i = 0; i < 4; i++)
		pthread_create(producer + i, NULL, producer, NULL);

	// consumer management

	pthread_create(&consumer_manager, NULL, consumer_manager, NULL);

	// writer
	pthread_create(&writer, NULL, writer, NULL);

	pthread_exit(NULL);
	return 0;
}