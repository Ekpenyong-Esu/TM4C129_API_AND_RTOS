/*
 * ButtonInitandLedInitdrivers.h
 *
 *  Created on: 25 Nov 2021
 *      Author: MAHONRI
 */

#ifndef DRIVERS_BUTTONINITANDLEDINITDRIVERS_H_
#define DRIVERS_BUTTONINITANDLEDINITDRIVERS_H_

#include <stdio.h>
#include <stdint.h>
#include <tm4c129encpdt.h>

#define delay_value   200000
#define GPIO_PIN_0              0x00000001  // GPIO pin 0
#define GPIO_PIN_1              0x00000002  // GPIO pin 1
#define GPIO_PIN_2              0x00000004  // GPIO pin 2
#define GPIO_PIN_3              0x00000008  // GPIO pin 3
#define GPIO_PIN_4              0x00000010  // GPIO pin 4

#define LED43                   (*((volatile uint32_t *)0x4005D3FC))
#define LED21                   (*((volatile uint32_t *)0x400643FC))

void Led_pin();
void buttons_init();
void delayMs(int n);





#endif /* DRIVERS_BUTTONINITANDLEDINITDRIVERS_H_ */
