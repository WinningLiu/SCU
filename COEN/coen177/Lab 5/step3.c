/*
Daren Liu
2/3/21
Lab 5 Step 3
Implements producer-consumer problem using semaphores
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h> 
#include <semaphore.h>
#include <string.h>

#define NITEMS 10
#define STRINGSIZE 100

sem_t full, empty, mutex;
char buffer[NITEMS][STRINGSIZE];
int bufferStart, bufferEnd;

void* producer(void* arg) {
	int i;
	char currItem[STRINGSIZE];
	for (i = 0; i < NITEMS + 1; i++){
		// Send terminate message
		if (i == NITEMS)
			strcpy(currItem, "END");
		// Print message
		else {
			sprintf(currItem, "Item %d", i);
			printf("Producer added %s to buffer.\n", currItem);
		}
		sem_wait(&empty); //wait for consumer before running code
		sem_wait(&mutex);
		// Add item to buffer
		strcpy(buffer[bufferEnd], currItem);
		bufferEnd = (bufferEnd + 1) % NITEMS;
		//allow consumer to run
		sem_post(&mutex);
		sem_post(&full);
	}
  	return (NULL);
} 

void* consumer(void* arg){
	char currItem[STRINGSIZE];
	do {
		//wait for producer before running
		sem_wait(&full);
		sem_wait(&mutex);
		// Remove item from buffer
		strcpy(currItem, buffer[bufferStart]);
		bufferStart = (bufferStart + 1) % NITEMS;
		//allow producer to run
		sem_post(&mutex);
		sem_post(&empty);
		// Stop listening for messages
		if (strcmp(currItem, "END") == 0)
			break;
		// Print message
		else
			printf("Consumer removed %s from buffer.\n", currItem);
	} while (1);
  	return (NULL);
}

int main() { 
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, NITEMS);
	sem_init(&mutex, 0, 1);
	pthread_t p, c;
	bufferStart = bufferEnd = 0;
   	
	pthread_create(&p, NULL, producer, NULL);
	pthread_create(&c, NULL, consumer, NULL);
	
   	pthread_join(p, NULL);
   	pthread_join(c, NULL);

	printf("Main thread done.\n");
	sem_destroy(&full);
	sem_destroy(&empty);
	sem_destroy(&mutex);
	return 0; 
} 