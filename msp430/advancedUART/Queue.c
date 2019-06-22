/*
 * Queue.c
 *
 *  Created on: Apr 9, 2019
 *      Author: WINNGUYEN
 */
#include "Queue.h"
struct Queue createQueue(int size)
{
    struct Queue tmp ;
    if(MAX_QUEUE_SIZE > size){
        tmp.buffer = malloc(size);
        tmp.size = size;
    }
    else{
        tmp.buffer = malloc(MAX_QUEUE_SIZE);
        tmp.size = MAX_QUEUE_SIZE;
    }
    tmp.front = tmp.rear = -1;
    tmp.numChar = 0;

    return tmp;
}
bool isEmpty(const struct Queue* this)
{
    return this->numChar == 0;
}
bool isFull(const struct Queue* this)
{
    return this->numChar == this->size;
}
uint16_t enqueue(struct Queue* this ,char u8num)
{
    this->rear = (this->rear + 1)% this->size;
    //if(isFull(this)) return FULL_KEY;
    //else
    //{
        this->buffer[this->rear] = u8num;
        this->numChar++;
        return  SUCCESS_KEY;
    //}
}
uint16_t dequeue(struct Queue* this ,char* u8num)
{
    if(isEmpty(this)) return EMPTY_KEY;
    else
    {
        this->front = (this->front+1) % this->size;
        *u8num = this->buffer[this->front];
        this->numChar--;
        return  SUCCESS_KEY;
    }
}

