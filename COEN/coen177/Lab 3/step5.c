/*
Daren Liu
1/25/2021
Lab3 step 5
Producer sends the consumer numbers 0-9 and both print them out in an order
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main(int argc,char *argv[]){
   int  fds[2];
   char buff[20]; //for the consumer to use
   char buffer[20]; //for the producer to use
   int count;
   int i;
   pipe(fds);
   
   //Producer
   if (fork()==0){
       printf("\nWriter on the upstream end of the pipe -> %d arguments \n",argc);
       close(fds[0]);
       
       //for the number of commandd line arguments
       for(i=1;i<argc;i++){
		   //concatanate all arguments to buffer
           strcat(buffer, argv[i]);
       }
	   //write from buffer to fds[1]
       write(fds[1],buffer, strlen(buffer));
       exit(0);
   }
   //Consumer
   else if(fork()==0){
       printf("\nReader on the downstream end of the pipe \n");
       close(fds[1]);
       
       // Read 20 bytes from fds[0] and stores it in buff
       read(fds[0], buff, 20);
	   
       // Write the contents of buff to the stdout
       write(1,buff, strlen(buff));
       printf("\n");
       exit(0);
    }
   else{     
      close(fds[0]);
      close(fds[1]);
      wait(0);
      wait(0);
   }
return 0;
}