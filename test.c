#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"


int main(int argc, char const *argv[]){
    PriorityQueue queue;
    char *path = "priority_queue_str";
    key_t key;
    key = ftok(path, 1);
    initQueue(&queue, key, PRIORITY);

    int i;
    for(i = 0; i < 12; i++){
        QueueElement elem;
        Prior priority;
        if(rand() % 2 == 0)
            priority = HIGH;
        else
            priority = LOW;
        elem = createElement(i, priority);
        enqueue(&queue, elem);

        displayQueue(&queue);
        printf("head: %d, tail: %d\n\n", queue.sharedMem->head, queue.sharedMem->tail);
        printf("prior: %d\n\n", queue.sharedMem->priorQuantity);
    }


    for(i = 0; i < 12; i++){
        QueueElement elem;
        dequeue(&queue, &elem);
        displayQueue(&queue);
        printf("head: %d, tail: %d\n\n", queue.sharedMem->head, queue.sharedMem->tail);
        printf("prior: %d\n\n", queue.sharedMem->priorQuantity);
    }


    deleteQueue(&queue);

    return 0;
}