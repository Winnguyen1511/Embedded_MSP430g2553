#ifndef RING_BUFFER_CUSTOM_LIB
#define RING_BUFFER_CUSTOM_LIB
#include <stdint.h>
typedef uint8_t RB_SIZE;

struct RingBuffer{
    uint8_t* data;
    RB_SIZE MAX_SIZE;
    RB_SIZE head;
    RB_SIZE tail;
};

int RingBuffer_create(struct RingBuffer *rb, uint8_t* data, RB_SIZE MAX_SIZE);
int RingBuffer_add(struct RingBuffer *rb, uint8_t* data, RB_SIZE MAX_SIZE);
int RingBuffer_get(struct RingBuffer *rb, uint8_t* data, RB_SIZE MAX_SIZE);
int RingBuffer_getFreeSpace(struct RingBuffer rb);
int RingBuffer_getUsedSpace(struct RingBuffer rb);

#endif
