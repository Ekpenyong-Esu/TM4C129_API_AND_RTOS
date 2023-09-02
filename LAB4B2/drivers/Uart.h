#include <stdint.h>
#include <tm4c129encpdt.h>


/*
 * Uart.h
 *
 *  Created on: 13 Oct 2021
 *      Author: EKPENYONG ESU EKPENYONG
 */

#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_

#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F


char c;
char myString[100];

void UART_Init();
void USART_reset();
char USART_getChar();
void USART_putChar(char c);
void USART_putString(char *string);
char USART_getString(char *string);
void USART_end();


#endif /* DRIVERS_UART_H_ */
