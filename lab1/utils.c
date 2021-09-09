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


void reader() {};
void producer() {};
void consumer() {};
void writer() {};