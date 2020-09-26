#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int counter = 0;

int check_duplicate(char s[]); //state the function before its called in insert

struct entry
{
        char names[20];
        int size;
};

struct entry reserve[10];

void insert(char guy[], int amount) 
{
	if (check_duplicate(guy) == 1) // checks if the name inserted is a duplicate
	{
		printf("Sorry but we already have a person named %s\n", guy);
	}
	else if (counter != 10) // copies the name and number from scanf to the array
	{
	 	struct entry *ptr = &reserve[counter];
		strcpy(ptr->names, guy);
		ptr->size = amount;
		counter++;
	}
	else if (counter == 10) // makes sure that the array doesn't go above the index
	{
		printf("Sorry but we are all booked up.\n");
	}
}

void delete(int people) //needs to use struct pointer
{
	struct entry *q = reserve;
	int i;
	int j;
	for (i = 0; i < counter; i++)
	{
		if (q->size <= people) //everytime the size is less than or equal to the number typed
		{
			for (j = i; j < counter-1; j++)
			{
				strcpy(q->names, (q+1)->names); // copies the string from the index above to the index at q
				q->size = (q+1)->size; // sets the number equal to the number from the index +1
				q++;
			}
			counter--;
			break; // breaks the loop
		}
		q++;
	}
}

void list()
{
	int i;
	struct entry *w = reserve;
	for (i = 0; i < counter; i++)
	{
		printf("%s, %d\n", w->names, w->size);//prints every size and name from each struc from the array
		w++;
	}
}

void search_size(int input)
{
	int i;
	struct entry *p = reserve;

	for (i = 0; i < counter; i++)
	{
		if (p->size <= input) //prints the name and size every time the size is less than or equal to the input
		{
			printf("%s, %d\n", p->names, p->size);
		}
		p++;
	}
}

int check_duplicate(char dup[])
{
	int i; 
	struct entry *d = reserve;
	for (i = 0; i < counter; i++)
	{
		if (strcmp(d->names, dup) == 0) //if the string dup is the same as any string in the array, it returns 1
		{
			return 1;
			break;
		}
		d++;
	}
	return 0;
}

int main()
{
	int answer = 0;
	int number = 0;
	char name[20];
	int avail; 
	int compare;
	while(1)
	{
		printf("What would you like to do?\n");
		printf("1: Insert\n");
		printf("2: Remove\n");
		printf("3: List\n");
		printf("4: Sizes less than a number\n");
		printf("0: Quit\n");
		scanf("%d", &answer);
		switch (answer)
		{
			case 1:
			printf("Name?\n");
			scanf("%s", name);
			printf("How many people?\n");
			scanf("%d", &number);
			insert(name, number);
			break;
	
			case 2:
			printf("I have a table available for ");
			scanf("%d", &avail);
			delete(avail);
			break;

			case 3:
			list();
			break;

			case 4:
			printf("Enter a number: ");
			scanf("%d", &compare);
			search_size(compare);
			break;

			case 0:
			return 0;
		}
	}
}
