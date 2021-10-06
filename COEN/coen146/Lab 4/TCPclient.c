//Daren Liu
//2/2/21
//Lab 4 client side
//Send a request and receive file from server

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
    char* ip = "127.0.0.1";
    int port = atoi(argv[1]);
    char* message = "COEN146Lab0.txt";

    //create socket and get fd
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //create socket address struct
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    //connect to server
    connect(sockfd, (struct sockaddr*)&address, sizeof(address));

    //send name of file
    int bytes = send(sockfd, message, strlen(message) + 1, 0);
    printf("%d bytes sent\n", strlen(message));

    void* buffer = malloc(BUFFERSIZE);
    int count;
    FILE* fp = fopen("ReceivedFile.txt", "w");
    //receive file
    while(count = recv(sockfd, buffer, BUFFERSIZE, 0)){
        fwrite(buffer, count, 1, fp);
    }

    //finished
    printf("File received\n");
    close(sockfd);
    free(buffer);
    return 0;
}