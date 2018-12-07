#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include "priority_queue.h"

void producer(PriorityQueue *queue){
    int i, randVal, ranSleep;
    for(i = 0; i < 10; i++){
        QueueElement element;
        randVal = rand() % 10;
        element = createElement(randVal, LOW);
        if(enqueue(queue, element)){
            printf("Element with value: %d and Priority: LOW was added\n", element.value);
            printf("Size: %d\n\n", queue->sharedMem->size);
        }
        else
            continue;
        ranSleep = 900000 + rand() % 200000;
        usleep(ranSleep);
        printf("rand: %d \n", ranSleep);
    }
}

void consumer(PriorityQueue *queue){
    int i, ranSleep;
    printf("no. elements: %d\n", queue->sharedMem->size);

    printf("head: %d\n", queue->sharedMem->buffer[0].value);
    printf("tail: %d\n", queue->sharedMem->buffer[2].value);

    for(i = 0; i < 10; i++){
        QueueElement element;
        if(dequeue(queue, &element))
            printf("Element recieved from queue, value: %d\n", element.value);

        printf("no. elements: %d\n\n", queue->sharedMem->size);
        ranSleep = 900000 + rand() % 200000;
        usleep(ranSleep);
        printf("rand: %d \n", ranSleep);
    }
}

int main(int argc, char const *argv[]){
    PriorityQueue queue;
    int PID;
    char *path = "priority_queue_string";
    initQueue(&queue, path);

    srand(time(NULL));
    PID = fork();

    if(PID == 0){
        producer(&queue);
        return 0;
    }
    else{
        consumer(&queue);
    }

    wait(NULL);

    deleteQueue(&queue);

    return 0;
}