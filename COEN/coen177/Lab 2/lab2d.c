//Daren LIu
//1/13/21
//Lab 2 steps 7
//The child process runs the 'ls' command while the parent process waits for the child process to complete.

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int i, n = atoi(argv[1]);
    printf("\n Before forking. \n");
    pid = fork();
    if (pid == -1)
        fprintf(stderr, "can't fork, error %d\n");

    else if (pid == 0)
    {
        execlp("/bin/ls", "ls", NULL);
    }
    else
    {
        wait(NULL);
        printf("Child Complete");
        exit(0);
    }
    
    return 0;
}