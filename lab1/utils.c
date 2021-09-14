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
        if (inCmd == 'X'){
        	input_finish = 1;
        	return;
        }
        // 1. Create a new workItem
        struct workItem* newWork = (struct workItem*)malloc(sizeof(struct workItem));
        
        // 2. Give newWork the info we currently know
        // CHECK: Do we need to init rest of struct?
        newWork->id = orderNum;
        newWork->cmd = inCmd;
        newWork->original_key = inKey;
        
        // 3. Now push our newWork onto the input queue
        pthread_mutex_lock(&input_lock);
        push(&input, newWork);
        pthread_mutex_unlock(&input_lock);
        
        // Increment our orderNum
        orderNum = orderNum + 1;
        printf("%d %c %d\n", input.tail->w->id, input.tail->w->cmd, input.tail->w->original_key);
        
    }
    
}

void* producer() {};
void* consumer() {
	while(1) {
		pthread_mutex_lock(&work_lock);
		if (work.size > 0) {
			if (work.size < LOW_THRESHOLD) {
				fprintf(stderr, "Passed low threshold\n");
				pthread_mutex_unlock(&work_lock);
				return;
			}
			workItem* w = pop(&work);
			if (work.size > HIGH_THRESHOLD) {
				fprintf(stderr, "Passed high threshold, current workItem: %d %c %d\n", 
					w->id, w->cmd, w->original_key);
			}
			if (work.size == FULL_SIZE)
				fprintf(stderr, "Completely full, current workItem: %d %c %d\n", 
					w->id, w->cmd, w->original_key);
			pthread_mutex_unlock(&work_lock);
			switch (w->cmd) {
				case 'A':
					w->decoded_key = transformA2(w->encode_key, &w->c_retval);
					break;
				case 'B':
					w->decoded_key = transformB2(w->encode_key, &w->c_retval);
					break;
				case 'C':
					w->decoded_key = transformC2(w->encode_key, &w->c_retval);
					break;
				case 'D':
					w->decoded_key = transformD2(w->encode_key, &w->c_retval);
					break;
				case 'E':
					w->decoded_key = transformE2(w->encode_key, &w->c_retval);
					break;
			}
			output[w->id] = w;
		}
		else {
			fprintf(stderr, "Completely empty\n");
			pthread_mutex_unlock(&work_lock);
			return;
		}
	}
};
void* writer() {};

void* consumer_manager() {
	int flag = 1;
	while(1) {
		if (work.size > LOW_THRESHOLD && flag) {
			pthread_t first;
			pthread_create(&first, NULL, consumer, NULL);
			flag = 0;
		}
		pthread_mutex_lock(&work_lock);
		if (work.size > HIGH_THRESHOLD) {
			pthread_mutex_unlock(&work_lock);
			pthread_t new;
			pthread_create(&new, NULL, consumer, NULL);
		}
		else if (work.size == 0) {
			pthread_mutex_unlock(&work_lock);
			if (produce_finish)
				pthread_exit(NULL);
		}
		else
			pthread_mutex_unlock(&work_lock);
		sleep(5);
	}
}