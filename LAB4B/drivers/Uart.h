#include <stdint.h>
#include "tm4c129encpdt.h"


/*
 * Uart.h
 *
 *  Created on: 13 Oct 2021
 *      Author: EKPENYONG ESU EKPENYONG
 */

#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_

// Macro definitions





char c;
char myString[100];

void UART_Init();
void USART_reset();

char USART_getChar();
void USART_putChar(char c);
void USART_putString(char *string);
void USART_getString(char* ptr_string, int size);
void USART_end();



#endif /* DRIVERS_UART_H_ */
