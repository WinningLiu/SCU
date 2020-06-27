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
	char **data;
	char *flag;
}SET;

static int search(SET *sp, char *elts, bool *found)
{
	unsigned index = strhash(elts) % sp->length;
	int key = index;
	int del = -1;
	
	while (sp->flag[key] != EMPTY)
	{
		if (sp->flag[key] == DELETED && del < 0)
			del = key;
		if (sp->flag[key] == FILLED && strcmp(sp->data[key], elts) == 0)
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

SET *createSet(int maxElts)
{
	int i;
	SET *sp = malloc(sizeof(SET)); //allocates memory for the entire set
        assert(sp != NULL); //makes sure that sp was malloced correctly
        sp->length = maxElts;
        sp->count = 0;
        sp->data = malloc(sizeof(char*) * maxElts); //allocates memory for the array data
        sp->flag = malloc(sizeof(char) * maxElts);
	for (i = 0; i < maxElts; i++)
		sp->flag[i] = EMPTY;
	assert(sp != NULL); //makes sure that data was malloced correctly
        return sp;
        //O(n)
}

void destroySet (SET *sp)
{
	int i;
        assert(sp != NULL);//makes sure sp exists
        for (i = 0; i < sp->length; i++)
	{
		if (sp->flag[i] == FILLED) 
			free(sp->data[i]);
        }
	free(sp->data);
	free(sp->flag);
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
	assert(sp != NULL);
	bool found = false;
	int i = search(sp, elt, &found);
	if (found == true)
		return;
	else if (sp->length > sp->count)
	{
		sp->data[i] = malloc((strlen(elt) + 1) * sizeof(char));
		strcpy(sp->data[i], elt);
		sp->flag[i] = FILLED;
		sp->count++;
	}
	return;
	//O(n)
}

void removeElement(SET *sp, char *elt)
{
	assert(sp != NULL);
	bool found = false;
	int i = search(sp, elt, &found);
	if (found == true)
	{
		free(sp->data[i]);
		sp->flag[i] = DELETED;
		sp->count--;
	}
	return;
	//O(n)
}

char *findElement(SET *sp, char *elt)
{
	bool found = false;
	int i = search(sp, elt, &found);
	if (found == false)
		return NULL;
	else
		return elt;
	//O(n)
}

char **getElements(SET *sp)
{
	int i;
	int j = 0;
	char **data2;
        data2 = malloc(sp->count * sizeof(char *));
        assert(sp != NULL);
	for (i = 0; i < sp->length; i++)
	{
		if (sp->flag[i] == FILLED)
		{
			data2[j] = malloc((strlen(sp->data[i]) + 1) * sizeof(char));
			strcpy(data2[j], sp->data[i]);
			j++;
		}
        }
	return data2;
        //O(n)
}

unsigned strhash(char *s)
{
	unsigned hash = 0; 
	while (*s != '\0')
		hash = 31 * hash + *s ++;

	return hash;
	//O(1)
}
