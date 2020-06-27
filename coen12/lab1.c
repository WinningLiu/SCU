#include <stdio.h>
#include <stdlib.h>

#define MAX_WORD_LENGTH 30 //assume max word length 30 characters

int word_count();

int main(int argc, char *argv[])
{
        if (argc < 2) //makes sure that there are two files 
        {
                printf("Error file missing\n");
                return 0;
        }
        printf("%d total words\n", word_count(argv[1])); //prints out the value that is returned by word count
}

int word_count(char name[])
{
        int count = 0;
        char buffer[MAX_WORD_LENGTH]; //a char buffer with a max of 30 characters
        FILE *fp = fopen(name, "r"); //opens the file for reading
        if (fp == NULL) //makes sure that there is a file to open
        {
                printf("Error file missing\n");
                return 0;
        }
        while(fscanf(fp, "%s", buffer) > 0) //scans the file for strings and puts them in the buffer
        {
                count++; //increases the count for each string that is scanned
        }
	fclose(fp);

        return count; //returns the number of strings or words
}

