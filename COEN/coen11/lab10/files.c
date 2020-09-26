#include "global.h"

void read_file(char *name, int key)
{
	//fgets
        FILE *fp;
        fp = fopen(name, "r");
        int num;
        char str[20], *p;
	char line[100];
        if (fp == NULL)
        {
                printf("Error, no file\n");
                return;
        }
        //fseek(fp, 32, SEEK_SET); // ignores the headers
        while(fgets(line, 100, fp) != NULL) // if it scans a string and an int, insert into a linked list
        {
		p = line;
		while (*p != '\0') //traverse through each character
		{
			*p ^= key; //decrypts with key
			p++;
		}
		sscanf(line, "%s %d", str, &num);
                insert(str, num);
        }
        fclose(fp);
}

void save_file(char *name, int key)
{
        int i;
        FILE *fp;
        struct node *ptr;
	char line[100], *p;
        fp = fopen(name, "w"); //opens the file
        if (fp == NULL)
        {
                printf("Error, no file\n");
                return;
        }
        //fprintf(fp, "Name\tGroup Size\n"); // the header
        //fprintf(fp, "----------------\n");
        for (i = 0; i < 4; i++) // prints every linked liist in the array to the file
        {
                ptr = arr[i].head;
                while (ptr != NULL)
                {
			sprintf(line, "%s %d", ptr->name, ptr->number); //stores name and number in line
                        p = line;
			while (*p != '\0') //traverses through each character
			{
				*p ^= key; //encrypts with key
				p++;
			}
			fprintf(fp, "%s\n", line);
                        ptr = ptr->next;
                }
        }
        fclose(fp); //closes the file
        return;
}
void *autosave(void *arg)
{
        int i;
        struct node *ptr;
        while(1)
        {
        pthread_mutex_lock(&lock);
        FILE *fp = fopen((char*)arg, "wb"); //writes binary into the file
        if (fp == NULL)
        {
                printf("ERROR\n");
                return NULL;
        }
        printf("Autosaving...\n");
        for (i = 0; i < 4; i++)
        {
                ptr = arr[i].head;
                while (ptr != NULL)
                {
                        fwrite(ptr, sizeof(struct node), 1, fp); //writes every node into a binary file
                        ptr = ptr->next;
                }
        }
        fclose(fp);
        printf("Finished autosaving\n");
        pthread_mutex_unlock(&lock);
        sleep(15); //waits for 15 seconds
        }
	return NULL;
}

void read_autosave(void *arg)
{
        pthread_mutex_lock(&lock);
        int i;
        struct node ptr;
        FILE *fp = fopen((char*)arg, "rb");
        if (fp == NULL)
        {
                printf("ERROR\n");
                return;
        }
        while (fread(&ptr, sizeof(struct node), 1, fp) == 1) //reads a node from the binary file and stores it in ptr
        {
                printf("%s %d\n", ptr.name, ptr.number);
        }
        pthread_mutex_unlock(&lock);
        fclose(fp);
        return;
}

