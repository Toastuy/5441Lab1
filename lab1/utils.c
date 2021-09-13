#include "utils.h"

// mutex and threshold management should be implemented by caller
void push(queue q, workItem* w) {
	struct node* new_node = (struct node*)malloc(sizeof(struct node));
	new_node->w = w;
	new_node->next = NULL;
	if (q.head == NULL) {
		q.head = new_node;
		q.tail = new_node;
	}
	else {
		q.tail->next = new_node;
		q.tail = new_node;
	}
	q.size++;
};

workItem* pop(queue q) {
	if (q.head == NULL)
		return NULL;
	struct node* tmp = q.head;
	q.head = q.head->next;
	workItem* re = tmp->w;
	free(tmp);
	q.size--;
	return re;
};


void* reader() {};
void* producer() {};
void* consumer() {};
void* writer() {};

void* consermer_manager() {
	int flag = 1;
	while(1) {
		if (work.size > LOW_THRESHOLD && flag) {
			consumer_info.size = 1;
			consumer_info.consumer = (pthread_t *)malloc(sizeof(pthread_t) * 1);
			pthread_create(consumer_info.consumer, NULL, consumer, NULL);
			flag = 0;
		}
		if (work.size > HIGH_THRESHOLD) {
			consumer_info.size++;
			consumer_info.consumer = (pthread_t*)realloc(consumer_info.consumer, sizeof(pthread_t) * consumer_info.size);
			pthread_create(consumer_info.consumer + consumer_info.size - 1, NULL, consumer, NULL);
		}
		if (work.size < LOW_THRESHOLD) {
			// handle by consumer threads
		}
		
		// make sure there is no consumer only when work is done
		if (consumer_info.size == 0)
			return;

		sleep(10);
	}
}