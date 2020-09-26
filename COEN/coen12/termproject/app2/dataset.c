#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dataset.h"

typedef struct set
{
	int length;//max length
	int count;//number of studnets
        int *arrID;//array of ID
	int *age;//array of age
} SET;

SET *createDataSet(int maxElts)
{
	int i;
	SET *sp = malloc(sizeof(SET));
	assert(sp != NULL);
	sp->length = maxElts;//max length of 3000
	sp->count = 0;
    	sp->arrID = malloc(sizeof(int) * maxElts);
	sp->age = malloc(sizeof(int) * maxElts);
		for (i = 0; i < maxElts; i++)//initializes each array to -1 so that we know it is empty
		{
            		sp->arrID[i] = -1;
			sp->age[i] = -1;
		}
	assert(sp->age != NULL);
	return sp;
	//O(1)
}

void destroyDataSet(SET *sp)
{
	assert(sp != NULL);
    	free(sp->arrID);//frees both arrays
	free(sp->age);
	free(sp);//free set pointer
	return;
	//O(1)
}

int searchID(SET *sp, int ID)
{
	int i;
	assert(sp != NULL);
	if (sp->age[ID] > 0)//ID acts as index for both array. if the element at the index is not -1 then the student exists
	{
		printf("The student with an ID of %d has been found.\n", ID);
		return 1;
	}
	printf("The student with an ID of %d has not been found.\n", ID);
	return -1;
	//O(1)
}

void insertion(SET *sp, int ID, int age)
{
	assert(sp != NULL && sp->count < sp->length);
	if (sp->age[ID] > 0) //if the student already exists return
	{
		printf("Could not be inserted. Student ID already exists.\n");
		return;
	}
    	sp->arrID[ID] = ID;//sets ID
	sp->age[ID] = age;//sets age
	printf("Student with an ID of %d and age of %d has been inserted.\n", ID, age);
	sp->count++;//increments total count of students
	return;
	//O(1)
}

void deletion(SET *sp, int ID)
{
	assert(sp != NULL && sp->count < sp->length);
	if (sp->age[ID] == -1)//if the element at the index is -1 the student doesnt exist
	{
		printf("Could not be deleted. Student ID does not exist.\n");
        	return;
	}
    	printf("Student with ID %d has been deleted.\n", sp->arrID[ID]);
    	sp->arrID[ID] = -1;//resets the array index to -1
	sp->age[ID] = -1;
	sp->count--;//decrements total count
	return;
	//O(1)
}
