#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include "priority_queue.h"

void producer(PriorityQueue *queue){
    int i, randVal;
    
    for(i = 0; i < 3; i++){
        QueueElement element;
        randVal = rand() % 10;
        element = createElement(randVal, LOW);
        if(enqueue(queue, element))
            printf("Element with value: %d and Priority: LOW was added\n", element.value);
        else
            continue;
        sleep(1);
    }
}

void consumer(PriorityQueue *queue){
    int i, randVal;
    sleep(4);
    printf("no. elements: %d\n", semctl(queue->semID, FULL, 12));

    assignMem(queue);
    printf("head: %d\n", queue->buffer[0].value);
    printf("tail: %d\n", queue->buffer[2].value);
    detachMem(queue);

    for(i = 0; i < 2; i++){
        QueueElement element;
        if(dequeue(queue, &element))
            printf("Element recieved from queue, value: %d\n", element.value);

        printf("no. elements: %d\n", semctl(queue->semID, FULL, 12));
        sleep(1);
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
        printf("done\n");
    }

    wait(NULL);

    deleteQueue(&queue);

    return 0;
}