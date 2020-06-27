#include "global.h"

void insert(char dude[], int num)
{
        pthread_mutex_lock(&lock);
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
        pthread_mutex_unlock(&lock);
        return;
}

void delete(int ava)
{       
        struct node *ptr;
        struct node *t; //trailing pointer
        int cat;
        pthread_mutex_lock(&lock);
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
                        pthread_mutex_unlock(&lock);
                        return;
                } 
                t = ptr;
                ptr = ptr->next;
        }
        
        }
        printf("Nothing was removed\n"); //prints if nothing was removed
        pthread_mutex_unlock(&lock);
        return;
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
        printf("Seats of 7 or more\n");
        while(ptr != NULL)
        {       
                printf("%s, %d\n", ptr->name, ptr->number);
                ptr = ptr->next;
        }
        return;
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
        printf("Sizes of 7 or more\n");
        while(ptr != NULL)
        {       
                if (ptr->number <= comp)
                        printf("%s, %d\n", ptr->name, ptr->number);
                ptr = ptr->next;
        }
        return;
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

void every(struct node *w)
{
        if (w == NULL)
                return;

        else {
                every(w->next);
                free(w);
        }
}

void back_show(struct node *ptr)
{
        if (ptr == NULL) //base case
                return;

        back_show(ptr->next); //calls itself so that it goes to the last node
        printf("%s, %d\n", ptr->name, ptr->number); //prints the last node first and then prints the rest afterwards
}

void back_name(struct node *p)
{
        int i;
        if (p == NULL) //base case
                return;

        for (i = strlen(p->name) - 1; i >= 0; i--) //starts from the last character in the string so that it prints it backwards
        {
                printf("%c", p->name[i]);
        }
        printf("\n");

        back_name(p->next); //calls on itself after the for loop so that the lists stay in order
}
