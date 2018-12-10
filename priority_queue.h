/*
  ************* Mikołaj Klepacz *************
  ****** Priority queue implementation ******

  This library allows to create and work with priority queue.
  The queue itself is implemented using semaphores and linux shared memory. 
  Libraries used: <sys/shm.h> <sys/sem.h>



*/


#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <sys/shm.h>

#define QUEUE_CAPACITY 10
#define SEMAPHORE_NUMBER 3

typedef enum {NORMAL, PRIORITY} QueueType;
typedef enum {LOW, HIGH} Prior;
typedef enum {MUTEX, FULL, EMPTY} Semaphores;

/* queue elements type */
typedef struct elem{
    Prior priority;
    int value; 
} QueueElement;

typedef struct shm{
    int head;
    int tail;
    int size;
    int priorQuantity;
    QueueElement buffer[QUEUE_CAPACITY];
} SharedMemory;

typedef struct prQ{
    QueueType type;
    int shmID;
    int semID;
    SharedMemory *sharedMem;
} PriorityQueue;

/* Private functions */
static void setSemaphore(PriorityQueue *q, Semaphores sem, int operation);
static void allocateMemory(PriorityQueue *q, key_t key);
static void initSemaphores(PriorityQueue *q, key_t key);
static void semUp(PriorityQueue *q, Semaphores sem);
static void semDown(PriorityQueue *q, Semaphores sem);
static void assignMem(PriorityQueue *q);
static void detachMem(PriorityQueue *q);

/* Public functions */

/*  
    Queue should be initialized at first using unique key.
    User can choose the type of the queue - NORMAL (typical fifo queue)
    or PRIORITY (elements with priority=HIGH are enqueued straignt to the front)
    Once the queue is initialized, it should be deleted afterwards
    as deleteQueue function frees shared memory assigned to the queue
*/

int initQueue(PriorityQueue *q, key_t key, QueueType type);
int deleteQueue(PriorityQueue *q);

/* Elementary queue operations */
int isBufferFull(PriorityQueue *q);
int isBufferEmpty(PriorityQueue *q);

int enqueue(PriorityQueue *q, QueueElement element);
int dequeue(PriorityQueue *q, QueueElement *element);

// Additional functions
void displayQueue(PriorityQueue *queue);
QueueElement createElement(int value, Prior priority);

#endif