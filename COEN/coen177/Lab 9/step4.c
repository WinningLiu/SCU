#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	char buffer[atoi(argv[2])]; // Accept buffer size
	FILE *fp, *ofp;
	fp = fopen(argv[1], "rb"); // Accept file input
	ofp = fopen("Output/output.txt", "w");
	// Write file data to output file
	while (fread(buffer, sizeof(buffer), 1, fp)){
		fwrite(buffer, 1, sizeof(buffer), ofp);
	}
	fclose(ofp);
	fclose(fp);
	return 0;
}
