#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"

#define EMPTY 0
#define DELETED 1
#define FILLED 2

unsigned strhash();

typedef struct set
{
	int count;
	int length;
	void **data;
	char *flag;
	int (*compare)(); //functions
	unsigned (*hash)();
} SET;

static int search(SET *sp, void *elts, bool *found)
{
	assert(sp != NULL && elts != NULL);
	int key = (*sp->hash)(elts) % sp->length;
	int del = -1;
	
	while (sp->flag[key] != EMPTY)
	{
		if (sp->flag[key] == DELETED && del < 0)
			del = key;
		if (sp->flag[key] == FILLED && (*sp->compare)(sp->data[key], elts) == 0)
		{
			*found = true;
			return key;
		}
		key = (key + 1) % sp->length;
	}
	*found = false;
	if (del > 0)
		return del;
	else
		return key; 
	//O(n)
}

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
	int i;
	SET *sp = malloc(sizeof(SET)); //allocates memory for the entire set
        assert(sp != NULL); //makes sure that sp was malloced correctly
        sp->length = maxElts;
        sp->count = 0;
        sp->data = malloc(sizeof(void*) * maxElts); //allocates memory for the array data
        assert (sp->data != NULL);
	sp->flag = malloc(sizeof(char) * maxElts);
	for (i = 0; i < maxElts; i++)
		sp->flag[i] = EMPTY;
	assert(sp->flag != NULL); //makes sure that data was malloced correctly
	sp->compare = compare;
	sp->hash = hash;
        return sp;
        //O(1)
}

void destroySet (SET *sp)
{
        assert(sp != NULL);//makes sure sp exists
	free(sp->flag);
	free(sp->data);
        free(sp);
        return;
        //O(1)
}

int numElements(SET *sp)
{
	assert(sp != NULL);
	return sp->count;
	//O(1)
}

void addElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);
	bool found = false;
	int i = search(sp, elt, &found);
	assert(sp->length > sp->count);
	if (found == false)
	{
		sp->data[i] = elt;
		sp->flag[i] = FILLED;
		sp->count++;
	}
	return;
	//O(n)
}

void removeElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);
	bool found = false;
	int i = search(sp, elt, &found);
	if (found == true)
	{
		sp->data[i] = NULL;
		sp->flag[i] = DELETED;
		sp->count--;
	}
	return;
	//O(n)
}

void *findElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);
	bool found = false;
	int i = search(sp, elt, &found);
	if (found == false)
		return NULL;
	else
		return sp->data[i];
	//O(n)
}

void *getElements(SET *sp)
{
	assert(sp != NULL);
	int i;
	int j = 0;
	void  **data2;
        data2 = malloc(sp->count * sizeof(void *));
        assert(sp != NULL);
	for (i = 0; i < sp->length; i++)
	{
		if (sp->flag[i] == FILLED)
		{
			data2[j] = sp->data[i];
			j++;
		}
        }
	return data2;
        //O(n)
}

