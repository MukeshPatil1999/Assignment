#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 1
#define NUM_ITEMS 20

int buffer[BUFFER_SIZE];
int buffer_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < NUM_ITEMS; ++i) {
        int item = rand() % 100;
        
        pthread_mutex_lock(&mutex);
        while (buffer_count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_producer, &mutex);
        }
        
        buffer[buffer_count++] = item;
        printf("Producer %d produced: %d\n", id, item);
        
        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    for (int i = 0; i < NUM_ITEMS * NUM_PRODUCERS; ++i) {
        pthread_mutex_lock(&mutex);
        while (buffer_count == 0) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }
        
        int item = buffer[--buffer_count];
        printf("Consumer consumed: %d\n", item);
        
        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    
    pthread_t producer_threads[NUM_PRODUCERS];
    pthread_t consumer_threads[NUM_CONSUMERS];
    
    int producer_ids[NUM_PRODUCERS];
    
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        producer_ids[i] = i;
        pthread_create(&producer_threads[i], NULL, producer, &producer_ids[i]);
    }
    
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        pthread_create(&consumer_threads[i], NULL, consumer, NULL);
    }
    
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        pthread_join(producer_threads[i], NULL);
    }
    
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        pthread_join(consumer_threads[i], NULL);
    }
    
    return 0;
}