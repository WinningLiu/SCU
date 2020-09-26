#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <assert.h>
#include "pqueue.h"
#define p(x) ((x-1)/2)
#define l(x) ((x*2)+1)
#define r(x) ((x*2)+2)

typedef struct pqueue
{
	int count;
	int length;
	void **data;
	int (*compare)();
} PQ;

PQ *createQueue(int (*compare)())
{
    	PQ *pq = malloc(sizeof(PQ));
    	assert(pq != NULL);//makes sure pq is malloced correctly
	pq->count = 0;
	pq->length = 10;//initial length of the array
	pq->data = malloc(sizeof(void*)*10);
    	pq->compare = compare;
    	assert(compare != NULL);
	return pq;
	//O(1)
}

void destroyQueue(PQ *pq)
{
	int i;
    	assert(pq != NULL);
    	for (i = 0; i < pq->count; i++)//frees each value in data 
		free(pq->data[i]);
    	free(pq->data);//frees the aray
    	free(pq);
    	return;
	//O(n)
}

int numEntries(PQ *pq)
{
    assert(pq != NULL);
    return pq->count;//returns number of entries in the priority queue
	//O(1)
}

void addEntry(PQ *pq, void *entry)
{
    	assert(pq != NULL && entry != NULL);
	assert(pq->count < pq->length);
    	if (pq->count == pq->length)//if array is full, double array size
    	{
        	pq->data = realloc(pq->data, sizeof(void*)*pq->length*2);
		assert(pq->data != NULL);
		pq->length = pq->length*2;
        
    	}
	pq->data[pq->count] = entry;
	int index = pq->count;//keeps track of root index
    	while (pq->compare(pq->data[index], pq->data[p(index)]) < 0)
	{
		void *temp = pq->data[p(index)];
		pq->data[p(index)] = pq->data[index];
		pq->data[index] =  temp;
		index = p(index);
	}
	pq->count++;
    	//O(logn)
}

void *removeEntry(PQ *pq)
{
   	assert(pq != NULL);
	assert(pq->count > 0);
	void *min = pq->data[0];//value to be deleted
	pq->data[0] = pq->data[pq->count-1]; //swaos value to be deleted with last value
    	pq->count--;
	int x = 0;//root index
	void *root = pq->data[x];//root value
	int n; //temp index
	while(l(x) < pq->count)
	{
		n = l(x);
		if (r(x) < pq->count && (*pq->compare)(pq->data[r(x)], pq->data[l(x)]) < 0)
			n = r(x);

	if ((*pq->compare)(pq->data[n], root) < 0) //if child is less than root, swap root with child
	{
		pq->data[x] = pq->data[n];
		pq->data[n] = root;
		x = n;
	}
	else
		break;

	}
	return min;
    	//O(logn)
}
