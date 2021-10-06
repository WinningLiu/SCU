//Lab5 Client

/*
Daren Liu
2/9/21
Lab 5 Client
Implements rdt2.2 on the client side
*/
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
#include "lab5.h"

//getChecksum()
int getChecksum(Packet packet) {
    packet.header.cksum = 0;

    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;

//Find the error (if any) from the remaining part getChecksum() function and correct it
//******STUDENT WORK****** 
    while (ptr < end) {
        checksum ^= *ptr++; //changed ptr to *ptr
    }

    return checksum;
}

// printPacket()
void printPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}


//clientSend()
void clientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet) {
    while (1) {
        packet.header.cksum=getChecksum(packet);
        // send the packet
        printf("Client: sending packet\n");
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
		//******STUDENT WORK******

        // receive an ACK from the server
        Packet recvpacket;
		//******STUDENT WORK******
        recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, address, &addrlen);

        // log what was received
        printf("Received ACK %d, checksum %d - ", recvpacket.header.seq_ack, recvpacket.header.cksum);

        //print received packet
        //******STUDENT WORK****** 

        // validate the ACK
        if (recvpacket.header.cksum != getChecksum(recvpacket)) //******STUDENT WORK******  
        {
            // bad checksum, resend packet
            printf("Bad checksum, expected checksum was: %d\n", getChecksum(recvpacket)); //******STUDENT WORK*****
        } else if (recvpacket.header.seq_ack != packet.header.seq_ack) //******STUDENT WORK******
        {
            // incorrect sequence number, resend packet
            printf("Bad seqnum, expected sequence number was: %d\n", packet.header.seq_ack);	//******STUDENT WORK******
        } else {
            // good ACK, we're done
            printf("Good ACK\n");
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip> <port> <infile>\n", argv[0]);
        return 1;
    }

    // seed the RNG
    srand((unsigned)time(NULL));

    // create and configure the socket [name the socket sockfd] 
	//******STUDENT WORK******	    
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failure to setup an endpoint socket");
        exit(1);
    }

    // initialize the server address structure using argv[2] and argv[1]
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
	//******STUDENT WORK******
    socklen_t addr_len = sizeof(address);
    struct hostent *host;
    host = (struct hostent *) gethostbyname(argv[1]);
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[2]));
    address.sin_addr = *((struct in_addr *)host->h_addr);

    // open file using argv[3]
	//******STUDENT WORK******
    int fp = open(argv[3], O_RDWR);

    // send file contents to server
	//-create a packet
	//-set the seq_ack field 
	//-set the len field
	//-calculate the checksum
	//-send the packet
	//******STUDENT WORK******
    Packet packet;
    int seqnum = 0;
    int bytesRead;
    while((bytesRead = read(fp, packet.data, sizeof(packet.data))) > 0){ // Reads from file then sends to server
        //printf("Client sending file contents to server...\n")
        packet.header.seq_ack = seqnum;
        packet.header.len = bytesRead;
        packet.header.cksum = getChecksum(packet);
        clientSend(sockfd, (struct sockaddr *)&address, addr_len, packet);
        seqnum = (seqnum + 1) % 2;
    }

    // send zero-length packet to server to end connection
	//******STUDENT WORK******
    Packet zero_length;
    zero_length.header.seq_ack = seqnum;
    zero_length.header.len = 0;
    zero_length.header.cksum = getChecksum(zero_length);
    clientSend(sockfd, (struct sockaddr *)&address, addr_len, zero_length);

    // clean up
	//-close the sockets and file
    	//******STUDENT WORK******
    close(fp);
    close(sockfd);
    return 0;
}
