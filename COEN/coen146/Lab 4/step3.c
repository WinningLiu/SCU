//Daren Liu
//2/2/21
//Lab 4 step 3
//Send a file to multiple clients 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFERSIZE 30

void* thread(void* sockfd){
    //initialize variables
    int socket = *(int*)sockfd;
    void* name = malloc(BUFFERSIZE);
    void* buffer = malloc(BUFFERSIZE);

    //receieve request
    recv(socket, name, BUFFERSIZE, 0);
    printf("File Requested: %s\n", (char*)name);

    //create file and file pointer
    FILE* fp = fopen((char*)name, "r");

    int count = 0;
    //read from file and send
    while(count = fread(buffer, 1, BUFFERSIZE, fp)){
        send(socket, buffer, count, 0);
    }
    //finished
    printf("Transfer of socket %d done\n", socket);
    close(socket);
    free(name);
    free(buffer);
}

int main(int argc, char* argv[]){
    //initialize variables
    pthread_t pthread;
    char* ip = "127.0.0.1";
    //use portnum 25000
    int portnum = atoi(argv[1]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //create socket address struct
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(portnum);
    socklen_t addlen = sizeof(address);

    //bind server to socket
    bind(sockfd, (struct sockaddr*)&address, sizeof(address));

    //listen for incoming packets/requests
    listen(sockfd, 5);
    int new_sockets;

    //use sleep to wait for all 5 requests before moving on
    sleep(10);
    //iterate indefinitely
    while (new_sockets = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addlen)){
        
        printf("New Connection\n");
        //create thread
        pthread_create(&pthread, NULL, thread, (void*)&new_sockets);

        pthread_join(pthread, NULL);
    }
    return 0;

}