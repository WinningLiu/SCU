//Daren LIu
//1/13/21
//Lab 2 steps 6
//This code is step 1-4 but with threads instead of processes. 

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

void *start_routine (void* arg)
{
    int i;
    for (i = 0; i < 100; i++)
    {
        printf("\t \t \t %d %d \n", pthread_self(), i);
        usleep(*((int*)(arg)));
    }
}

int main(int argc, char *argv[])
{
    int pthread1, pthread2;
    pthread_t pid1, pid2;
    int i, n = atoi(argv[1]);
    printf("\n Before forking. \n");
    pthread1 = pthread_create(&pid1, NULL, &start_routine, &n);
    pthread2 = pthread_create(&pid2, NULL, &start_routine, &n);
    if (pid1 == -1 || pid2 == -1)
        fprintf(stderr, "can't fork, error %d\n");

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    return 0;
}