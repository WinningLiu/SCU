#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"

struct node
{
	void *data; //generic data
	struct node *next; //pointer to next and prev nodes
	struct node *prev;
};

typedef struct list
{
	int count; // number of nodes in the list
	struct node * head; // pointer to the head of the list
	int (*compare)(); //function compare
} LIST;

LIST *createList(int (*compare)())
{
	LIST *lp = malloc(sizeof(LIST));
	assert(lp != NULL); // makes sure lp was malloced correctly
	lp->count = 0;
	lp->head = malloc(sizeof(struct node));
	lp->head->next = lp->head; //head points to itself, a dummy node
	lp->head->prev = lp->head;
	lp->compare = compare;
	return lp;
	//O(1)
}

void destroyList(LIST *lp)
{
	assert(lp != NULL);
	struct node *pDel, *pNext;
	pDel = lp->head;//set pDel to the head
	
	do {
		pNext = pDel->next;//set pNext equal to pDel
		free(pDel);//free pDel
		pDel = pNext;//moves on to the next node
	} while (pDel != lp->head);

	free(lp);
	//O(n)
}

int numItems(LIST *lp)
{
	assert(lp != NULL);
	return lp->count; // returns the amount of nodes in the list
	//O(1)
}

void addFirst(LIST *lp, void *item)
{
	struct node *Cur; // curent node
	struct node *dum; //dummy node
	assert(lp != NULL && item != NULL); //makes sure that lp and item exists
	Cur = malloc(sizeof(struct node));
	Cur->data = item;
	dum = lp->head; // sets dum to head

	Cur->prev = dum; //the previous node to cur is the head
	Cur->next = dum->next; //
	dum->next->prev = Cur;
	dum->next = Cur;
	lp->count++;
	//O(1)
}

void addLast(LIST *lp, void *item)
{
	assert(lp != NULL);
	struct node *Cur;//current node
	struct node *dum; //dummy node
	Cur = malloc(sizeof(struct node));
	Cur->data = item;
	dum = lp->head;//sets the dummy node to the head
	
	Cur->next = dum; //the next Cur node equal to the dummy node
	Cur->prev = dum->prev;//sets the prev pointer to what dum was pointing before
	dum->prev->next = Cur;//dum prev next equal to Cur
	dum->prev = Cur;//dum prev points to Cur
	lp->count++;//increases count
	//O(1)
}

void *removeFirst(LIST *lp)
{
	assert(lp != NULL);//makes sure lp exists
	if (lp->count == 0)//if the list is empty return NULL
		return NULL;

	struct node *temp;//temp node
	struct node *dum;//dummy node
	void *store;
	dum = lp->head;
	temp = dum->next;
	store = temp->data; //stores temp->data into store
	
	dum->next = temp->next;//head next's is equal to temp next
	temp->next->prev = dum; // temp next prev is equal to dum
	free(temp); // free temp
	lp->count--;//decreases count
	return store;//returns the data in store
	//O(1)
}

void *removeLast(LIST *lp)
{
	assert(lp != NULL);
	if (lp->count == 0)//makes sure that the linked list is not empty
		return NULL;
	struct node *temp;
	struct node *dum;
	void *store;
	dum = lp->head;
	temp = dum->prev;
	store = temp->data;
	
	temp->prev->next = dum;//temp prev next equal to the dummy node
	dum->prev = temp->prev;//dummy previous is equal to temp prev
	free(temp);//free temp
	lp->count--;//decreases count
	return store;//returns the data stored in temp
	//O(1)
}

void *getFirst(LIST *lp)
{
	assert(lp != NULL);
	return lp->head->next->data;//returns the first node's data
	//O(1)
}

void *getLast(LIST *lp)
{
	assert(lp != NULL);
	return lp->head->prev->data;//returns the last node's data
	//O(1)
}

void removeItem(LIST *lp, void *item)
{
	assert(lp != NULL && item != NULL);//makes sure that lp and item are not empty
	struct node *temp;
	temp = lp->head->next; //sets temp to the first node
	while (temp != lp->head) //traverses the linked list
	{
		if ((*lp->compare)(temp->data, item) == 0) //if the data in the node is the same as the item, free the node
		{
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			free(temp);
			return;
		}	
		temp = temp->next;
	}
	//O(n)
}

void *findItem(LIST *lp, void *item)
{
	assert(lp != NULL && item != NULL);//makes sure that lp and item are not empty
	struct node *temp;
	temp = lp->head->next;
	while (temp != lp->head) //traverses the linked list
	{
		if((*lp->compare)(temp->data,item) == 0) //if the data in the node is equal to the item return the data
			return temp->data;
		temp = temp->next;
	}
	return NULL;
	//O(n)
}

void *getItems(LIST *lp)
{
	assert(lp != NULL);//makes sure that lp is not null
	struct node *temp;
	temp = lp->head->next;
	void **data2;
	data2 = malloc(lp->count * sizeof(void*));
	int i = 0;
	while (temp != lp->head)//traverses the linked list
	{
		data2[i] = temp->data;//sets the data2 array to the temp data
		i++;
		temp = temp->next;
	}
	return data2;
	//O(n)
}
