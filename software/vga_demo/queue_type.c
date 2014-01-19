#include <stdio.h>
#include <stdlib.h>
#include "queue_type.h"

Queue *queue_new(int capacity) {
    Queue *queue = malloc(sizeof(Queue));
    queue->front = 0;
    queue->back = 0;
    queue->capacity = capacity;
    queue->size = 0;

    queue->array = malloc(capacity * sizeof(int));

    return queue;
}

int enqueue(Queue * queue, int value) {
    if (!is_full(queue)) {
        if (!is_empty(queue)) {
            queue->back++;
            queue->back %= queue->capacity;
        }
        queue->array[queue->back] = value;
        queue->size++;
        return 1;
    } else
        return 0;
}

int dequeue(Queue * queue) {
    if (!is_empty(queue)) {
        int dequeued = queue->array[queue->front];
        queue->size--;
        if (!is_empty(queue)) {
            queue->front++;
            queue->front %= queue->capacity;
        }
        return dequeued;
    }
    else
        return 0;
}

int is_full(Queue * queue) {
    if (queue->size >= queue->capacity)
        return 1;
    else
        return 0;
}

int is_empty(Queue * queue) {
    if (queue->size == 0)
        return 1;
    else
        return 0;
}

void print_queue(Queue * queue) {
    printf("Front: %d\n", queue->front);
    printf("Back: %d\n", queue->back);
    int i;
    printf("Current List: ");
    if (queue->size == 0)
        printf("Empty");
    else
        for (i = 0; i < queue->size; i++) {
            int index = queue->front + i;
            index %= queue->capacity;
            printf("%d ", queue->array[index]);
        }
    printf("\n\n");
}

void verbose_enqueue(Queue * queue, int value) {
    if (enqueue(queue, value)) printf("Enqueued %d\n", queue->array[queue->back]);
    else printf("Failed to enqueue\n");

    print_queue(queue);
}

void verbose_dequeue(Queue * queue) {
    int dequeued = dequeue(queue);
    if (dequeued) printf("Dequeued %d\n", dequeued);
    else printf("Failed to dequeue\n");

    print_queue(queue);
}

// int main(int argc, char *argv[]) {

//     Queue *queue = queue_new(1);

//     verbose_enqueue(queue, 1);
//     verbose_enqueue(queue, 2);
//     verbose_enqueue(queue, 3);
//     verbose_enqueue(queue, 4);
//     verbose_enqueue(queue, 5);
//     verbose_dequeue(queue);
//     verbose_dequeue(queue);
//     verbose_dequeue(queue);
//     verbose_dequeue(queue);
//     verbose_dequeue(queue);
//     verbose_dequeue(queue);
//     verbose_enqueue(queue, 5);
//     verbose_enqueue(queue, 5);
//     verbose_enqueue(queue, 5);
//     verbose_enqueue(queue, 5);
//     verbose_enqueue(queue, 5);
//     verbose_enqueue(queue, 5);
// }
