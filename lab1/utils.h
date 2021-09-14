#ifndef __UTILS__
#define __UTILS__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#define LOW_THRESHOLD 50
#define HIGH_THRESHOLD 150
#define FULL_SIZE 200
#define MAX_SIZE 200000

typedef struct workItem
{
	uint16_t id; // sequence number
	int pos; // work queue position
	char cmd; // cmd
	uint16_t original_key; // input key
	uint16_t encode_key; // encoded key by producer
	double p_retval; // retval by producer
	uint16_t decoded_key; //decoded key by consumer
	double c_retval //retval by consumer
} workItem;

struct node
{
	workItem* w;
	struct node* next;
};

typedef struct queue
{
	int size;
	struct node* head;
	struct node* tail;
} queue;


// queue functions
void push(queue* q, workItem* w);
workItem* pop(queue* q);
void initializeQueue(queue* q);

// global queues and mutexes defined globally in main file
extern queue input, work;
extern workItem* output[MAX_SIZE];
extern pthread_mutex_t input_lock, work_lock, output_lock;
extern int input_finish, produce_finish;

// thread functions
void* reader();
void* producer();
void* consumer();
void* writer();
void* consumer_manager();

#endif