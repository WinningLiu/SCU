#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "set.h"

typedef struct set {
    int count;
    int length;
    LIST ** list;
    int (*compare)();
    unsigned(*hash)();
} SET;

struct node
{
	void *data;
	struct node *next;	
	struct node *prev;
};

typedef struct list
{
	int count;
	struct node * head;
	int (*compare)();
} LIST;

LIST *createList(int (*compare)())
{
	LIST *lp = malloc(sizeof(LIST));
	assert(lp != NULL);
	lp->count = 0;
	lp->head = malloc(sizeof(struct node));
	lp->head->next = lp->head;
	lp->head->prev = lp->head;
	lp->compare = compare;
    	return lp;
}

void destroyList(LIST *lp)
{
	assert(lp != NULL);
	struct node *pDel, *pNext;
	pDel = lp->head;
	
	do {
		pNext = pDel->next;
		free(pDel);
		pDel = pNext;
	} while (pDel != lp->head);

	free(lp);
}

int numItems(LIST *lp)
{
	assert(lp != NULL);
	return lp->count;
}

void addFirst(LIST *lp, void *item)
{
	struct node *Cur;
	struct node *dum; //dummy node
	assert(lp != NULL && item != NULL);
	Cur = malloc(sizeof(struct node));
	Cur->data = item;
	dum = lp->head;

	Cur->prev = dum;
	Cur->next = dum->next;
	dum->next->prev = Cur;
	dum->next = Cur;
	lp->count++;
}

void addLast(LIST *lp, void *item)
{
	assert(lp != NULL);
	struct node *Cur;
	struct node *dum; //dummy node
	Cur = malloc(sizeof(struct node));
	Cur->data = item;
	dum = lp->head;
	
	Cur->next = dum;
	Cur->prev = dum->prev;
	dum->prev->next = Cur;
	dum->prev = Cur;
	lp->count++;
}

void *removeFirst(LIST *lp)
{
	assert(lp != NULL);
	if (lp->count == 0)
		return NULL;

	struct node *temp;
	struct node *dum;
	void *store;
	dum = lp->head;
	temp = dum->next;
	store = temp->data;
	
	dum->next = temp->next;
	temp->next->prev = dum;
	free(temp);
	lp->count--;
	return store;
}

void *removeLast(LIST *lp)
{
	assert(lp != NULL);
	if (lp->count == 0)
		return NULL;
	struct node *temp;
	struct node *dum;
	void *store;
	dum = lp->head;
	temp = dum->prev;
	store = temp->data;
	
	temp->prev->next = dum;
	dum->prev = temp->prev;
	free(temp);
	lp->count--;
	return store;
}

void *getFirst(LIST *lp)
{
	assert(lp != NULL);
	return lp->head->next->data;
}

void *getLast(LIST *lp)
{
	assert(lp != NULL);
	return lp->head->prev->data;
}

void removeItem(LIST *lp, void *item)
{
	assert(lp != NULL && item != NULL);
	struct node *temp;
	temp = lp->head->next;
	while (temp != lp->head)
	{
		if ((*lp->compare)(temp->data, item) == 0)
		{
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			free(temp);
			return;
		}	
		temp = temp->next;
	}
}

void *findItem(LIST *lp, void *item)
{
	assert(lp != NULL && item != NULL);
	struct node *temp;
	temp = lp->head->next;
	while (temp != lp->head)
	{
		if((*lp->compare)(temp->data,item) == 0)
			return temp->data;
		temp = temp->next;
	}
	return NULL;
}

void *getItems(LIST *lp)
{
	assert(lp != NULL);
	struct node *temp;
	temp = lp->head->next;
	void **data2;
	data2 = malloc(lp->count * sizeof(void*));
	int i = 0;
	while (temp != lp->head)
	{
		data2[i] = temp->data;
		i++;
		temp = temp->next;
	}
	return data2;
}
