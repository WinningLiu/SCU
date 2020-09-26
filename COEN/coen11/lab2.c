#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char names[10][20];
int sizes[10];
int counter;

void insert(char name[], int people)
{
	if (counter >= 10)
	{
		printf("Sorry but we are all booked\n");
		return;
	}
	strcpy(names[counter],name);
	sizes[counter] = people;
	counter++;
}


void size(int seat)
{       
	int i;
	int j;
	for (i = 0; i < counter; i++)
	{
		if (sizes[i] <= seat)
		{
			for (j = i; j < counter; j++)
			{
				strcpy(names[j], names[j+1]);
				sizes[j] = sizes[j+1];
			}
			names[counter][0] = '\0';
			counter--;
			break;
		}
	}
	
}

void list() 
{       
        int i = 0;
	
        while (names[i][0] != '\0')
        {  
                printf("%s, %d\n", names[i], sizes[i]);
                i++;
        }
	
}

int main() 
{
int answer = 4;
int number;
int seats;
while (1) 
{
        printf("What would you like to do?\n");
        printf("1: Insert\n");
        printf("2: Remove\n");
        printf("3: List\n");
        printf("0: Quit\n");
        scanf("%d", &answer);

        switch (answer)
        {
                case 1:
                {
                char guy[21];
                printf("Name?\n");
                scanf("%s", guy);
                printf("How many people?\n");
                scanf("%d", &number);
                insert(guy, number);
                break;
                }
                case 2:
		printf("I have a seat available for ");
		scanf("%d", &seats);
                size(seats);
                break;

                case 3:
                list();
                break;

                case 0:
                return 0;

        }
}
}
