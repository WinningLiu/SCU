#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"

typedef struct set 
{
	int count;
	int length;
	char ** data;
} SET;

static int search(SET *sp, char *elt, bool *found)
{
	int lo = 0, hi = sp->count-1, mid;
	while (lo <= hi)
	{
		mid = (hi + lo ) / 2;
		if (strcmp(elt, sp->data[mid]) < 0)
			hi = mid - 1;
		else if (strcmp(elt, sp->data[mid]) > 0)
			lo = mid + 1;
		else {
			*found = true;
			return mid; // returns mid if found
		}
	}
	*found = false;
	return lo; //returns lo if string is not found
	//O(logn)
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
	bool found = false;
	int i = search(sp, elt, &found);
	int j;
	if (found == true) //if the element exists exits out of the function
		return;
	else if (sp->length > sp->count) //makes sure that it doesn't insert beyond length
	{
		for (j = sp->count; j > i; j--)
		{
			sp->data[j] = sp->data[j - 1]; // shifts j - 1 to j
		}
		sp->data[i] = strdup(elt);
                sp->count++; //increases count
	}
	return;
	//O(n)
}

void removeElement(SET *sp, char *elt)
{
	bool found = false;
	int i = search(sp, elt, &found); //calls search
	int j;
	if (found == true) //if search returns true then it found the element
	{
		free(sp->data[i]); //frees the string at data[i]
		if (i != sp->count-1) //if i is not equal to the last element
		{
			for (j = i; j < sp->count; j++) //shifts j+1 to j
			{
				sp->data[j] = sp->data[j+1];
			}
		}
		sp->count--;
	}
	return;
	//O(n)
}

char *findElement(SET *sp, char *elt)
{
	bool found = false;
	int i = search(sp, elt, &found);
	if (found == false) //if nothing is found in search return null
		return NULL;
	else 
		return sp->data[i]; //returns the string 
	//O(logn)
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
