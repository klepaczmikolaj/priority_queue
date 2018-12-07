#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "priority_queue.h"

// Private
void setSemaphore(PriorityQueue *q, Semaphores sem, int operation){
    struct sembuf semB;
    semB.sem_flg = 0;
    semB.sem_num = sem;
    semB.sem_op = operation;
    if(semop(q->semID, &semB, 1) == -1){
        perror("semop");
        exit(1);
    }
}

void allocateMemory(PriorityQueue *q, key_t key){
    size_t memSize;
    // allocate memory and get its id
    memSize = sizeof(SharedMemory);
    q->shmID = shmget(key, memSize, IPC_CREAT | 0600);
    if(q->shmID == -1){
        perror("error allocating shared memory");
        exit(1);
    }
}

void assignMem(PriorityQueue *q){
    // assign memory to the queue
    q->sharedMem = shmat(q->shmID, NULL, 0);
    if(q->sharedMem == (SharedMemory *)(-1)){
        perror("error assigning memory to buffer");
        exit(1);
    }
}

void detachMem(PriorityQueue *q){
    //detach memory from the buffer
    shmdt(q->sharedMem);
}

void initSemaphores(PriorityQueue *q, key_t key){
    q->semID = semget(key, SEMAPHORE_NUMBER, 0600 | IPC_CREAT);
    if(q->semID == -1){
        perror("error initializing semaphore");
        exit(1);
    }
    semctl(q->semID, MUTEX, SETVAL, 1);
    semctl(q->semID, FULL, SETVAL, 0);
    semctl(q->semID, EMPTY, SETVAL, QUEUE_CAPACITY - 1);

}

void semUp(PriorityQueue *q, Semaphores sem){
    setSemaphore(q, sem, 1);
}

void semDown(PriorityQueue *q, Semaphores sem){
    setSemaphore(q, sem, -1);
}

// Public
int isBufferFull(PriorityQueue *q){
    //return (q->sharedMem->tail + 1) % QUEUE_CAPACITY == q->sharedMem->head;
    int size;
    semDown(q, MUTEX);
    size = q->sharedMem->size;
    semUp(q, MUTEX);
    return size == QUEUE_CAPACITY;
}

int isBufferEmpty(PriorityQueue *q){
    int size;
    semDown(q, MUTEX);
    size = q->sharedMem->size;
    semUp(q, MUTEX);
    return size == 0;
}

int initQueue(PriorityQueue *q, char *keyStr){
    key_t key;

    key = ftok(keyStr, 1);
    allocateMemory(q, key);
    initSemaphores(q, key);
    assignMem(q);

    q->sharedMem->head = 0;
    q->sharedMem->tail = 0;
    q->sharedMem->size = 0;
}

int deleteQueue(PriorityQueue *q){
    semctl(q->semID, 0, IPC_RMID);
    shmctl(q->shmID, IPC_RMID, NULL);
    detachMem(q);
}


int enqueue(PriorityQueue *q, QueueElement element){
    if(q == NULL){
        perror("queue pointer is null");
        exit(1);
    }

    if(isBufferFull(q)){
        return 0;
    }

    semDown(q, EMPTY);
    semDown(q, MUTEX);
    //start of critical section
    q->sharedMem->buffer[q->sharedMem->tail] = element;
    q->sharedMem->tail = (q->sharedMem->tail + 1) % QUEUE_CAPACITY;
    q->sharedMem->size++;

    //end of critical section
    
    semUp(q, MUTEX);
    semUp(q, FULL);

    return 1;
}

int dequeue(PriorityQueue *q, QueueElement *element){
    if(q == NULL){
        perror("queue pointer is null");
        exit(1);
    }

    if(isBufferEmpty(q)){
        return 0;
    }

    semDown(q, FULL);
    semDown(q, MUTEX);
    //start of critical

    *element = q->sharedMem->buffer[q->sharedMem->head];
    q->sharedMem->head = (q->sharedMem->head + 1) % QUEUE_CAPACITY;
    q->sharedMem->size--;

    //end of critical
    semUp(q, MUTEX);
    semUp(q, EMPTY);
    
    return 1;
}

QueueElement createElement(int value, Prior priority){
    QueueElement element;
    element.value = value;
    element.priority = priority;
    return element;
}