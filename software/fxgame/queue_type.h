//This defines a queue structure that was useful for doing animations, you can use this for other things but this version only stores ints.

#ifndef QUEUE_TYPE_H_
#define QUEUE_TYPE_H_

typedef struct {
    int front;
    int back;
    int capacity;
    int size;
    int *array;
} Queue;

Queue *queue_new(int capacity);

int enqueue(Queue * queue, int value);

int dequeue(Queue * queue);

int is_empty(Queue * queue);

int is_full(Queue * queue);

void verbose_enqueue(Queue * queue, int value);

void verbose_dequeue(Queue * queue);

#endif
