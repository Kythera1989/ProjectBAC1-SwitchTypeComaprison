#include "avrhal/usart-string.h"
#include "avrhal/usart.h"
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

uint8_t usartStringWrite(const char* str)
{
    uint8_t i = 0;
    uint8_t cnt = 0;
    while (str[i] != '\0') { //get the length of the string until \0
        i++;
    }
    if (usartAvailableForWrite() >= i) { //check if buffer size is big enough
        for (int k = 0; k < i; k++) {
            if (usartWrite(str[k]) == true) {
                cnt++;
            }
        }
    }
    usartFlush(); //make sure all buffered data has been written to the interface
    return cnt;
}
uint8_t usartStringPrint(const char* format, ...)
{ //tutorial on youtube:https://www.youtube.com/watch?v=S-ak715zIIE

    uint8_t cnt = 0;
    va_list args;
    char buffer[usartAvailableForWrite()]; //make a buffer for the string from va_list
    va_start(args, format);
    cnt = vsnprintf(buffer, usartAvailableForWrite(), format, args); //got this from https://www.educative.io/answers/what-is-vsnprintf-in-c
    va_end(args);

    if (cnt <= usartAvailableForWrite()) { //check if ringbuffer size is big enough for the string
        usartStringWrite(buffer);
    } else {
        return 0;
    }
    usartFlush(); //make sure all buffered data has been written to the interface
    return cnt;
}
uint8_t usartStringPrintln(const char* format, ...) //function is nearly the same like usartTringPrint
{
    uint8_t cnt = 0;
    uint8_t newlineBacklashAdd = 2; //need to append 2 characters /r/n
    va_list args;
    char buffer[usartAvailableForWrite()]; //make a buffer for the string from va_list
    if (usartAvailableForWrite() >= (strlen(format) + newlineBacklashAdd)) { //check if ringbuffer size is big enough for the string +2 for \r\n
        va_start(args, format);
        cnt = vsnprintf(buffer, usartAvailableForWrite(), format, args); //got this from https://www.educative.io/answers/what-is-vsnprintf-in-c
        usartStringWrite(buffer);
        cnt += usartWrite('\r');
        cnt += usartWrite('\n');
        va_end(args);
    } else {
        return 0;
    }
    usartFlush(); //make sure all buffered data has been written to the interface
    return cnt;
}
