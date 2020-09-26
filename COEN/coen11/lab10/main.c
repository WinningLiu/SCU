#include "global.h"

struct list arr[4];
pthread_mutex_t lock;

int main(int argc, char *argv[])
{
	int key = atoi(argv[3]); //converts the key into an integer 
        int i;
        for (i = 0; i < 4; i++)
        {
                arr[i].head = NULL;
                arr[i].tail = NULL;
        }
        if (argc < 3) // if there is no file
        {
                printf("ERROR\n");
                return 1;
        }
        else
        {
                read_file(argv[1], key);
        }

        int answer = 0;
        int number = 0;
        char name[20];
        int avail;
        int compare;
        int k;
        pthread_t thread;

        pthread_mutex_init(&lock, NULL); //initializes the lock
        pthread_create(&thread, NULL, autosave, (void*) argv[2]); //creates a new thread in autosave

        while(1)
        {
                printf("What would you like to do?\n");
                printf("1: Insert\n");
                printf("2: Remove\n");
                printf("3: List\n");
                printf("4: Sizes less than a number\n");
                printf("5: Show each list backwards\n");
                printf("6: Show each name backwards\n");
                printf("7: Print binary file\n");
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

                        case 5:
                        for (i = 0; i < 4; i++)
                        {
                                if (i == 0)
                                        printf("Sizes 1 or 2\n");
                                if (i == 1)
                                       printf("Sizes 3 or 4\n");
                                if (i == 2)
                                        printf("Sizes 5 or 6\n");
                                if (i == 3)
                                        printf("Sizes 7 or more\n");
                                struct node *ptr = arr[i].head; //sets the pointer to the head of the index of the array
                                back_show(ptr);
                        }
                        break;

                        case 6:
                        for (i = 0; i < 4; i++)
                        {
                                struct node *p = arr[i].head; // sets the pointer to the head of the index of the array
                                if (i == 0) // prints each thing depending on e
                                        printf("Sizes 1 or 2\n");
                                if (i == 1)
                                        printf("Sizes 3 or 4\n");
                                if (i == 2)
                                        printf("Sizes 5 or 6\n");
                                if (i == 3)
                                        printf("Sizes 7 or more\n");
                                back_name(p);
                        }
                        break;

                        case 7:
                        read_autosave(argv[2]);
                        break;

                        case 0:
                        pthread_mutex_lock(&lock);
                        pthread_cancel(thread);
                        pthread_mutex_unlock(&lock);
                        save_file(argv[1], key); //saves the file before it deletes everything
                        int j;
                        for (j = 0; j < 4; j++)
                        {
                                struct node *w = arr[j].head;
                                every(w);
                        }
                        return 0;
                }
        }
}

