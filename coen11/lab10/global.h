#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

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


extern struct list arr[4]; // array of linked lists
extern pthread_mutex_t lock;

void insert(char dude[], int num);
void delete(int ava);
void show();
void search_size(int comp);
int check_dup(char guy[]);
void every(struct node *w);
void read_file(char *name, int key);
void save_file(char *name, int key);
void back_show(struct node *ptr);
void back_name(struct node *p);
void *autosave(void *arg);
void read_autosave(void *arg);
