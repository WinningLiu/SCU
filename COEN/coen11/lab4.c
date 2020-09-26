#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int counter = 0;

typedef union data
{       
	char bdayboi[20];
	int anni;
	float averageage;
} EXTRA;
struct info
{
	int occasion;
	int size;
	char name[20];
	EXTRA extra;
};

struct info arr[10];

void insert(char s[], int n);
void delete(int d);
void show();
void check_size(int a);
int check_dup(char d[]);

int main ()
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
				if (check_dup(name) == 1) //checks duplicates in the array
				{
					printf("Sorry, we already have a person by the name of %s\n", name);
					break;
				}
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
                        check_size(compare);
                        break;

                        case 0:
                        return 0;
		}
	}
}
void insert(char guy[], int num)
{
	struct info *ptr = &arr[counter];
	int spec;
	
	if (counter != 10)
	{
		strcpy(ptr->name, guy); //copies name into struct
		ptr->size = num; //copies number into struct
		counter++;	
      			 while (1) // forces user to enter special occasion
    		  	 {       
         		        printf("What is the special occasion?\n");
         		        printf("1: Birthday\n");
               			printf("2: Anniversary\n");
               			printf("3: Average age\n");
                		scanf("%d", &spec); 
                        if (spec == 1) 
                        {       
                                printf("Name of the birthday person?\n");
                                scanf("%s", ptr->extra.bdayboi);
				ptr->occasion = 1;
                                break;
                        }
                        else if (spec == 2)
                        {       
                                printf("How long have you been married?\n");
                                scanf("%d", &ptr->extra.anni);
				ptr->occasion = 2;
                                break;
                        }
                        else if (spec == 3)
                        {       
                                printf("What is the average age of your group?\n");
                                scanf("%f", &ptr->extra.averageage);
				ptr->occasion = 3;
                                break;
                        }
                        else //forces user to re enter data if spec is not 1 2 or 3   
                        {       
                                printf("Invalid command, try again.\n");
                        }
       			 }

	}
	else if (counter == 10) //makes sure array stays at 10
	{
		printf("Sorry, we are all booked up.\n");
	}
}

void delete(int ppl)
{
	struct info *ptr = arr;
	int i;
	int j;
	for (i = 0; i < counter; i++)
	{
		if (ptr->size <= ppl)
		{
			for (j = 0; j < counter-1; j++)
			{
				strcpy(ptr->name, (ptr+1)->name); //replaces all the info below it with array at j
				ptr->size = (ptr+1)->size;
				ptr->occasion = (ptr+1)->occasion;
				ptr->extra.anni = (ptr+1)->extra.anni;
				strcpy(ptr->extra.bdayboi, (ptr+1)->extra.bdayboi);
				ptr->extra.averageage = (ptr+1)->extra.averageage;
				ptr++;
			}
		counter--;
		break;
		}
	}
}

void show()
{
	struct info *p = arr;
	int i;
	for (i = 0; i < counter; i++) //prints all the entries based on which occasion they have
	{
		if (p->occasion == 1)
		{
			printf("Name: %s, Number: %d, Occasion: %d, Birthday name: %s\n", p->name, p->size, p->occasion, p->extra.bdayboi);
		}
		else if (p->occasion == 2)
		{
			printf("Name: %s, Number: %d, Occasion: %d, Years together: %d\n", p->name, p->size, p->occasion, p->extra.anni);
		}
		else if (p->occasion == 3)
		{
			printf("Name: %s, Number: %d, Occasion: %d, Average age: %f\n", p->name, p->size, p->occasion, p->extra.averageage);
		}
		p++;
	}
}

void check_size(int ref)
{
	struct info *p = arr;
	int i;
	for (i = 0; i < counter; i++)
	{
		if (p->size <= ref) //prints all occasions if the size is below ref
		{	
			if (p->occasion == 1)
			{
				printf("Name: %s, Number: %d, Occasion: %d, Birthday name: %s\n", p->name, p->size, p->occasion, p->extra.bdayboi);
			}
			else if (p->occasion == 2)
			{
				printf("Name: %s, Number: %d, Occasion: %d Years together: %d\n", p->name, p->size, p->occasion, p->extra.anni);
			}
			else if (p->occasion == 3)
			{
				printf("Name: %s, Number: %d, Occasion: %d Average age: %f\n", p->name, p->size, p->occasion, p->extra.averageage);
			}
		}
		p++;
	}
}

int check_dup(char dude[])
{
	struct info *p = arr;
	int i;
	for (i = 0; i < counter; i++)
	{
		if (strcmp(p->name, dude) == 0) //if any of the names in the array is the same as dude, return 1
		{
			return 1;
		}
		p++;
	}
	return 0;
}
