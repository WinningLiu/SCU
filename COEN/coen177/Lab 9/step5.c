#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static char **args; // Global variable to store command line args for thread function

void *copyFile(void *);

int main(int argc, char *argv[]){
	args = argv;
	int num_threads = atoi(argv[3]); // Accept number of threads
	pthread_t threads[num_threads];
	int i;
	// Create and execute threads
	for (i = 0; i < num_threads; i++){
		pthread_create(&threads[i], NULL, copyFile, (void *) (size_t) i);
	}
	// Join threads
	for (i = 0; i < num_threads; i++){
		pthread_join(threads[i], NULL);
	}
	return 0;
}

void *copyFile(void *arg){
	int i = (int) (size_t) arg; // Thread number
	char buffer[atoi(args[2])]; // Accept buffer size
	FILE *fp, *ofp;
	char nameBuffer[20];
	sprintf(nameBuffer, "Output/output%d.txt", i); // Generate file name based on thread number
	fp = fopen(args[1], "rb"); // Accept file input
	ofp = fopen(nameBuffer, "w");
	// Write file data to output file
	while (fread(buffer, sizeof(buffer), 1, fp)){
		fwrite(buffer, 1, sizeof(buffer), ofp);
	}
	fclose(ofp);
	fclose(fp);
	return NULL;
}
