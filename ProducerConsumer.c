/*
 * * Name: Patrick Brennan
 * * Date: March 10th, 2020
   * Lab 5 - Sync and semaphores
 * * Solves the producer - consumer problem using semaphores
   */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZE 10
#define NTHREADS 10
#define PRODUCER_LOOPS 2
#define CONSUMER_LOOPS 2
  
#define TRUE 1
#define FALSE 0
 
typedef int buffer_t;
buffer_t buffer[SIZE];
int buffer_index;
  
pthread_mutex_t buffer_mutex; //inital buffer will be empty
sem_t full_sem; //full_sem = 0 buffer is FULL  
sem_t empty_sem; //empty_sem = 0 buffer is EMPTY

/*adding a value to the buffer used by producer*/
void insertbuffer(buffer_t value) {
    if (buffer_index < SIZE) {
        buffer[buffer_index++] = value;
    }else {
        printf("Buffer overflow\n");
    }
}

/*taking an element from the buffer used by the consumer*/
buffer_t dequeuebuffer() {
    if (buffer_index > 0) {
        return buffer[--buffer_index];
    }else {
        printf("Buffer underflow\n");
    }
    return 0;
}

/*check if buffer is full used by producer*/
int isFull() {
    if (buffer_index == SIZE)
        return TRUE;
    return FALSE;
}

/*check if buffer is empty used by consumer*/
int isEmpty() {
    if (buffer_index == 0)
        return TRUE;
    return FALSE;
}

/*producer thread */
void *producer(void *thread_n) {
    int thread_numb = *(int *)thread_n;
    buffer_t value;
    int i = 0;
    
    while (i++ < PRODUCER_LOOPS) {
        /*produce the next item*/
        sleep(rand() % 10);
        value = rand() % 100;
        pthread_mutex_lock(&buffer_mutex); //
        
        do {
            pthread_mutex_unlock(&buffer_mutex); //condition variable for unlock and wait avoids race condition
            sem_wait(&full_sem); //if sem = 0 then wait, and if sem > 0 go and decrement
            pthread_mutex_lock(&buffer_mutex); //lock after finsih so another thread can get access to buffer
        }while (isFull()); //check fpr other threads wake up
        
        /*add item to the buffer*/
        insertbuffer(value);
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&empty_sem); //after the increment empty buffer semaphore
        printf("Producer %d added %d to buffer\n", thread_numb, value);
    }
    pthread_exit(0);
}
  
/*consumer thread*/
void *consumer(void *thread_n) {
    int thread_numb = *(int *)thread_n;
    buffer_t value;
    int i = 0;
    
    while (i++ < PRODUCER_LOOPS) {
        pthread_mutex_lock(&buffer_mutex);
        /*remove next item from buffer*/
        do {
            pthread_mutex_unlock(&buffer_mutex);
            sem_wait(&empty_sem); //if sem = 0 then wait, and if sem > 0 go and increment
            pthread_mutex_lock(&buffer_mutex);
        } while (isEmpty()); //check for other threads wake up
        
        /*consume the item*/
        value = dequeuebuffer(value);
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full_sem); //after the decrement full buffer semaphore
        printf("Consumer %d dequeue %d from buffer\n", thread_numb, value);
   }
    pthread_exit(0);
}
  
int main(int argc, int **argv) {
    buffer_index = 0;
    
    pthread_mutex_init(&buffer_mutex, NULL);
    sem_init(&full_sem, 0, SIZE);
    sem_init(&empty_sem, 0, 0);

    pthread_t thread[NTHREADS];
    int thread_numb[NTHREADS];
    int i;
    for (i = 0; i < NTHREADS;) {
        thread_numb[i] = i;
        pthread_create(thread + i, NULL, producer, thread_numb + i); //call for the producer
        i++;
        
        thread_numb[i] = i;
        pthread_create(&thread[i], NULL, consumer, &thread_numb[i]); //call for the consumer
        i++;
        
    }
  
    for (i = 0; i < NTHREADS; i++)
        pthread_join(thread[i], NULL);
    
    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&full_sem);
    sem_destroy(&empty_sem);
  
    return 0;
}

