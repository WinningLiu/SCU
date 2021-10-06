//Daren LIu
//1/13/21
//Lab 2 steps 5
//This code creates 7 processes using fork()

#include <stdio.h>                              // printf, stderr
#include <stdlib.h>             // exit()
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    
    // Clear the home/console before running
    //system("clear");    

    // Create our different process IDs
    pid_t child_l, child_r;
    pid_t child_ll, child_lr;
    pid_t child_rl, child_rr;
    
    // First fork, creating 1 child
    child_l = fork();

    // If id = 0 (child)
    if (child_l == 0) {

        // Create a child from child (left-left)
        child_ll = fork();

        // If id = 0 (child-child)
        if (child_ll == 0) {
            printf("I am child_ll %d PID (left-left) %d \n\n", getpid(), getppid()); // child a a
            exit(0);
        }
        else {

            // Create a second child from child (left-right)
            child_lr = fork();
            
            // If id = 0 (child-child)
            if (child_lr == 0) {
                printf("I am child_lr %d PID (left-right) %d \n\n", getpid(), getppid()); // child a b
                exit(0);
            }
            
            // Else (id = 1), then original child
            else {
                printf("I am child_l %d PID (left) %d \n\n", getpid(), getppid()); // child a
                exit(0);
            }
        }
    }

    // Creating second child
    else {
        
        // Same code as above
        child_r = fork();

        // Creating child-child (right-left)
        if (child_r == 0) {
            child_rl = fork();

            if (child_rl == 0) {
                printf("I am child_rl %d PID (right-left) %d \n\n", getpid(), getppid()); //child b a
                exit(0);
            }

            // Creating child-child (right-right)
            else {
                child_rr = fork();
                
                if (child_rr == 0) {
                    printf("I am child_rr %d PID (right-right) %d \n\n", getpid(), getppid()); // child b b
                    exit(0);
                }
                
                // Else (id = 1), then original second child
                else {
                    printf("I am child_r %d PID (right) %d \n\n", getpid(), getppid()); // child b
                    exit(0);
                }
            }
        }
            
        // Last else statement is for the original parent
        else {
            printf("I am parent %d PID %d \n\n", getpid(), getppid()); //parent
        }
    }
    
    return 0;
}