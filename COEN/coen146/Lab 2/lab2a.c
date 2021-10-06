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
    clock_t start_t, end_t, total_t;
    start_t = clock();
    FILE *fp1, *fp2;
    fp1 = fopen("src.dat", "rb");
    if (fp1 == NULL)
    {
        printf("fp1 error \n");
        return 0;
    }
    fp2 = fopen("destPart1.dat", "wb");
    if (fp2 == NULL)
    {
        printf("fp2 error \n");
        return 0;
    }
    unsigned char buff[100000];
    int n, m;
    do{
        n = fread(buff, 1, sizeof(buff), fp1);
        if (n) m = fwrite(buff, 1, n, fp2);
        else m = 0;
    } while ((n > 0) && (n==m));
    
    fclose(fp1);
    fclose(fp2);
    end_t = clock();
    total_t = ((double) (end_t - start_t)) / (CLOCKS_PER_SEC);
    printf("Time taken: %le\n", total_t);
}