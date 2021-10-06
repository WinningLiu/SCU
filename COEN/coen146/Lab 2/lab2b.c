#include <time.h>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fp1, fp2, rd, wr;
    clock_t start_t, end_t;
    double total_t;
    start_t = clock();
    fp1 = open("src.dat", O_RDONLY | O_CREAT, 0644);
    fp2 = open("destPart2.dat", O_WRONLY | O_CREAT, 0644);
    char buffer[100000];
    do{
        rd = read(fp1, buffer, 100000);
        if (rd) wr = write(fp2, buffer, 100000);
        else wr = 0;
    } while ((rd > 0) && ((rd == wr)));
    close(fp1);
    close(fp2);
    end_t = clock();
    total_t = ((double) (end_t - start_t)) / (CLOCKS_PER_SEC);
    printf("Time taken: %f\n", total_t);
}