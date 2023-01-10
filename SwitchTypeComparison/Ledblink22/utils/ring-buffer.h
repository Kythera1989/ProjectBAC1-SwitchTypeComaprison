/**
 * @Authors: Venier/Winter
 * @brief A lightweight header-only ringbuffer implementation
 *got some codesnippets and some ideas from :https://www.mikrocontroller.net/articles/FIFO , https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/ and some Youtube videos :-)
 */

#ifndef ring_buffer_h_Opee6yieGhiph2Du
#define ring_buffer_h_Opee6yieGhiph2Du

#include <stdbool.h>
#include <stdint.h>

#ifndef RING_BUFFER_CAPACITY
/* Define size of ring buffer with a default value. */
#define RING_BUFFER_CAPACITY 128
#endif

typedef struct {
    uint8_t buffer[RING_BUFFER_CAPACITY];
    uint8_t* read;
    uint8_t* write;
    uint8_t usedCapacity;
    bool isFull;
} RingBuffer;

/** Initialize the given RingBuffer struct.
 * @param ringBuffer The RingBuffer instance
 * @note Must be called before the ring buffer is used.
*/
static inline void ringBufferInit(RingBuffer* ringBuffer)
{
    ringBuffer->read = ringBuffer->buffer;
    ringBuffer->write = ringBuffer->buffer;
    ringBuffer->usedCapacity = 0;
    ringBuffer->isFull = false;
}

/** Returns the number of elements in the ring buffer.
 * @param ringBuffer The RingBuffer instance
 * @return Number of elements
*/
static inline uint8_t ringBufferSize(const RingBuffer* ringBuffer)
{
    return ringBuffer->usedCapacity;
}

/** Returns the number of elements which can be stored at maximum.
 * @return Number of elements
*/
static inline uint8_t ringBufferCapacity()
{
    return RING_BUFFER_CAPACITY - 1;
}

/** Checks if the ring buffer has no elements
 * @param ringBuffer The RingBuffer instance
 * @return  True if empty, false otherwise
*/
static inline bool ringBufferEmpty(const RingBuffer* ringBuffer)
{
    if (ringBuffer->read == ringBuffer->write) { //when read and write pointer are the same the buffer is empty
        return true;
    } else {
        return false;
    }
}

/** Checks if the ring buffer is full.
 * @param ringBuffer The RingBuffer instance
 * @return  True if full, false otherwise
*/
static inline bool ringBufferFull(const RingBuffer* ringBuffer)
{
    if (ringBuffer->usedCapacity == RING_BUFFER_CAPACITY - 1) {
        return true;
    } else {
        return false;
    }
}

/** Pushes the given value to the ring buffer.
 * @param ringBuffer The RingBuffer instance
 * @param[in] value The element to be added
 * @return  True if successful, false otherwise
*/
static inline bool ringBufferPush(RingBuffer* ringBuffer, uint8_t value)
{
    if (ringBuffer->isFull) { //if the buffer is full return
        return false;
    }
    *ringBuffer->write = value;

    if (ringBuffer->write == &ringBuffer->buffer[RING_BUFFER_CAPACITY - 1]) { //check if the write pointer is standing at the the end of the buffer
        ringBuffer->write = ringBuffer->buffer;
    } else {
        ringBuffer->write++;
    }
    ringBuffer->usedCapacity++;
    if (ringBufferFull(ringBuffer) == true) { //check if buffer is now full if yes Full=true
        ringBuffer->isFull = true;
    }
    return true;
}

/** Returns the oldest value from the ring buffer, without removing it.
 * @param ringBuffer The RingBuffer instance
 * @param[out] value The oldest value
 * @return  True if successful, false otherwise
*/
static inline bool ringBufferPeek(RingBuffer* ringBuffer, uint8_t* value)
{
    if (ringBuffer->usedCapacity == 0) {
        return false;
    }
    *value = *ringBuffer->read;
    return true;
}

/** Removes the oldest value from the ring buffer.
 * @param ringBuffer The RingBuffer instance
 * @param[out] value The removed value
 * @return  True if successful, false otherwise
*/
static inline bool ringBufferPop(RingBuffer* ringBuffer, uint8_t* value)
{
    if (ringBuffer->usedCapacity == 0) {
        return false;
    }
    *value = *ringBuffer->read;
    if (ringBuffer->read == &ringBuffer->buffer[RING_BUFFER_CAPACITY - 1]) { //standing at the end of the ringbuffer set read back to starting position
        ringBuffer->read = ringBuffer->buffer;
    } else {
        ringBuffer->read++;
    }
    ringBuffer->usedCapacity--;
    if (ringBuffer->isFull == true) {
        ringBuffer->isFull = false;
    }
    return true;
}

#endif
