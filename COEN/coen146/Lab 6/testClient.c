//Ryan Summers
//Coen 146 Lab 6
//Client for stop and wait with an unreliable channel that has loss
//Incorporates a timer to decide whether a timeout occurs

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



// NOTE: beware of sign extension if the checksum is stored in a char, then converted to an int!
int getChecksum(Packet packet) {
    // reset to 0 because we want to calculate checksum the same way the server did
    packet.header.cksum = 0;

    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;

//Please find an error from the remaining part getChecksum() function
//******STUDENT WORK******
    while (ptr < end) {
        checksum ^= *ptr++; //changed ptr to *ptr 
    }

    return checksum;
}

void logPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}



void ClientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet, unsigned retries) {
    while (1) {
		//calculate the checksum
		//******STUDENT WORK******		
		if(rand()%10 != 0){
			packet.header.cksum = getChecksum(packet);
		}
		else
			packet.header.cksum = 0;
		//simulate lost packet.
		if(rand() %10 == 0){
			printf("Dropping packet\n");
		}
		else{
        // send the packet
        printf("Sending packet ACK:%d\n",packet.header.seq_ack);
			//******STUDENT WORK******
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
		}
		//wait until either a pakcet is recieve or timeout, i.e. using the select statement
		struct timeval tv;
		tv.tv_sec = 3;
		tv.tv_usec = 0;
		int rv;
		fd_set readfds;
		fcntl(sockfd, F_SETFL, O_NONBLOCK);
		FD_ZERO(&readfds);
		FD_SET(sockfd,&readfds);
		rv = select(sockfd +1, &readfds, NULL, NULL, &tv);
		
		if(rv==0){//timeout
			printf("Timeout...\n");
		}
		else if(rv == 1){

        // receive an ACK from the server
        Packet recvpacket;
			//******STUDENT WORK******
        recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, address, &addrlen);

        // log what was received
        printf("Received ACK %d, checksum %d - ", recvpacket.header.seq_ack, recvpacket.header.cksum);

        // validate the ACK
        if (recvpacket.header.cksum != getChecksum(recvpacket)) {//******STUDENT WORK******
            // bad checksum, resend packet
            printf("Bad checksum, expected checksum was: %d\n", getChecksum(recvpacket));
        }else if (recvpacket.header.seq_ack != packet.header.seq_ack) {//******STUDENT WORK******
     		 // incorrect sequence number, resend packet
            printf("Bad seqnum, expected sequence number was: %d\n", packet.header.seq_ack);//******STUDENT WORK******
        }else {
            // good ACK, we're done
            printf("Good ACK\n");
            break;
        }
		}
    }

    printf("\n");
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
	// ******STUDENT WORK******
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
	// ******STUDENT WORK******
    FILE *fp = fopen(argv[3], "r");

    // send file contents to server
	//-create a packet
	//-set the seq_ack field 
	//-set the len field
	//-calculate the checksum
	//-send the packet
	//******STUDENT WORK******
    Packet packet;
	int seqnum = 0;
    while(fgets(packet.data, 1024, fp) != NULL){
        packet.header.seq_ack = seqnum;
        packet.header.len = strlen(packet.data);
        ClientSend(sockfd, (struct sockaddr *)&address, addr_len, packet,5);
		seqnum = (seqnum +1) % 2;
    }


    // send zero-length packet to server to end connection
	// ******STUDENT WORK******
    Packet zero_length;
    zero_length.header.seq_ack = seqnum;
    zero_length.header.len = 0;
    zero_length.header.cksum = getChecksum(zero_length);
    ClientSend(sockfd, (struct sockaddr *)&address, addr_len, zero_length,5);

    // clean up
	//-close the sockets and file
	//******STUDENT WORK******
    fclose(fp);
    close(sockfd);

    return 0;
}
