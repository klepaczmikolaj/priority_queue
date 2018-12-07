#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <sys/shm.h>

#define QUEUE_CAPACITY 11
#define SEMAPHORE_NUMBER 3

typedef enum {NORMAL, PRIORITY} QueueType;
typedef enum {LOW, HIGH} Prior;
typedef enum {MUTEX, FULL, EMPTY} Semaphores;

typedef struct elem{
    Prior priority;
    int value; 
} QueueElement;

typedef struct shm{
    int head;
    int tail;
    int size;
    QueueElement buffer[QUEUE_CAPACITY];
} SharedMemory;

typedef struct prQ{
    QueueType type;
    int shmID;
    int semID;
    SharedMemory *sharedMem;
} PriorityQueue;

// Private
static void setSemaphore(PriorityQueue *q, Semaphores sem, int operation);
static void allocateMemory(PriorityQueue *q, key_t key);
static void initSemaphores(PriorityQueue *q, key_t key);
static void semUp(PriorityQueue *q, Semaphores sem);
static void semDown(PriorityQueue *q, Semaphores sem);

// Public
void assignMem(PriorityQueue *q);
void detachMem(PriorityQueue *q);

int isBufferFull(PriorityQueue *q);
int isBufferEmpty(PriorityQueue *q);

int initQueue(PriorityQueue *q, char *keyStr);
int deleteQueue(PriorityQueue *q);
int enqueue(PriorityQueue *q, QueueElement element);
int dequeue(PriorityQueue *q, QueueElement *element);
QueueElement createElement(int value, Prior priority);

#endif