//Daren Liu
//2/3/21
//Lab 4 Step 3
//Calculates matrix multiplication using threads

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N 1024
#define M 1024
#define L 1024
#define MAX_RAND 10

double matrixA[N][M], matrixB[M][L], matrixC[N][L];

void *multiplyRow(void *);
pthread_t threads[N];

int main(){
	int i, j;

	// Initialize and print Matrix A
	printf("Matrix A:\n");
	srand(time(NULL));
	for (i = 0; i < N; i++){
		for (j = 0; j < M; j++){
			matrixA[i][j] = rand() % MAX_RAND; //initialize A[i][j] to a random number
			printf("%f ", matrixA[i][j]);
		}
		printf("\n");
	}
	
	// Initialize and print matrix B
	printf("\nMatrix B:\n");
	for (i = 0; i < M; i++){
		for (j = 0; j < L; j++){
			matrixB[i][j] = rand() % MAX_RAND; //initialize B[i][j] to a random number
			printf("%f ", matrixB[i][j]);
		}
		printf("\n");
	}

	// Perform matrix multiply
	for (i = 0; i < N; i++){
		pthread_create(&threads[i], NULL, multiplyRow, (void *) (size_t) i);
	}
	for (i = 0; i < N; i++){
		pthread_join(threads[i], NULL);
	}

	// Print Matrix C
	printf("\nMatrix C:\n");
	for (i = 0; i < N; i++){
		for (j = 0; j < L; j++){
			printf("%f ", matrixC[i][j]);
		}
		printf("\n");
	}
	return 0;
}

void *multiplyRow(void *arg){
	int i = (int) (size_t) arg;
	int j, k;
	// Multiply row i of A with all columns j of B to produce row i of C
	for (j = 0; j < L; j++){
		double temp = 0;
		for (k = 0; k < M; k++)
			temp += matrixA[i][k] * matrixB[k][j]; //multiply one row in A by all columns in B
		matrixC[i][j] = temp;
	}
	return NULL;
}