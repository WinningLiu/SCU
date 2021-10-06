#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	char buffer[atoi(argv[2])]; // Accept buffer size
	FILE *fp;
	fp = fopen(argv[1], "rb"); // Accept file input
	// Read file data
	while (fread(buffer, sizeof(buffer), 1, fp));
	fclose(fp);
	return 0;
}