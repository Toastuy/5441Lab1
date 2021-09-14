#include "utils.h"

// mutex and threshold management should be implemented by caller
void push(queue* q, workItem* w) {
	struct node* new_node = (struct node*)malloc(sizeof(struct node));
	new_node->w = w;
	new_node->next = NULL;
	if (q->head == NULL) {
		q->head = new_node;
		q->tail = new_node;
	}
	else {
		q->tail->next = new_node;
		q->tail = new_node;
	}
	q->size++;
};

workItem* pop(queue* q) {
	if (q->head == NULL)
		return NULL;
	struct node* tmp = q->head;
	q->head = q->head->next;
	workItem* re = tmp->w;
	free(tmp);
	q->size--;
	return re;
};

void initializeQueue(queue* q) {
	q->size = 0;
	q->head = q->tail = NULL;
};

// Called by a thread
// Function will read all info from stdin, assign it to a corresponding
// work item, and then push that workitem onto the input queue
// Note: Mutex handled in Main.c
void* reader() {
    
    // We need a counter to keep track of the order we read items
    uint16_t orderNum;
    
    // The two pieces of info we'll need to get
    char inCmd;
    uint16_t inKey;
    
    // Definitions
    // CHECK: Definition above a declaration (newWork)
    orderNum = 1;
    
    // For every set of cmd/key in stdin
    // 1. Create a workItem for them
    // 2. Put the inCmd and inKey into the workitem
    // 3. Push workItem onto input queue
    while (scanf("%c %d\n", &inCmd, &inKey)){
        if (inCmd == 'X')
        	return;
        // 1. Create a new workItem
        struct workItem* newWork = (struct workItem*)malloc(sizeof(struct workItem));
        
        // 2. Give newWork the info we currently know
        // CHECK: Do we need to init rest of struct?
        newWork->id = orderNum;
        newWork->cmd = inCmd;
        newWork->original_key = inKey;
        
        // 3. Now push our newWork onto the input queue
        // pthread_mutex_lock(&input_lock);
        push(&input, newWork);
        // pthread_mutex_unlock(&input_lock);
        
        // Increment our orderNum
        orderNum = orderNum + 1;
        printf("%d %c %d\n", input.tail->w->id, input.tail->w->cmd, input.tail->w->original_key);
        
    }
    
}

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