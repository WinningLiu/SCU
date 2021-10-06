//Daren Liu
//2/2/21
//Lab 4 server side
//Receive a request from client 

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

int main(int argc, char* argv[]){
    //initialize variables
    int portnum = atoi(argv[1]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //create socket address struct
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portnum);
    socklen_t addlen = sizeof(address);

    //bind server to socket
    bind(sockfd, (struct sockaddr*)&address, sizeof(address));

    //listen to incoming requests
    listen(sockfd, 1);

    //accept request, initialize buffers
    int new_sockets = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addlen);
    printf("Connection accepted\n");
    void* name = malloc(BUFFERSIZE);
    void* buffer = malloc(BUFFERSIZE);

    //receieve and get name of requested file
    recv(new_sockets, name, BUFFERSIZE, 0);
    printf("File Requested: %s\n", (char*)name);

    //create file
    FILE* fp = fopen((char*)name, "r");

    int count = 0;
    //read and send info in buffer
    while(count = fread(buffer, 1, BUFFERSIZE, fp)){
        send(new_sockets, buffer, count, 0);
    }

    //finished
    close(new_sockets);
    free(name);
    free(buffer);


}