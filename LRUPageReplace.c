/*
 * Name: Patrick Brennan
 * Date: June 1st, 2020
 * 
 * Assignment: Write a program to preform Least recently used (LRU) page replacement
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX 100

struct pageStruct {
	int size;			//size of cache
	int p [MAX];			//array of all pages
	int value [MAX];		//array of values in the cache
	int count [MAX];		//array of counts that represent when the page was most recently used
};


void lru( struct pageStruct cache ){
	int q;
	for (q=0; q<cache.size; q++){							//initialize all counts and values to 0
		cache.count[q]= 0;
		cache.value[q]= 0;
	}
	int counter=0;
	int j;
	for (j=0; j< 100; j++){								//loop through all pages
		counter++;								//counter keeps track of most recent page reference
		int page_num= cache.p[j];
		int temp=0;	
		int i;
		for (i=0; i<=cache.size; i++){						//loop through cache
			if (cache.value[i]== page_num){					//if page is in cache
				temp=1;							//will skip to next page when temp = 1
				cache.count[i]= counter;				//set count= j, updating it to most recently used, skip to next page
			}
		}
		if (temp==0){								//if page is not in cache...
			printf("Page %d caused a page fault.\n", page_num);		//...page fault
			if (j<cache.size){						//if cache is not yet full...
				cache.value[j]= page_num;				//...store page in next spot in cache
				cache.count[j]= counter;
			}
			else{								//if cache is full...
				int c;
				int location=0;
				int minimum = cache.count[0];
				for (c=0; c<cache.size; c++){				//...find page with minimum count, which would indicate least recently used
					if (cache.count[c]< minimum){
						minimum= cache.count[c];
						location=c;
					}
				}
				cache.value[c]= page_num;				//replace least recently used page with new page
				cache.count[c]= counter;				//update count as most recently used
			}
		}
	}	
}

int main (int argc, char *argv[]){
	int size= atoi(argv[1]);
	char pages[MAX];
		struct pageStruct cache;
	cache.size= size;
	int page=0;
	int count=0;
	FILE *fp; 
	fp = fopen("Lab8/testinput.txt", "r"); 
	if (fp == NULL){
		perror("Error opening file"); 
		return(-1); 
	}
	else {
		printf("file open worked"); 
	}
	while (fgets(pages, MAX, fp)){		
		page = atoi (pages);
		cache.p[count]= page;
		count++;
	}
	printf("count is %d", count); 
	lru(cache);
	return 0;
}




