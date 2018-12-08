#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/time.h>
#include "priority_queue.h"

#define SIMULATION_TIME 30
#define QUEUE_NUMBER 4
#define PROCESS_INTERVAL 2
// probability of high priority element creation: 1/HIGH_PRI_PROBABILITY
#define HIGH_PRI_PROBABILITY 4  

typedef enum {LEFT_UP, RIGHT_UP, LEFT_DOWN, RIGHT_DOWN} queueID;

int randSleep(){
    int interval = 1e6 * PROCESS_INTERVAL;
    interval = interval * 3 / 4 + rand() % (interval / 2);
    return interval;
}

QueueElement createRandomElement(){
    Prior priority;
    int randVal;
    randVal = rand() % 10;
    if(rand() % HIGH_PRI_PROBABILITY == 0)
        priority = HIGH;
    else
        priority = LOW;
    return createElement(randVal, priority);
}

void producerDouble(PriorityQueue *queue1, PriorityQueue *queue2){
    int i, ranSleep;
    time_t timer;
    timer = time(0);
    while(time(0) - timer <= SIMULATION_TIME){
        QueueElement element;
        element = createRandomElement();
        if(rand() % 2 == 0)
            enqueue(queue1, element);
        else
            enqueue(queue2, element);
        ranSleep = randSleep() / 2;
        usleep(ranSleep);
    }
}

void consumerDouble(PriorityQueue *queue1, PriorityQueue *queue2){
    int i, ranSleep;
    time_t timer;
    timer = time(0);
    while(time(0) - timer <= SIMULATION_TIME){
        QueueElement element;
        if(queue1->sharedMem->priorQuantity > queue2->sharedMem->priorQuantity)
            dequeue(queue1, &element);
        else if(queue1->sharedMem->priorQuantity < queue2->sharedMem->priorQuantity)
            dequeue(queue2, &element);
        else{
            if(rand() % 2 == 0)
                dequeue(queue1, &element);
            else
                dequeue(queue2, &element);
        }
        ranSleep = randSleep();
        usleep(ranSleep);
    }
}

void producerConsumer(PriorityQueue *sourceQueue, PriorityQueue *destinationQueue){
    int ranSleep;
    time_t timer;
    timer = time(0);
    sleep(10);
    while(time(0) - timer <= SIMULATION_TIME){
        QueueElement element;
        if(dequeue(sourceQueue, &element));
            enqueue(destinationQueue, element);

        ranSleep = randSleep();
        usleep(ranSleep);
    }
}

void displayQueues(PriorityQueue queueList[QUEUE_NUMBER]){
    printf("\n######################################\n");
    printf("\nLeft up: \n");
    displayQueue(&queueList[LEFT_UP]);
    printf("\n\t\t\tRight up: \n\t\t\t");
    displayQueue(&queueList[RIGHT_UP]);
    printf("\nLeft down: \n");
    displayQueue(&queueList[LEFT_DOWN]);
    printf("\n\t\t\tRight down: \n\t\t\t");
    displayQueue(&queueList[RIGHT_DOWN]);
    printf("\n######################################\n");
}

void displayTask(PriorityQueue queueList[QUEUE_NUMBER]){
    int interval;
    time_t timer;

    timer = time(0);
    interval = 50000;   //microseconds

    while(time(0) - timer <= SIMULATION_TIME){
        //clear console 
        printf("\033[H\033[J");
        displayQueues(queueList);

        usleep(interval);
    }

}

int main(int argc, char const *argv[]){
    int PID1, PID2, i;
    char *path;
    PriorityQueue queueList[QUEUE_NUMBER];

    path = "./priority_queue.c";
    srand(time(NULL));

    for(i = 0; i < QUEUE_NUMBER; i++){
        key_t key;
        QueueType type;
        key = ftok(path, i+1);
        if(i == LEFT_UP || i == RIGHT_UP)
            type = PRIORITY;
        else
            type = NORMAL;
        initQueue(&queueList[i], key, type);
    }

    PID1 = fork();
    PID2 = fork();

    if(PID1 == 0 && PID2 == 0){
        //child 1
        //producer(&queueList[LEFT_DOWN]);
        producerDouble(&queueList[LEFT_DOWN], &queueList[RIGHT_DOWN]);
        return 0;
    }
    else if(PID1 == 0 && PID2 > 0){
        //child 2
        //consumer(&queueList[LEFT_UP]);
        consumerDouble(&queueList[LEFT_UP], &queueList[RIGHT_UP]);
        return 0;
    }
    else if(PID1 > 0 && PID2 == 0){
        //child 3
        producerConsumer(&queueList[LEFT_DOWN], &queueList[LEFT_UP]);
        return 0;
    }
    else{
        if(fork() == 0){
            //child 4
            producerConsumer(&queueList[RIGHT_DOWN], &queueList[RIGHT_UP]);
            return 0;
        }
        else{
            //parent
            displayTask(queueList);
        }
    }

    for(i = 0; i < 4; i++)
        wait(NULL);

    for(i = 0; i < QUEUE_NUMBER; i++)
        deleteQueue(&queueList[i]);

    return 0;
}