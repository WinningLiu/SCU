#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int a(int RTT0, int RTT1, int RTT2, int RTTHTTP)
{
    return (RTT0 + RTT1 + RTT2 + (2*RTTHTTP));
}

int b(int RTT0, int RTT1, int RTT2, int RTTHTTP)
{
    return (RTT0 + RTT1 + RTT2 + (2*RTTHTTP) + (2*6*RTTHTTP));
}

void c(int RTT0, int RTT1, int RTT2, int RTTHTTP)
{
    printf("Part C: \n");
    int i;
    //assume 6 objects
    //non-persistent
    int arr[12];
    int RTTtotal = RTT0 + RTT1 + RTT2;
    arr[0] = RTTtotal + (2*RTTHTTP) + (6/1)*(2*RTTHTTP);
    arr[1] = RTTtotal + (2*RTTHTTP) + (6/2)*(2*RTTHTTP);
    arr[2] = RTTtotal + (2*RTTHTTP) + (6/3)*(2*RTTHTTP);
    arr[3] = RTTtotal + (2*RTTHTTP) + (6/4)*(2*RTTHTTP);
    arr[4] = RTTtotal + (2*RTTHTTP) + (6/5)*(2*RTTHTTP);
    arr[5] = RTTtotal + (2*RTTHTTP) + (6/6)*(2*RTTHTTP);
    for (i = 0; i < 6; i++)
    {
        printf("Non-persistent with %d parallel processes: %dms\n", i + 1, arr[i]);
    }
    //persistent
    arr[6] = RTTtotal + (2*RTTHTTP) + (6/1)*(RTTHTTP);
    arr[7] = RTTtotal + (2*RTTHTTP) + (6/2)*(RTTHTTP);
    arr[8] = RTTtotal + (2*RTTHTTP) + (6/3)*(RTTHTTP);
    arr[9] = RTTtotal + (2*RTTHTTP) + (6/4)*(RTTHTTP);
    arr[10] = RTTtotal + (2*RTTHTTP) + (6/5)*(RTTHTTP);
    arr[11] = RTTtotal + (2*RTTHTTP) + (6/6)*(RTTHTTP);
    for (i = 6; i < 12; i++)
    {
        printf("Persistent with %d parallel processes: %dms\n", i - 5, arr[i]);
    }
}

int main()
{
    int RTT0 = 3, RTT1 = 20, RTT2 = 26;
    int RTTHTTP = 47;
    int a0 = a(RTT0, RTT1, RTT2, RTTHTTP);
    int b0 = b(RTT0, RTT1, RTT2, RTTHTTP);
    printf("Part A: %dms\n", a0);
    printf("Part B: %dms\n", b0);
    c(RTT0, RTT1, RTT2, RTTHTTP);
}