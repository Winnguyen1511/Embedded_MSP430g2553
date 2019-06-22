#include "RingBuffer.h"

int RingBuffer_create(struct RingBuffer *rb, uint8_t* data, RB_SIZE MAX_SIZE)
{
    (*rb).data = data;
    (*rb).head = (*rb).tail = 0;
    (*rb).MAX_SIZE = MAX_SIZE;
}

int RingBuffer_add(struct RingBuffer *rb, uint8_t* data, RB_SIZE dataSize)
{
    if(!rb || !data){
        return -1;
    }

    RB_SIZE numberOfData = RingBuffer_getFreeSpace(*rb);
    if(numberOfData > dataSize){
        numberOfData = dataSize;
    }
    int ret = numberOfData;
    while(numberOfData){
        (*rb).data[(*rb).head++] = *data++;
        if((*rb).head == (*rb).MAX_SIZE){
            (*rb).head = 0;
        }
        numberOfData--;
    }

    return ret;
}

int RingBuffer_get(struct RingBuffer *rb, uint8_t* data, RB_SIZE dataSize)
{
    if(!rb || !data){
        return -1;
    }

    RB_SIZE numberOfData = RingBuffer_getUsedSpace(*rb);
    if(numberOfData > dataSize){
        numberOfData = dataSize;
    }

    int ret = numberOfData;

    while(numberOfData){
        *data++ = (*rb).data[(*rb).tail++];
        if((*rb).tail == (*rb).MAX_SIZE){
            (*rb).tail = 0;
        }
        numberOfData--;
    }
    return ret;
}

int RingBuffer_getFreeSpace(struct RingBuffer rb)
{
    int freeSpace = rb.tail - rb.head - 1;
    if(freeSpace >= 0)
        return freeSpace;
    else
        return rb.MAX_SIZE + freeSpace;
}

int RingBuffer_getUsedSpace(struct RingBuffer rb)
{
    int usedSpace = rb.head - rb.tail;
    if(usedSpace >=0){
        return usedSpace;
    }else{
        return rb.MAX_SIZE + usedSpace;
    }
}
