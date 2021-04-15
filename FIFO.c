/*
 * Name: Patrick Brennan
 * Date: June 1st 2020
 * Assignment: Write a program to preform first in first out (FIFO) replacement algorithm
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX 100

void fifo (int sz, int pages[]){
	int cache [sz];
	int index=0;									//index that represents to oldest page (first in)
	int j;
	for (j=0; j< 100; j++){								//loop through all pages
		int temp=0;	
		int i;
		for (i=0; i<sz; i++){							//loop through cache
			if (cache[i]== pages[j]){					//if page is already in the cache, no page fault
				temp=1;
			}
		}
		if(temp==0){								//if page is not in cache...
			int page_num= pages[j];
			printf("Page %d caused a page fault.\n", page_num);		//...print page fault
			cache[index]= page_num;						//adds page to cache at index (replaces oldest page)
			index++;							//increments index to updated oldest page
			if (index>=sz){
				index=0;						//sets pointer to beginning of cache if it reaches the end
			}			
		}
	}	
}

int main (int argc, char *argv[]){
	int size= atoi(argv[1]);
	char pages[MAX];
	int pageNumbers[MAX];
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
		int page = atoi (pages);
		pageNumbers[count]= page;
		count++;
	}
	int i;
	fifo(size, pageNumbers);
	return 0;
}
