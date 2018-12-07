#include <stdio.h>
#include "priority_queue.h"

int main(int argc, char const *argv[]){
    PriorityQueue queue;
    char *path = "priority_queue_str";
    initQueue(&queue, path);

    printf("head: %d tail: %d\n", queue.sharedMem->head, queue.sharedMem->tail);


    printf("no. elements: %d\n", queue.sharedMem->size);
    int i;
   /* for(i = 0; i < 3; i++){
        QueueElement elem;
        elem = createElement(2, HIGH);
        enqueue(&queue, elem);
        printf("no. elements: %d\n", queue.sharedMem->size);
    }
*/

    for(i = 0; i < 6; i++){
        QueueElement elem;
        dequeue(&queue, &elem);
        printf("elem val: %d\n", elem.value);
        printf("no. elements: %d\n", queue.sharedMem->size);
    }
    
    deleteQueue(&queue);

    return 0;
}