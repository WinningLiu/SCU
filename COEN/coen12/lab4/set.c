#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#define AVG_LENGTH 20

typedef struct set
{
	int count;
	int length;
	LIST **lists;
	int (*compare)();
	unsigned(*hash)();
} SET;


SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
	SET *sp;
	sp = malloc(sizeof(SET));
	assert(sp != NULL);//makes sure that sp was malloced corretly
	sp->count = 0;
	sp->length = maxElts/AVG_LENGTH; //length of the array
	sp->lists = malloc(sizeof(void*)*sp->length);
	sp->compare = compare;
	sp->hash = hash;
	
	int i;
	for (i = 0; i < sp->length; i++) //creates an empty list at each spot in the array
		sp->lists[i] = createList(compare);

	assert(sp->lists!= NULL);//makes sure that the list was malloced correctly.
	return sp;
	//O(n)
}

void destroySet(SET *sp)
{
	free(sp->lists);//free the list
	free(sp);//free the set 
	//O(1)
}

int numElements(SET *sp)
{
	assert(sp != NULL);
	return sp->count;//returns the elements in the set
	//O(1)
}

void addElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);//makes sure that sp and elt are not empty
	int index = (*sp->hash)(elt)%sp->length; // gives us the index where the element should be placed in
	if (findItem(sp->lists[index], elt) == NULL) //if the element is not found at the index, insert it to the list
	{
		addFirst(sp->lists[index], elt);
		sp->count++;
	}
	//O(n)
}

void removeElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);//makes sure that sp and elt are not empty
	int index = (*sp->hash)(elt)%sp->length; //gives us the index where the element should place in
	if (findItem(sp->lists[index], elt) != NULL) //if the item in the list is found, remove it
	{
		removeItem(sp->lists[index], elt);
		sp->count--;
	}
	//O(n)
}

void *findElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);//makes sure that sp and elt are not empty
	int index = (*sp->hash)(elt)%sp->length;//gives an index where the element should be placed in
	return findItem(sp->lists[index], elt);//returns the 
	//O(n)
}

void *getElements(SET *sp)
{
	assert(sp != NULL);//makes sure that sp is not empty
	void **arr;
	int copiedcount = 0;//keeps track of the index of arr
	arr = malloc(sizeof(void*) * sp->count);
	int i;
	int j;
	for (i = 0; i < sp->length; i++)
	{
		void **copiedArray = getItems(sp->lists[i]);//copies the first list
	
		for (j = 0; j < numItems(sp->lists[i]); j++)
		{
			arr[copiedcount] = copiedArray[j];//copies the items inside the list
			copiedcount++;
		}
	}
	return arr;//returns the copied array
	//O(n^2)
}
