/*
Daren Liu
2/3/21
Lab 5 Step 4
Implements producer-consumer problem using condition variables and locks
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h> 
#include <string.h>

#define NITEMS 10
#define STRINGSIZE 100

pthread_mutex_t mutex;
pthread_cond_t full, empty;
char buffer[NITEMS][STRINGSIZE];
int bufferStart, bufferEnd, bufferSize;

void* producer(void* arg) {
	int i;
	char currItem[STRINGSIZE];
	for (i = 0; i < NITEMS + 1; i++){
		// Send terminate message at the end of the iteration
		if (i == NITEMS)
			strcpy(currItem, "END");
		// Print message
		else
			sprintf(currItem, "Item %d", i);
		pthread_mutex_lock(&mutex); //locks mutex
		// Make sure buffer is not full
		while (bufferSize == NITEMS)
			pthread_cond_wait(&empty, &mutex); //waits for consumer
		// Add item to buffer
		strcpy(buffer[bufferEnd], currItem);
		bufferEnd = (bufferEnd + 1) % NITEMS;
		bufferSize++;
		pthread_cond_signal(&full); //allows consumer to run
		pthread_mutex_unlock(&mutex); //unlocks mutex
		if (strcmp(currItem, "END") != 0)
			printf("Producer added %s to buffer.\n", currItem);
	}
  	return (NULL);
} 

void* consumer(void* arg){
	char currItem[STRINGSIZE];
	do {
		pthread_mutex_lock(&mutex); //locks mutex
		// Make sure buffer is not empty before consuming
		while (bufferSize == 0)
			pthread_cond_wait(&full, &mutex); //waits for producer
		// Remove item from buffer
		strcpy(currItem, buffer[bufferStart]);
		bufferStart = (bufferStart + 1) % NITEMS;
		bufferSize--; 
		pthread_cond_signal(&empty); //allows producer to run
		pthread_mutex_unlock(&mutex); //unlocks mutex
		// Stop listening to messages
		if (strcmp(currItem, "END") == 0)
			break;
		// Print message
		else
			printf("Consumer removed %s from buffer.\n", currItem);
	} while (1);
  	return (NULL);
}

int main() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&full, NULL);
	pthread_cond_init(&empty, NULL);
	pthread_t p, c;
	bufferStart = bufferEnd = bufferSize = 0;
   	
	pthread_create(&p, NULL, producer, NULL);
	pthread_create(&c, NULL, consumer, NULL);
	
   	pthread_join(p, NULL);
   	pthread_join(c, NULL);

	printf("Main thread done.\n");
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&full);
	pthread_cond_destroy(&empty);
	return 0; 
} 