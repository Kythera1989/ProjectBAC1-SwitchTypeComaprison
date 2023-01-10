/**
 * 
 * @brief Provides basic interrupt-based hardware usart access with internal buffering
 * @Authors: Venier/Winter
 */

#ifndef usart_h_zidaWSgah22o7ier
#define usart_h_zidaWSgah22o7ier

#include <stdbool.h>
#include <stdint.h>

typedef enum { USART_B9600 = 9600 } UsartBaudrate;
typedef enum { USART_CONFIG_8N1 } UsartConfig;

/** Setup and enable the usart interface
 * @param[in] baud The desired baudrate
 * @param[in] config The interface configuration (example: 8 data bits, no parity, 1 stop bit)
*/
void usartSetup(UsartBaudrate baud);

/** Get the number of bytes available in the receive buffer for reading.
 *
 * @return number of bytes
*/
uint8_t usartAvailableForRead();

/** Get the number of bytes which can be written to the transmit buffer.
 *
 * @return number of bytes
*/
uint8_t usartAvailableForWrite();

/** Read the next byte from the buffered incoming usart data without removing it.
 * @param[out] byte The next byte
 * @return True if a byte is available, false otherwise
*/
bool usartPeek(uint8_t* byte);

/** Read the next byte from the buffered incoming usart data.
 * @param[out] byte The next byte
 * @return True if a byte is available, false otherwise
*/
bool usartRead(uint8_t* byte);

/** Read multiple bytes from the buffered incoming usart data.
 * @param[out] bytes An array, at least of size maxRead
 * @param[in] maxRead The maximum number of bytes to read
 * @return The number of characters read
*/
uint8_t usartReadBytes(uint8_t* bytes, uint8_t maxRead);

/** Write a byte to the usart interface.
 * @param[in] byte The data byte to be transmitted
 * @return True if the byte was written to the transmit buffer, false otherwise
*/
bool usartWrite(uint8_t byte);

/** Waits until all buffered data has been written to the usart interface.
 *
 * @note Returns once the software-based transmit ring buffer is empty and the avr hardware
 *  'data register empty' flag is set.
*/
void usartFlush();

#endif
