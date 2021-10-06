

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct { 
    int pageno;
	int timestamp; // Used only in LRU
	bool bit; // Used only in Second Chance
} ref_page;

// Returns index of page_num in cache if it exists, -1 otherwise
int pageTableLookup(int page_num, int cache_size, ref_page cache[]){
	int i;
	for (i = 0; i < cache_size; i++){
		if (cache[i].pageno == page_num)
			return i;
	}
	return -1;
}

int main(int argc, char *argv[]){
	int C_SIZE = atoi(argv[1]); // Size of Cache passed by user 
	int algorithmType = atoi(argv[2]); // 1 = FIFO, 2 = LRU, 3 = Second Chance
    ref_page cache[C_SIZE]; // Cache that stores pages 
    char pageCache[100]; // Cache that holds the input from test file

    int i;
    int totalFaults = 0; // keeps track of the total page faults
	int curIndex = 0; // keeps track of position in queue
	int numRequests = 0; // keeps track of number of requests
   
    for (i = 0; i < C_SIZE; i++){//initialise cache array  
         cache[i].pageno = -1; // -1 means empty spot in cache
		 cache[i].timestamp = -1;
		 cache[i].bit = false;
    }

    while (fgets(pageCache, 100, stdin)){
		// Skip non-digit characters
		if (pageCache[0] < '0' || pageCache[0] > '9') continue;
    	int page_num = atoi(pageCache); // Stores number read from file as an int
		numRequests++;
		int tempIndex = pageTableLookup(page_num, C_SIZE, cache);
		if (algorithmType == 1){ // FIFO
			// If page_num does not exist in cache, insert at curIndex
			// If a page already exists at curIndex, new page simply overwrites it
			if (tempIndex < 0){
				cache[curIndex].pageno = page_num;
				totalFaults++;
				curIndex = (curIndex + 1) % C_SIZE;
			}
		} else if (algorithmType == 2){ // LRU
			// If page_num exists in cache, update its timestamp
			if (tempIndex >= 0){
				cache[tempIndex].timestamp = numRequests;
			} else {
				// If cache is full, find oldest page to throw out
				if (cache[curIndex].pageno >= 0){
					int oldestIndex = 0;
					int j;
					for (j = 1; j < C_SIZE; j++){
						if (cache[j].timestamp < cache[oldestIndex].timestamp)
							oldestIndex = j;
					}
					curIndex = oldestIndex;
				}
				// Add or insert page_num with current timestamp
				cache[curIndex].pageno = page_num;
				cache[curIndex].timestamp = numRequests;
				totalFaults++;
				curIndex = (curIndex + 1) % C_SIZE;
			}
		} else { // Second Chance
			// If page_num exists in cache, set its bit to 1
			if (tempIndex >= 0){
				cache[tempIndex].bit = true;
			} else {
				// If cache is full, find first page from curIndex whose bit is 0
				// Also flips bits of all pages it traverses from 1 to 0
				if (cache[curIndex].pageno >= 0){
					while (cache[curIndex].bit){
						cache[curIndex].bit = false;
						curIndex = (curIndex + 1) % C_SIZE;
					}
				}
				// Add or insert page_num with bit 0
				cache[curIndex].pageno = page_num;
				cache[curIndex].bit = false;
				totalFaults++;
				curIndex = (curIndex + 1) % C_SIZE;
			}
		}
    }

    printf("Total Page Faults: %d\n", totalFaults);
	printf("Number of Requests: %d\n", numRequests);
	printf("Hit Rate: %f\n", 1 - ((float) totalFaults / numRequests));
    return 0;
}