#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node //linked list for one index
{       
        int number;
        char name[20];
        struct node *next;
};

struct list // linked list of nodes
{       
        struct node *head;
        struct node *tail;
};

struct list arr[4]; // array of linked lists

//prototypes
void insert();
void delete();
void show();
void search_size();
int check_dup();
void every();
void read_file();
void save_file();

int main(int argc, char *argv[])
{       
	int i;
        for (i = 0; i < 4; i++)
        {
                arr[i].head = NULL;
                arr[i].tail = NULL;
        }
	if (argc == 1) // if there is no file
	{
		printf("The name of the file is missing\n");
		return 1;
	}
	else 
	{
		read_file(argv[1]);
	}
        int answer = 0;
        int number = 0;
        char name[20];
        int avail;
        int compare;
	int k;
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
			save_file(argv[1]); //saves the file before it deletes everything
                       	every();
                        return 0;
                }
        }
}

void insert(char dude[], int num)
{
        struct node *p;
        int cat = 0;
        if (check_dup(dude) == 1) //checks for any duplicates
        {
                printf("Sorry, there already is a person named %s\n", dude);
		return;
        }
        //determines the index to insert in
        if (num == 1 || num == 2)
                cat = 0;
        else if (num == 3 || num == 4)
                cat = 1;
        else if (num == 5 || num == 6)
                cat = 2;
        else{
                cat = 3;
        }

        if (arr[cat].head == NULL) //if there is nothing in the index
        {
                arr[cat].head = (struct node*)malloc(sizeof(struct node));
                p = arr[cat].head;
                p->number = num;
                strcpy(p->name, dude);
                arr[cat].tail = arr[cat].head;
                arr[cat].tail->next = NULL;
        }
        else { //if there is something at the index, inserts after the tail
                p = (struct node*)malloc(sizeof(struct node));
                arr[cat].tail->next = p;
                p->number = num;
                strcpy(p->name, dude);
                arr[cat].tail = p;
                p->next = NULL;
        }
}

void delete(int ava)
{
	struct node *ptr;
        struct node *t; //trailing pointer          
        int cat;

        for (cat = 0; cat < 4; cat++)
        {
        ptr = arr[cat].head;
        t = ptr;
        while (ptr != NULL)
        {
                if (ptr->number <= ava)
                {
                        if (arr[cat].head == arr[cat].tail) //deletes the only node in the linked list
                        {
                                arr[cat].head = NULL;
                                arr[cat].tail = NULL;
                                free(ptr);
                        }
                        else if (ptr == arr[cat].tail) //deletes the last node
                        {
                                t->next = NULL;
                                free(ptr);
                                arr[cat].tail = t;
                        }
                        else if (ptr == arr[cat].head) // deletes the first node
                        {
                                arr[cat].head = ptr->next;
                                free(ptr);
                        }
                        else //deletes in the middle
                        {
                                t->next = ptr->next;
                                free(ptr);
                        }
                        return;
                }
                t = ptr;
                ptr = ptr->next;
        }

        }
        printf("Nothing was removed\n"); //prints if nothing was removed
}

void show()
{
        //traverses through each index of the array and prints everything out
        struct node *ptr = arr[0].head;
        printf("Seats of 1 or 2\n");
        while(ptr != NULL)
        {
                printf("%s, %d\n", ptr->name, ptr->number);
                ptr = ptr->next;
        }

        ptr = arr[1].head;
        printf("Seats of 3 or 4\n");
        while(ptr != NULL)
        {
                printf("%s, %d\n", ptr->name, ptr->number);
                ptr = ptr->next;
        }

        ptr = arr[2].head;
        printf("Seats of 5 or 6\n");
        while(ptr != NULL)
        {
                printf("%s, %d\n", ptr->name, ptr->number);
                ptr = ptr->next;
        }

        ptr = arr[3].head;
        printf("Seats of 6 or more\n");
        while(ptr != NULL)
        {
                printf("%s, %d\n", ptr->name, ptr->number);
                ptr = ptr->next;
        }
}

void search_size(int comp)
{
        //traverses each index of the array and prints anything that is less than or equal to comp
        struct node *ptr;
        ptr = arr[0].head;
        printf("Sizes of 1 or 2\n");
        while(ptr != NULL)
        {
                if (ptr->number <= comp)
                        printf("%s, %d\n", ptr->name, ptr->number);
                ptr = ptr->next;
        }

        ptr = arr[1].head;
        printf("Sizes of 3 or 4\n");
        while(ptr != NULL)
        {
                if (ptr->number <= comp)
                        printf("%s, %d\n", ptr->name, ptr->number);
                ptr = ptr->next;
        }

        ptr = arr[2].head;
        printf("Sizes of 5 or 6\n");
        while(ptr != NULL)
        {
                if (ptr->number <= comp)
                        printf("%s, %d\n", ptr->name, ptr->number);
                ptr = ptr->next;
        }

        ptr = arr[3].head;
        printf("Sizes of 6 or more\n");
        while(ptr != NULL)
        {
                if (ptr->number <= comp)
                        printf("%s, %d\n", ptr->name, ptr->number);
                ptr = ptr->next;
        }

}

int check_dup(char guy[])
{
	//traverses through each index and searches for the guys name
        struct node *ptr;
        int cat;
        for (cat = 0; cat < 4; cat++)
        {
                ptr = arr[cat].head;
                        while(ptr != NULL)
                         {
                                if (strcmp(ptr->name, guy) == 0)
                                         return 1; //if the guys name is the same, return 1
                           ptr = ptr->next;
                         }
        }
                      
        return 0; //if there is no one with the same name, return 0
}               
        
void every()
{       
        struct node *p;
        struct node *temp;
        int cat;
        for (cat = 0; cat < 4; cat++)
        {
        	p = arr[cat].head;
        	while(p != NULL) //traverses each index and frees each node of the linked list
        	{
                	temp = p;
                	p = p->next;
                	free(temp);
        	}
        }

}

void read_file(char *name)
{
	FILE *fp;
	fp = fopen(name, "r");
	int num;
	char str[20];
	if (fp == NULL)
	{
		printf("Error, no file\n");
		return;
	}
	fseek(fp, 32, SEEK_SET); // ignores the headers
	while(fscanf(fp, "%s %d\n", str, &num) == 2) // if it scans a string and an int, insert into a linked list
	{
		insert(str, num);
	}
	fclose(fp);
}

void save_file(char *name)
{
	int i;
	FILE *fp;
	struct node *ptr;
	fp = fopen(name, "w"); //opens the file
	if (fp == NULL)
	{
		printf("Error, no file\n");
		return;
	}
	fprintf(fp, "Name\tGroup Size\n"); // the header
	fprintf(fp, "----------------\n");
	for (i = 0; i < 4; i++) // prints every linked liist in the array to the file
	{
		ptr = arr[i].head;
		while (ptr != NULL)
		{
			fprintf(fp, "%s %d\n", ptr->name, ptr->number);
			ptr = ptr->next;
		}
	}
	fclose(fp); //closes the file
	return;
}
