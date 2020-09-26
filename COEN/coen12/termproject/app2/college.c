#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset.c"
#include <time.h>
#define MAX 3000

int main()
{
    srand(time(NULL));
    SET *sp = createDataSet(MAX);//creates a new set
    int i, age, n, temp = 0;
    
    for (i = 0; i < 1000; i++)//creates 1000 new studnets
    {
        age = (rand() % (30 - 18 + 1)) + 18;//random age
        n = (rand() % 2) + 1;//random initial id
        temp += n;//random new id
        insertion(sp, temp, age);
    }
    
    int sID = (rand() % 2000) + 1;//random new ID between 1 and 2000;
    searchID(sp, sID);//searches for the random id
    deletion(sp, sID);//deletes the random id 
    destroyDataSet(sp);//destroys the set
	return 1;
}
