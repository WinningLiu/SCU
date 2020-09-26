#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int compare(int input, int correct)
{
	if (input == correct)
	{
		return 1;
	}
	else {
		return 0;
	}
}

int main (void)
{
int corrects = 0;
srand((int)time(NULL));
for (int i = 0; i < 10; i++)
{
        int top = rand()%13;
        int bottom = rand()%12+1;
        int both =  top * bottom;
        int answer = 0;

        printf("%d / %d\n", both, bottom);
        scanf("%d", &answer);

	if (compare(answer, top) == 1)
	{
		printf("Correct!\n");
		corrects++;
	}
	else if (compare(answer, top) == 0)
	{
		printf("Incorrect\n");
	}
}

	printf("You got %d correct!\n", corrects); 
}        
