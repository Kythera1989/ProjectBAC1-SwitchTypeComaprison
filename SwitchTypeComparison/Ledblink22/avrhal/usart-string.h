/**
 * 
 * @brief Additional string helper functions for the usart interface
 * @Authors: Venier/Winter
 */

#ifndef usart_string_h_oShWsTohZ22Tho2g
#define usart_string_h_oShWsTohZ22Tho2g

#include <stdbool.h>
#include <stdint.h>

/** Write a string to the usart interface.
 * @note The input string must be zero-terminated (`\0`). The trailing `\0` is not transmitted.
 * @param[in] str A zero-terminated string
 * @return The number of characters written
*/
uint8_t usartStringWrite(const char* str);

/** Print a formatted string to the usart interface.
 * @note Internally va_list(), va_start(), vsnprintf() and va_end() are used.
 * @param[in] format A printf-style format string
 * @return The number of characters written of the resulting string
*/
uint8_t usartStringPrint(const char* format, ...);

/** Print a formatted string to the usart and end the current line.
 * Therefore a trailing \r\n sequence is appended (also known as DOS/Windows line ending).
 * @note Internally va_list(), va_start(), vsnprintf() and va_end() are used.
 * @param[in] format A printf-style format string
 * @return The number of characters written of the resulting string
*/
uint8_t usartStringPrintln(const char* format, ...);

#endif
