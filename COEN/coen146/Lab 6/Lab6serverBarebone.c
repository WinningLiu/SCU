//Lab6 Server
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "lab6.h"

//getChecksum()
int getChecksum(Packet packet) {
    packet.header.cksum = 0;
    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len; 
    while (ptr < end) {
        checksum ^= *ptr++;
    }
    return checksum;
}

//print packet
void printPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

//serverSend()
void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    // Simulating a chance that ACK gets lost
    if (rand() % PLOSTMSG == 0) {
        printf("Dropping ACK\n");
    }
    else{
        Packet packet;
        //prepare and send the ACK
        //******STUDENT WORK******
        packet.header.seq_ack = seqnum;
        packet.header.len = sizeof(packet.data);
        if (rand()%10 != 0)
            packet.header.cksum = getChecksum(packet);
        else
            packet.header.cksum = 0;
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        printf("Sent ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
    }
}

Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum) //******STUDENT WORK******
{
                     
    Packet packet;

    while (1) {
        //Receive a packet from the client
        //******STUDENT WORK******
        recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        // validate the length of the packet
        //******STUDENT WORK******

        // print what was received
        printf("Received: ");
        printPacket(packet);

        //verify the checksum and the sequence number
        if (packet.header.cksum != getChecksum(packet)) {
            printf("Bad checksum, expected %d\n", getChecksum(packet));
            //******STUDENT WORK******
            serverSend(sockfd, address, *addrlen, !seqnum);
        } else if (packet.header.seq_ack != seqnum) {
            printf("Bad seqnum, expected %d\n", seqnum);
            //******STUDENT WORK******
            serverSend(sockfd, address, *addrlen, !seqnum);
        } else {
            printf("Good packet\n");
            //******STUDENT WORK******
            serverSend(sockfd, address, *addrlen, seqnum);
            break;
        }
    }
    printf("\n");
    return packet;
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <outfile>\n", argv[0]);
        exit(1);
    }

    // seed the RNG
    srand((unsigned)time(NULL));

    // create a socket
    //******STUDENT WORK******
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Failure to setup and endpoint socket");
        exit(1);
    }
    // initialize the server address structure
    //******STUDENT WORK******
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[1]));
    address.sin_addr.s_addr = INADDR_ANY;
    // bind the socket
    //******STUDENT WORK******
    if ((bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr))) < 0)
    {
        perror("Failure to bind server address to the endpoint socket");
        exit(1);
    }
    // open file using argv[2]
    int fp=open(argv[2],O_WRONLY | O_CREAT | O_TRUNC , 0644);
    if(fp<0){
	    perror("file failed to open\n");
	    exit(1);
    }
    // get file contents from client and save it to the file
    int seqnum = 0;
    Packet packet;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    do {
        //******STUDENT WORK******
        packet = serverReceive(sockfd, (struct sockaddr *)&clientaddr, &clientaddr_len, seqnum);
        write(fp, packet.data, packet.header.len);
        seqnum = (seqnum + 1) % 2;
    } while (packet.header.len != 0);

    //cleanup
    //******STUDENT WORK******
    close(fp);
    close(sockfd);
    return 0;
}
