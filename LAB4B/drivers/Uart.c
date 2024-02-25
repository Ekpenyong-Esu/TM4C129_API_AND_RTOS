/*
 * Uart.c
 *
 *  Created on: 13 Oct 2021
 *      Author: EKPENYONG ESU EKPENYONG
 */
#include <tm4c129encpdt.h>
#include <stdint.h>
#include "Uart.h"

void UART_Init(void)
{


  // Activate the UART clock.
  SYSCTL_RCGCUART_R |=SYSCTL_RCGCUART_R0;

  // Activate the PORTA clock.
  SYSCTL_RCGCGPIO_R |=SYSCTL_RCGCGPIO_R0;

  // allow time for clock to stabilize
  while((SYSCTL_RCGCUART_R &SYSCTL_RCGCUART_R0) == 0){}


  // enable alt funct on PA1-0
    GPIO_PORTA_AHB_AFSEL_R = (1<<0)|(1<<1);


   // configure PA1-0 as UART
    GPIO_PORTA_AHB_PCTL_R = (1<<0)|(1<<4);


    // enable Digital funct on PA1-0
    GPIO_PORTA_AHB_DEN_R = (1<<0)|(1<<1);


  // disable UART
    UART0_CTL_R = ~(1<<0);

  // IBRD = int(16,000,000 / (16 * 9600)) = 104.016666667
  // FBRD = round(0.16667 * 64 + 0.5) = 11.
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;



  // 8 bit word length (no parity bits, one stop bit, FIFOs)
   UART0_LCRH_R =(0x03<<5)|(1<<4);


  // UART gets its clock from the system clock
  UART0_CC_R = 0X0;


  // Enable UART
  UART0_CTL_R =(1<<0)|(1<<8)|(1<<9);


  // allow time for clock to stabilize
  //while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R0) == 0){};

  // disable analog functionality on PA0 and pA1
  //GPIO_PORTA_AHB_AMSEL_R &= ~0x03;
  //while(!(SYSCTL_PRUART_R & SYSCTL_PRUART_R0)){}
}


//This function is used to receive a character
char USART_getChar(){
    char c; //Character to be read
    while((UART0_FR_R&(UART_FR_RXFE)) != 0); //When the value is 1, there is no data to received

    //When the value is 0, there is data to received
    c = UART0_DR_R;
    return c;
}

//This function is used to transmit a character
void USART_putChar(char c){
    while((UART0_FR_R&(UART_FR_TXFF)) != 0); //When the value is 1,can't transmit

    //When the value is 0, you can transmit the character
    UART0_DR_R = c;
}


//This function is used to send a string
void USART_putString(char *string){
    while (*string!='\0'){
   //Transmits the string separated in characters
        USART_putChar(*(string++));
    }
}


void USART_reset(){
        SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;

        UART0_CTL_R &= ~(UART_CTL_UARTEN | UART_CTL_TXE | UART_CTL_RXE);// Disable UART0.
        UART0_CC_R &= ~(UART_CC_CS_PIOSC);                              // Unassign PIOSC from UART0.
        UART0_LCRH_R &= ~(UART_LCRH_WLEN_M);                            // word length reset.
        UART0_IBRD_R &= ~(0xFFFF);                                      // Clear int part of BRD.
        UART0_FBRD_R &= ~(0x3F);                                        // Clear fractional part of BRD.
        GPIO_PORTA_AHB_DEN_R &= ~(1<<0)|(1<<1);             // Disable PA0 and PA1.
        GPIO_PORTA_AHB_PCTL_R &= ~(0x11);                               // Clear PA0 and PA1.
        GPIO_PORTA_AHB_AFSEL_R &= ~(1<<0)|(1<<1);
        SYSCTL_RCGCGPIO_R &= ~(SYSCTL_RCGCGPIO_R0);                     // Disable clock for port A.
        SYSCTL_RCGCUART_R &= ~(SYSCTL_RCGCUART_R0);                     // Disable clock for UART0.

}

//This function is used to receive a string
    void USART_getString(char* ptr_string, int size)
    {
        char c = 0;
        int i = 0;

        //while string char is not '\n' string is not finished
        while (c != 13 && i <= size)
        {
            // Add char every time and goto next char
            c = USART_getChar();
            USART_putChar(c);
            ptr_string[i] = c;
            i++;
        }

        ptr_string[i] = 0;
    }



void USART_end() {
    SYSCTL_RCGCUART_R &= ~(SYSCTL_RCGCUART_R0);
    SYSCTL_RCGCGPIO_R &= ~(SYSCTL_RCGCGPIO_R0);
    UART0_CTL_R &= ~UART_CTL_UARTEN;
}

