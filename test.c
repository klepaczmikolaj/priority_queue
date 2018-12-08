#include <stdio.h>
#include "priority_queue.h"


int main(int argc, char const *argv[]){
    PriorityQueue queue;
    char *path = "priority_queue_str";
    key_t key;
    key = ftok(path, 1);
    initQueue(&queue, key);

    printf("head: %d tail: %d\n", queue.sharedMem->head, queue.sharedMem->tail);


    printf("no. elements: %d\n", queue.sharedMem->size);
    int i;
    for(i = 0; i < 12; i++){
        QueueElement elem;
        elem = createElement(i, HIGH);
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