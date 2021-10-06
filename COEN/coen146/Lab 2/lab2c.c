#include <time.h>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <string.h>

struct args {
    char* input;
    char* output;
};

void *copying (void* argument)
{
    FILE* fp1;
    FILE *fp2;
    struct args* argument1 = argument; 
    fp1 = fopen(argument1->input, "rb");
    fp2 = fopen(argument1->output, "wb");
    unsigned char buff[1000];
    int n, m;
    do {
        fread(buff, 1, sizeof(buff), fp1);
        if (n) m = fwrite(buff, 1, n, fp2);
        else m = 0;
    } while ((n > 0) && (n == m));
    fclose(fp1);
    fclose(fp2);
}

char* dest(int i)
{
    if (i == 0)
        return "dest0.dat";
    else if (i == 1)
        return "dest1.dat";
    else if (i == 2)
        return "dest2.dat";
    else if (i == 3)
        return "dest3.dat";
    else if (i == 4)
        return "dest4.dat";
    else if (i == 5)
        return "dest5.dat";
    else if (i == 6)
        return "dest6.dat";
    else if (i == 7)
        return "dest7.dat";
    else if (i == 8)
        return "dest8.dat";
    else if (i == 9)
        return "dest9.dat";
    return "";
}

char* src(int i)
{
    if (i == 0)
        return "src0.dat";
    else if (i == 1)
        return "src1.dat";
    else if (i == 2)
        return "src2.dat";
    else if (i == 3)
        return "src3.dat";
    else if (i == 4)
        return "src4.dat";
    else if (i == 5)
        return "src5.dat";
    else if (i == 6)
        return "src6.dat";
    else if (i == 7)
        return "src7.dat";
    else if (i == 8)
        return "src8.dat";
    else if (i == 9)
        return "src9.dat";
    return "";
}

int main(int argc, char *argv[])
{
    clock_t start_t, end_t, total_t;
    start_t = clock();

    int i;
    pthread_t pid;
    for (i = 0; i < 10; i++)
    {
        struct args *total = (struct args*)malloc(sizeof(struct args));
        total->input = src(i);
        total->output = dest(i);
        pthread_create(&pid, NULL, copying, (void*)total);
    }
    pthread_join(pid, NULL);
    end_t = clock();
    total_t = ((double) (end_t - start_t)) / (CLOCKS_PER_SEC);
    printf("Time taken: %f\n", total_t);
}