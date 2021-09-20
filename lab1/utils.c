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
    while (scanf("%c %hd\n", &inCmd, &inKey)){
        if (inCmd == 'X'){
        	input_finish = 1;
        	total_num = orderNum - 1;
#ifdef DEBUG
        	printf("input finished\n");
#endif
        	return NULL;
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
#ifdef DEBUG
        printf("reader: %d %c %d\n", input.tail->w->id, input.tail->w->cmd, input.tail->w->original_key);
#endif
        pthread_mutex_unlock(&input_lock);
        
        // Increment our orderNum
        orderNum = orderNum + 1;
    }
    return NULL;
}

void* consumer() {
	time_t start = time(NULL);
	while(1) {
		pthread_mutex_lock(&work_lock);
		if (work.size > 0) {
			if (work.size < LOW_THRESHOLD) {
				fprintf(stderr, "Passed low threshold\n");
				pthread_mutex_unlock(&work_lock);
				if (pthread_self() != first_consumer) {
#ifdef DEBUG
					printf("consumer-%ld deleted\n", pthread_self());
#endif
					consumer_time += time(NULL) - start;
					return NULL;
				}
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
			pthread_mutex_lock(&output_lock);
			output[w->id] = w;
			pthread_mutex_unlock(&output_lock);
#ifdef DEBUG
        	printf("consumer-%ld: %d %c %d %d %lf %d %lf\n", pthread_self(), output[w->id]->id, output[w->id]->cmd, output[w->id]->original_key,
        		output[w->id]->encode_key, output[w->id]->p_retval, output[w->id]->decoded_key, output[w->id]->c_retval);
#endif
		}
		else {
			fprintf(stderr, "Completely empty\n");
			pthread_mutex_unlock(&work_lock);
			if (pthread_self() != first_consumer || produce_finish) {
#ifdef DEBUG
				printf("consumer-%ld deleted\n", pthread_self());
#endif
				consumer_time += time(NULL) - start;
				return NULL;
			}
		}
	}
};
//Will take items from the inout queue and make adjustments on them based on the info contained in the workItwm struct.

void* producer() {
	time_t start = time(NULL);
    while(1){
        pthread_mutex_lock(&input_lock);
        if (input.size == 0) {
        	pthread_mutex_unlock(&input_lock);
        	if (input_finish) {
        		produce_finish = 1;
#ifdef DEBUG
        		printf("produce finished\n");
#endif
        		producer_time += time(NULL) - start;
        		return NULL;
        	}
        }
        else {
	        workItem* currentWork = pop(&input);
	        pthread_mutex_unlock(&input_lock);
	        switch(currentWork->cmd){
	            case 'A':
	                currentWork->encode_key = transformA1(currentWork->original_key, &currentWork->p_retval);
	            break;
	            case 'B':
	                currentWork->encode_key = transformB1(currentWork->original_key, &currentWork->p_retval);
	            break;
	            case 'C':
	                currentWork->encode_key = transformC1(currentWork->original_key, &currentWork->p_retval);
	            break;
	            case 'D':
	                currentWork->encode_key = transformD1(currentWork->original_key, &currentWork->p_retval);
	            break;
	            case 'E':
	                currentWork->encode_key = transformE1(currentWork->original_key, &currentWork->p_retval);
	        }
	        pthread_mutex_lock(&work_lock);
	        currentWork->pos = work.size;
	        push(&work, currentWork);
#ifdef DEBUG
        	printf("producer-%ld: %d %c %d %d %f\n", pthread_self(), work.tail->w->id, work.tail->w->cmd, work.tail->w->original_key,
        		work.tail->w->encode_key, work.tail->w->p_retval);
#endif
	        pthread_mutex_unlock(&work_lock);
	    }
    }
};
void* writer() {
    
    // Writing from an array that's sorted as it's filled
    // We start writing whenever the work_queue is finished /
    // everything is in the output array
    // Array filled with workitems
    
    // Declare + everything we'll be outputting to stdout
    int i = 1;
    uint16_t outId; // sequence number
	int outPos; // work queue position
	char outCmd; // cmd
	uint16_t out_encode_key; // encoded key by producer
	double out_p_retval; // retval by producer
	uint16_t out_decoded_key; //decoded key by consumer
	double out_c_retval; //retval by consumer
#ifdef DEBUG
	printf("writer started\n");
#endif
    // Check the work queue member "size"
    while(!consume_finish || i <= total_num) {
    	pthread_mutex_lock(&output_lock);
	    while (output[i] == NULL) {
#ifdef DEBUG
	    	printf("writer: waiting for %d's output, total_num: %d\n", i, total_num);
#endif
	    	pthread_mutex_unlock(&output_lock);
	    	sleep(3);
	    	pthread_mutex_lock(&output_lock);
	    }
	       
        // get all the values we'll be outputting
        outId = output[i]->id;
        outPos = output[i]->pos;
        outCmd = output[i]->cmd;
        out_encode_key = output[i]->encode_key;
        out_p_retval = output[i]->p_retval;
        out_decoded_key = output[i]->decoded_key;
        out_c_retval = output[i]->c_retval;
        pthread_mutex_unlock(&output_lock);
        i++;
        
        printf("%d %d %c %d %lf %d %lf\n", outId, outPos, outCmd, out_encode_key, out_p_retval, out_decoded_key, out_c_retval);
	}
#ifdef DEBUG
    	printf("writer existed\n");
#endif
    return NULL;
};

void* consumer_manager() {
	int flag = 1;
	while(1) {
		if (work.size > LOW_THRESHOLD && flag) {
			pthread_create(&first_consumer, NULL, consumer, NULL);
#ifdef DEBUG
			printf("consumer_manager: creating first consumer-%ld\n", first_consumer);
#endif
			flag = 0;
		}
		pthread_mutex_lock(&work_lock);
		if (work.size > HIGH_THRESHOLD) {
#ifdef DEBUG
			printf("current work size: %d\n", work.size);
#endif
			pthread_mutex_unlock(&work_lock);
			pthread_t new;
			pthread_create(&new, NULL, consumer, NULL);
#ifdef DEBUG
			printf("consumer_manager: creating a new consumer-%ld\n", new);
#endif
		}
		else if (work.size == 0) {
			pthread_mutex_unlock(&work_lock);
			if (produce_finish) {
#ifdef DEBUG
				printf("consumer manager existed\n");
#endif
				pthread_join(first_consumer, NULL);
				consume_finish = 1;
				return NULL;
			}
		}
		else
			pthread_mutex_unlock(&work_lock);
		// sleep(1);
	}
};