//Ryan Summers
//Coen 146 Lab 6
//Server for stop and wait for an unreliable channel with loss
//same functionality as lab 5 but with added chance to drop a packet

//Explain what each library does in the context of this lab. Do we need all of them?
#include <arpa/inet.h>//used for in_port_t and in_addr_t
#include <stdio.h>//used for file reading and writing
#include <stdlib.h>//used for many things, such as srand() and rand()
#include <string.h>//used for manipulating arrays of characters, strlen() used
#include <sys/socket.h>//used for socket programming such as socklen_t and SOCK_DGRAM
#include <time.h>//used for creating random numbers based on time
#include <unistd.h>//not required for lab

#include <netinet/in.h>//defines types in_port_t and in_addr_t
#include "lab6.h"


// NOTE: beware of sign extension if the checksum is stored in a char, then converted to an int!
int getChecksum(Packet packet) {
    packet.header.cksum = 0;

    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;

//Please find an error from the remianing part getChecksum() function 
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

//Sending a ACK to the client, i.e., letting the client know what was the status of the packet it sent.
void ServerSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {

	// Simulating a chance that ACK gets lost
	if(rand()%10 == 0){
		printf("Dropping packet\n");
	}
	else{
    	Packet packet;
		//-Prepare the packet headers
		//-send the packet
		//******STUDENT WORK******
    	packet.header.seq_ack = seqnum;
    	packet.header.len = sizeof(packet.data);
		if(rand()%10 != 0){
    		packet.header.cksum = getChecksum(packet);
		}
		else
			packet.header.cksum = 0;
    	sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);

    	printf("Sent ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
	}
}

Packet ServerReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum) {
    Packet packet;

    while (1) {
        //recv packets from the client
		//******STUDENT WORK******
        recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // log what was received
        printf("Received: ");
        logPacket(packet);

        if (packet.header.cksum != getChecksum(packet)) {//******STUDENT WORK******
            printf("Bad checksum, expected checksum was: %d\n", getChecksum(packet));
            ServerSend(sockfd, address, *addrlen, !seqnum);
        }else if (packet.header.seq_ack != seqnum) {
            printf("Bad seqnum, expected sequence number was:%d\n", seqnum);//******STUDENT WORK******
            ServerSend(sockfd, address, *addrlen, !seqnum);
        }
        else {
            printf("Good packet\n");
					//******STUDENT WORK******

            ServerSend(sockfd, address, *addrlen, seqnum);
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
        return EXIT_FAILURE;
    }

    // seed the RNG
    srand((unsigned)time(NULL));

    // create a socket
	  //******STUDENT WORK******
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failure to setup an endpoint socket");
        exit(1);
    }

    // initialize the server address structure using argv[1]
    struct sockaddr_in address;
		//******STUDENT WORK******

    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[1]));
    address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket
		// ******STUDENT WORK******
	// No, a specific client-side port is not needed
    if ((bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr))) < 0){
        perror("Failure to bind server address to the endpoint socket");
        exit(1);
    }

	//Please answer the following questions
	//[Q1] Briefly explain what is "AF_INET" used for.
 	//Answer:
	//It is used to speficy the type of address the socket can communicate with, it is IPv4 addresses in this case
	//[Q2] Briefly explain what is "SOCK_DGRAM" used for.
	//Answer:
    //SOCK_DGRAM specifies that we are using an unreliable channel(UDP)
	//[Q3] Briefly explain what is "htons" used for.
	//Answer:
	//Used to convert an IP port number in host byte order to the IP port number in network byte order
   	//[Q4] Briefly explain why bind is required on the server and not on the client.
   	//Answer:
   	//A client only needs to be aware of the server port it needs to connect to, the server needs to bind to an IP address and port so that the client can connect to it


    // open file using argv[2]
		//******STUDENT WORK******
    FILE *fd = fopen(argv[2], "w");

    // get file contents from client
    int seqnum = 0;
    Packet packet;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    do {
        packet = ServerReceive(sockfd, (struct sockaddr *)&clientaddr, &clientaddr_len, seqnum);//******STUDENT WORK******
        fwrite(packet.data,1,strlen(packet.data),fd);
        seqnum = (seqnum + 1) % 2;
    } while (packet.header.len != 0);

    fclose(fd);
    close(sockfd);
    return 0;
}
