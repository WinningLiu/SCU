//Daren Liu
//1/5/2021
//Lab 1 part 1-5
//Displays differences in time delays and for loops and pthreads

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

//Part 1-4
/*int main(int argc, char *argv[])
{
    pid_t pid;
    int i, n = atoi(argv[1]);
    printf("\n Before forking. \n");
    pid = fork();
    if (pid == -1)
        fprintf(stderr, "can't fork, error %d\n");

    if (pid)
    {
        for (i = 0; i < 100; i++)
        {
            printf("\t \t \t Parent Process %d \n", i);
            usleep(n);
        }
    }
    else
    {
        for (i = 0; i < 100; i++)
        {
            printf("Child process %d\n", i);
            usleep(n);
        }
    }
    return 0;
}

*/
//Step 3: Segmentation fault

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
