#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/time.h>
#include "priority_queue.h"

#define SIMULATION_TIME 10
#define QUEUE_NUMBER 4
#define PROCESS_INTERVAL 1

typedef enum {LEFT_UP, RIGHT_UP, LEFT_DOWN, RIGHT_DOWN} queueID;

int randSleep(){
    int interval = 1e6 * PROCESS_INTERVAL;
    interval = interval * 3 / 4 + rand() % (interval / 2);
    return interval;
}

void producer(PriorityQueue *queue){
    int i, randVal, ranSleep;
    time_t timer;
    timer = time(0);
    while(time(0) - timer <= SIMULATION_TIME){
        QueueElement element;
        randVal = rand() % 10;
        element = createElement(randVal, LOW);
        enqueue(queue, element);
        ranSleep = randSleep();
        usleep(ranSleep);
    }
}

void producerDouble(PriorityQueue *queue1, PriorityQueue *queue2){
    int i, randVal, ranSleep, choice;
    time_t timer;
    timer = time(0);
    while(time(0) - timer <= SIMULATION_TIME){
        QueueElement element;
        randVal = rand() % 10;
        element = createElement(randVal, LOW);
        choice = rand() % 2;
        if(choice == 0)
            enqueue(queue1, element);
        else
            enqueue(queue2, element);
        ranSleep = randSleep() / 2;
        usleep(ranSleep);
    }
}

void consumer(PriorityQueue *queue){
    int i, ranSleep;
    time_t timer;
    timer = time(0);
    sleep(5);
    while(time(0) - timer <= SIMULATION_TIME){
        QueueElement element;
        dequeue(queue, &element);
        ranSleep = randSleep();
        usleep(ranSleep);
    }
}

void consumerDouble(PriorityQueue *queue1, PriorityQueue *queue2){
    int i, ranSleep;
    time_t timer;
    timer = time(0);
    sleep(5);
    while(time(0) - timer <= SIMULATION_TIME){
        QueueElement element;
        if(queue1->sharedMem->priorQuantity >= queue2->sharedMem->priorQuantity)
            dequeue(queue1, &element);
        else
            dequeue(queue2, &element);
        ranSleep = randSleep() / 2;
        usleep(ranSleep);
    }
}

void producerConsumer(PriorityQueue *sourceQueue, PriorityQueue *destinationQueue){
    int ranSleep;
    time_t timer;
    timer = time(0);
    sleep(3);
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
    printf("\nLeft down: \n");
    displayQueue(&queueList[LEFT_DOWN]);
    printf("\nRight up: \n");
    displayQueue(&queueList[RIGHT_UP]);
    printf("\nRight down: \n");
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
        key = ftok(path, i+1);
        initQueue(&queueList[i], key);
    }

    PID1 = fork();
    PID2 = fork();

    if(PID1 == 0 && PID2 == 0){
        //child 1
        producer(&queueList[LEFT_DOWN]);
        return 0;
    }
    else if(PID1 == 0 && PID2 > 0){
        //child 2
        consumer(&queueList[LEFT_UP]);
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