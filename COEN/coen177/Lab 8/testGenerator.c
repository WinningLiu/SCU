#include <stdio.h>

int main(int argc, char *argv[]){
	FILE *fp;
	char buffer[sizeof(int)];
	int i;
	fp = fopen("testInput.txt", "w");
	for (i = 0; i < 10; i++){
		sprintf(buffer, "%d\n", rand() % 8);
		fputs(buffer, fp);
	}
	fclose(fp);
	return 0;
}
