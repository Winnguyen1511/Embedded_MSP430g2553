/*
 * Queue.h
 *
 *  Created on: Apr 9, 2019
 *      Author: WINNGUYEN
 */

#ifndef QUEUE_H_
#define QUEUE_H_
#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#define     MAX_QUEUE_SIZE      16
typedef     int     bool;
#define     true    1
#define     false   0
#define     FULL_KEY    2
#define     EMPTY_KEY   false
#define     SUCCESS_KEY  true
struct Queue
{
    char *buffer;
    int rear, front;
    int numChar;
    int size;
};
struct Queue createQueue(int);
bool isEmpty(const struct Queue*);
bool isFull(const struct Queue*);
uint16_t enqueue(struct Queue*,char);
uint16_t dequeue(struct Queue*,char*);

#endif /* QUEUE_H_ */
