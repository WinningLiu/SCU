#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "pack.h"
#include "pqueue.h"

typedef struct node NODE;

/*NODE *createNode (int count)
{
	NODE *np = malloc(sizeof(NODE));
	assert(np != NULL);
	np->count = count;
	np->parent = parent;
}*/

int hopNumber(NODE *leaf)
{
	int hops = 0;
	while(leaf->parent != NULL)
	{
		leaf = leaf->parent;
		hops++;
	}
	return hops;
}


static int compare(NODE *i, NODE *j)
{
	return (i->count < j->count) ? -1 : (i->count > j->count);
}

int main (int argc, char **argv)
{
	int arr[257] = {0};
	FILE *fp = fopen(argv[1], "r");
	
	if (fp == NULL)
	{
		printf("File does not exist\n");
		return 0;
	}
	if (argc < 3)
	{
		printf("Insufficient arguments\n");
		return 0;
	}

	//step 1
	int a;
	while (fscanf(fp, "%c", &a) == 1)
	{
		arr[a] += 1;
	}
	fclose(fp);

	//step 2
	NODE *leaves[257] = {NULL};
	PQ *pq = createQueue(compare);
	int i;
	for (i =0; i < 257; i++)
	{
		if (arr[i] > 0)
		{
			NODE *temp = malloc(sizeof(NODE));
			temp->count = arr[i];
			temp->parent = NULL;
			addEntry(pq, temp);
			leaves[i] = temp;
		}
	}
	NODE *zero = malloc(sizeof(NODE));
	zero->count = 0; 
	zero->parent = NULL;
	addEntry(pq, zero);
	leaves[253] = zero;

	//step 3
	while (numEntries(pq) > 1)
	{
		NODE *low1 = removeEntry(pq);
		NODE *low2 = removeEntry(pq);
		NODE *low3 = malloc(sizeof(NODE));
		low3->count = low1->count + low2->count;
		low3->parent = NULL;
		low1->parent = low3;
		low2->parent = low3;
		addEntry(pq, low3);
	}

	//step 4
	int j;
	for (j = 0; j < 257; j++)
	{
		if (leaves[j] == NULL)
		{
			int numhop;
			numhop = hopNumber(leaves[j]);
				if (isprint(j))
					printf("%c: %d x %d bits = %dn", j, arr[j], numhop, arr[j] * numhop);
				else 
    					printf("%03o: %d x %d bits = %d\n", j, arr[j], numhop, arr[j]*numhop);
		}
	}

	//step 5
	pack(argv[1], argv[2], leaves);
}
