
#ifndef CIRCBUFFER_H
#define CIRCBUFFER_H

typedef struct CircBuffer {
    char* buffer;
    int start;
    int end;
    int size;
} CircBuffer;

CircBuffer* initBuffer (int kbsize);

void destroyBuffer (CircBuffer* cb);

char readBuffer (CircBuffer* cb);
char writeBuffer (CircBuffer* cb, char d);


#endif
