#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

typedef struct
{
    int	seq_ack;
    int len;
    int cksum;
} Header;

typedef struct
{
    Header	header;
    char	data[10];
} Packet;


