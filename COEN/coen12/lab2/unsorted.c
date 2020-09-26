#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

typedef struct set 
{
	int count;
	int length;
	char ** data;
} SET;

static int search(SET *sp, char *elt)
{
	int i;
	for (i = 0; i < sp->count; i++) //sequential search
	{
		if (strcmp(sp->data[i], elt) == 0) //if the compare is 0 then the search found a string that is the same as elt
			return i; //returns the index where the string is
	}
	return -1; //returns -1 if string is not found

	//O(n)
}

SET *createSet(int maxElts)
{
	SET *sp = malloc(sizeof(SET)); //allocates memory for the entire set
	assert(sp != NULL); //makes sure that sp was malloced correctly
	sp->length = maxElts;
	sp->count = 0;
	sp->data = malloc(sizeof(char*) * maxElts); //allocates memory for the array data
	assert(sp != NULL); //makes sure that data was malloced correctly
	return sp;
	//O(1)
}

void destroySet(SET *sp)
{
	int i;
	assert(sp != NULL);//makes sure sp exists
	for (i = 0; i < sp->count; i++) 
		free(sp->data[i]); 
	free(sp->data);
	free(sp);
	return; 
	//O(n)
}

int numElements(SET *sp)
{
	assert(sp != NULL); 
	return sp->count; 
	//O(1)
}

void addElement(SET *sp, char *elt)
{
	if (search(sp, elt) >= 0) //if the element exists exits out of the function
		return;
	else if (sp->length > sp->count) //makes sure that it doesn't insert beyond length
	{
		sp->data[sp->count] = malloc((strlen(elt) + 1) * sizeof(char)); //malloc includes the '\0'
		strcpy(sp->data[sp->count], elt);
		sp->count++; //increases count
		return;
	}
	return;
	//O(n)
}

void removeElement(SET *sp, char *elt)
{
	int i = search(sp, elt); //calles search
	if (i != -1) //if search returns anything not -1 then it found the element
	{
		free(sp->data[i]); //frees the string at data[i]
		if (i != sp->count) //if i is not equal to the last element
		{
			sp->data[i] = sp->data[sp->count-1]; //replaces the empty space at i with the last element
		}
		sp->count--;
	}
	return;
	//O(n)
}

char *findElement(SET *sp, char *elt)
{
	int i = search(sp, elt);
	if (i == -1) //if nothing is found in search return null
		return NULL;
	else 
		return sp->data[i]; //returns the string 
	//O(n)
}


char **getElements(SET *sp)
{
	char **data2;
	data2 = malloc(sp->count * sizeof(char *)); 
	assert(sp != NULL);
	memcpy(data2, sp->data, sp->count * sizeof(char*)); //copies data into data2
	return data2;
	//O(1)
}
