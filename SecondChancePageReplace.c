#include <stdio.h>
#include <stdlib.h>
#define MAX 100

/*
Assignment: Write a program to preform the second chance page replacement algorithm
*/

void secondChance (int sz, int pages[]){
	int cache [sz];										// cache holds page numbers
	int bit [sz];										// array of bits holds referenced bit value
	int c;
	for (c=0; c<sz; c++){									// initialize all bits to 0 (unreferenced)
		bit[c]=0;
	}
	int index=0;
	int j;
	for (j=0; j< MAX; j++){	
		int page_num= pages[j];								// loop through all pages
		int temp=0;									// temp= 1 tells us page is already in queue
		int i;
		for (i=0; i<=sz; i++){
			if (cache[i]== page_num){						// check if page is in queue
				temp= 1;
				bit[i]= 1;							// set bit to referenced if already in queue
			}
		}	
		if(temp==0){									// page is not already in queue-> page fault
			printf("Page %d caused a page fault.\n", page_num);
			if (j<sz){								// if cache is not yet full, add page to next open stop. bit stays 0
				cache[j]= page_num;
				bit[j]=1;
			}
			else{
				int q;
				for (q=0; q<=sz; q++){					
					if (bit[index]==0){					// start at oldest pages, then swap out if unreferenced (bit=0). 
						cache[index]= page_num;				// swap
						bit[index]=1;					// set new page's bit to referenced for 'first chance'
						break;					
					}
					else{							// if old page's bit is 1...
						bit[index]=0;					// ...old page loses its 'first chance', bit gets set to 0
					}
					index++;						//increment index, which points to oldest page
					if (index>=sz){
						index=0;					//resets index to beginning of cache if it passes the last element
					}
				}
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
	secondChance(size, pageNumbers);
	return 0;
}

