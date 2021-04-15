/*
 *Name: Patrick Brennan
Date: April 30th, 2020
Title: Lab4 step 3
This program computes matrix multiplication using threads to compute each row 
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define MATRIXSIZE 5000 // matrix size
int total_threads; // number of threads

int A[MATRIXSIZE][MATRIXSIZE], B[MATRIXSIZE][MATRIXSIZE], C[MATRIXSIZE][MATRIXSIZE],D[MATRIXSIZE][MATRIXSIZE],E[MATRIXSIZE][MATRIXSIZE];


void createMatrix(int z[MATRIXSIZE][MATRIXSIZE])
{
int value = 0;
for (int i = 0; i < MATRIXSIZE; i++)
for (int j = 0; j < MATRIXSIZE; j++)
z[i][j] = value++;
}

void printingMatrix(int z[MATRIXSIZE][MATRIXSIZE])
{
for (int i = 0; i < MATRIXSIZE; i++) {
printf("\n \t| ");
for (int j = 0; j < MATRIXSIZE; j++)
printf("%3d ", z[i][j]);
printf("|");
}
}

void* multiplySlice(void* sliceArray)
{
int p = (int)sliceArray;
int from = (p * MATRIXSIZE)/total_threads;
int to = ((p+1) * MATRIXSIZE)/total_threads;

printf("calculating slicepiece %d - from row %d to %d \n", p, from, to-1);
for (int i = from; i < to; i++)
{
for (int j = 0; j < MATRIXSIZE; j++)
{
C[i][j] = 0;
for ( int k = 0; k < MATRIXSIZE; k++)
       C[i][j] += A[i][k]*B[k][j];
}
}
printf("completed sliceArray %d\n", p);
return 0;
}

int main(int argc, char* argv[])
{

static int i; 

pthread_t* thread;

if (argc!=2)
{
printf("Usage: %p number_of_threads\n",argv[0]);
exit(-1);
}

total_threads = atoi(argv[1]);
createMatrix(A);
createMatrix(B);
thread = (pthread_t*) malloc(total_threads*sizeof(pthread_t));

for ( i = 1; i < total_threads; i++)
{
if (pthread_create (&thread[i], NULL, multiplySlice, (void*)(size_t)i) != 0 )
{
perror("Unable to create thread");
free(thread);
exit(-1);
}
}
multiplySlice(0);


for (int i = 1; i < total_threads; i++)
   pthread_join (thread[i], NULL);

printf("\n\n");
printingMatrix(A);
printf("\n\n\t * \n");
printingMatrix(B);
printf("\n\n\t = \n");
printingMatrix(C);
printf("\n\n");

free(thread);

return 0;

}

