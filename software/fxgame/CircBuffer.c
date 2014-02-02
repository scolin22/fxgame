
#include <stdio.h>

#include "CircBuffer.h"


CircBuffer* initBuffer (int kbsize)
{
    CircBuffer* cb = (CircBuffer*)malloc(sizeof(CircBuffer));
    cb->buffer = (char*)malloc(kbsize*1024*sizeof(char));
    cb->start = 0;
    cb->end = 0;
    cb->size = kbsize*1024;
    return cb;
}

// Read the next item in the buffer and increment start
char readBuffer (CircBuffer* cb)
{
    if (cb->start != cb->end) {
        int i = cb->start;
        cb->start = cb->start + 1 % cb->size;
        return cb->buffer[i];
    } else {
        return NULL;
    }
}

// Write to the next index in the buffer and return 1 or 0 on success or failure
char writeBuffer (CircBuffer* cb, char d)
{
    if (cb->end + 1 % cb->size != cb->start) {
        cb->buffer[cb->end] = d;
        cb->end = cb->end + 1 % cb->size;
        return 1;
    } else {
        return 0;
    }
}

void destroyBuffer (CircBuffer* cb)
{
    free(cb->buffer);
    free(cb);
}
