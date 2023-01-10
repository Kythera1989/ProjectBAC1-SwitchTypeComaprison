#include "avrhal/usart.h"
#include "avrhal/usart-string.h"
#include "time.h"
#include "utils/bit.h"
#include "utils/ring-buffer.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/atomic.h>
#include <util/delay.h>
RingBuffer* read; //we need two buffers one for read and write
RingBuffer* write;
// some helper funktions
static inline void enableCompleteInterruptRecieve() //enable  recieve RXC Flag
{
    BIT_SET(UCSR0B, RXCIE0);
}

static inline void enableTransmitBufferEmptyInterrupt() //set UDRIE 1= enable data Register empty interrrupt
{
    BIT_SET(UCSR0B, UDRIE0);
}
static inline void disableTransmitBufferEmptyInterrupt() //set UDRIE 0= dissable data Register empty interrrupt
{
    BIT_CLR(UCSR0B, UDRIE0);
}
ISR(USART0_UDRE_vect) //load new data in UART Register got the idea from: https://www.mikrocontroller.net/articles/Interrupt
{
    uint8_t pop;
    if (ringBufferPop(write, &pop) == true) {
        UDR0 = pop;
    } else {
        disableTransmitBufferEmptyInterrupt();
    }
}

ISR(USART0_RX_vect) //is called when a data is received
{
    uint8_t push;
    push = UDR0;
    ringBufferPush(read, push);
}

void usartSetup(UsartBaudrate baud)
{
    //calculation of baud rate register value according to formula p143.
    uint16_t ubrrValue = (F_CPU / (16UL * baud)) - 1; //=51,083 with U2X=0 Error0,2% table p167
    UBRR0L = ubrrValue;
    UCSR0C = BIT_CLR(UCSR0C, UMSEL00); //set Asynchronus normal Mode bit 6==0
    BIT_SET(UCSR0B, RXEN0); // enable recieve
    BIT_SET(UCSR0B, TXEN0); //enable transmit
    enableCompleteInterruptRecieve(); //enable interrupt for recieving data
    read = malloc(sizeof(RingBuffer)); //allocate memory for buffer
    write = malloc(sizeof(RingBuffer));
    ringBufferInit(read); //initialise buffers
    ringBufferInit(write);
}
uint8_t usartAvailableForRead()
{
    return ringBufferSize(read);
}
uint8_t usartAvailableForWrite()
{
    return (ringBufferCapacity(write) - ringBufferSize(write));
}
bool usartPeek(uint8_t* byte)
{
    return ringBufferPeek(read, byte);
}
bool usartRead(uint8_t* byte)
{
    bool pop;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) //got this from https://www.nongnu.org/avr-libc/user-manual/group__util__atomic.html / https://www.mikrocontroller.net/articles/Interrupt
    {
        pop = ringBufferPop(read, byte);
    }
    return pop;
}
uint8_t usartReadBytes(uint8_t* bytes, uint8_t maxRead)
{
    uint8_t temp;
    uint8_t cnt = 0;
    for (uint8_t i = 0; i < maxRead; i++) {
        if (ringBufferPop(read, &temp) == true) {
            bytes[i] = temp;
            cnt++;
        }
    }
    return cnt;
}
bool usartWrite(uint8_t byte)
{
    bool push;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) //got this from https://www.nongnu.org/avr-libc/user-manual/group__util__atomic.html
    {
        push = ringBufferPush(write, byte);
    }
    enableTransmitBufferEmptyInterrupt();
    return push;
}
void usartFlush()
{
    while (ringBufferEmpty(write) == false) { //wait until the buffer is empty
        //do nothing
    }
    if (BIT_IS_SET(UCSR0A, UDRE0) && ringBufferEmpty(write) == true) { //check if buffer empty and empty data register flag is set
        return;
    }
}
