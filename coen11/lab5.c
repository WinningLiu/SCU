//Daren Liu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
	int number;
	char name[20];
  	struct Node *next; //pointer to the next list
};

struct Node *head = NULL;
struct Node *tail = NULL;

//prototypes
void insert(char guy[], int ppl);
void delete(int nums);
void show();
void search_size(int a);
int check_dup(char dude[]);

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
                        show();
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


void insert(char guy[], int ppl)
{
	struct Node *p;

	if (check_dup(guy) == 1) //checks for duplicates
	{
		printf("Sorry, we already have a person named %s\n", guy);
		return;
	}
	else	
	{
		if (head == NULL) //inserts if the list is empty
		{
			head = (struct Node*)malloc(sizeof(struct Node));
			head->number = ppl;
			strcpy(head->name, guy);
			tail = head;
			tail->next = NULL;
		}
		else //inserts if the list is not empty
		{
			p = (struct Node*)malloc(sizeof(struct Node));
			p->number = ppl;
			strcpy(p->name, guy);
			tail->next = p;
			tail = tail->next;
			p->next = NULL;
		}
	}
}

void delete(int nums)
{
	struct Node *ptr = head;
	struct Node *d;

	int a = 0;
	d = ptr;
	while (ptr != NULL) //traverses the list
	{
		if (ptr->number <= nums)
		{
			if (head == tail) //if there is only one in the list
			{
				head = NULL;
				tail = NULL;
				free(ptr);
				return;
			}
			else if (ptr == head) //if there is multiple stuff in the list, but the first one is the hed
			{
				head = ptr->next;
				free(ptr);
				return;
			}
			else if (ptr == tail) //if its the last one
			{
				d->next = NULL;
				free(ptr);
				tail = d;
				return;
			}
			else	//if its in the middle
			{
				d->next = ptr->next;
				free(ptr);
				return;
			}
			a = 1;
		}
		d = ptr;
		ptr = ptr->next;
	}

	if(a == 0) //prints if it tries to remove something that is not there
	{
		printf("Nothing was removed\n");
	}
}

void show()
{
	struct Node *s = head;

	while(s != NULL) //traverses the list and prints everything
	{
		printf("%s, %d\n", s->name, s->number);
		s = s->next;
	}
}

void search_size(int a)
{
	struct Node *u = head;
	
	while (u != NULL) //traverses the list and prints all that is less that a 
	{
		if (u->number <= a)
		{
			printf("%s, %d\n", u->name, u->number);
		}
		u = u->next;
	}
}

int check_dup(char dude[])
{
	struct Node *n = head;
	
	while (n != NULL) //traverses the list and returns 1 if there is a string that matches dude
	{
		if(strcmp(dude, n->name) == 0)
		{
			return 1;
		}
		n = n->next;
	}
	
	return 0;
}
