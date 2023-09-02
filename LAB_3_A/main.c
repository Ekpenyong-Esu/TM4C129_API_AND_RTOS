
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/tm4c129encpdt.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"

#include    <USART.h>

//Definition of global variables
int hour = 0, min = 0, sec = 0; //For keeping the time
char str_time[256] = {'\0'}; // Saves the value of the string representing time
int counter = 0; //Counter for the cycles of the clock (depends on the load value and prescaler)

// This function initializes the timer and the interruption associated
void timer_init(){

    //Definition of the clock
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_OSC), 16000000);

    //Enable Timer0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //Wait for Timer0 to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}

    //Configure the TIMER0 as a half periodic count up timer
    TimerConfigure(TIMER0_BASE, (TIMER_CFG_PERIODIC_UP | TIMER_CFG_SPLIT_PAIR));

    //Set the load time, choosing a period of 200ms (Period = (Prescaler/Frec)*Load)
    TimerLoadSet(TIMER0_BASE, TIMER_A, 64000); //max 65535
    TimerPrescaleSet(TIMER0_BASE, TIMER_A, 50); //Max 255

    // Enable the interrupt for the timer
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //Register the interrupt handler function for the timer
    TimerIntRegister(TIMER0_BASE, TIMER_A, TimerIntHandler);

    // Enable the processor interrupt
    IntMasterEnable();

}

// This function starts the stop watch
void start(){
    //Enable the timer to start counting
    TimerEnable(TIMER0_BASE, TIMER_A);
}

// This function stops the stop watch
void stop(){
    //Disable the timer, so the timer stops
    TimerDisable(TIMER0_BASE, TIMER_A);

}

// This function stops and reset the initial values of the stop watch
void reset(){

    //Disable the timer
    TimerDisable(TIMER0_BASE, TIMER_A);

    //Reset to the initial values
    hour = 0;
    min = 0;
    sec = 0;
   sprintf(str_time, "\r %02d : %02d : %02d ", hour, min, sec); //Shows 2 digits
    USART_putString(str_time);

}

//This is the interrupt handler for the timer interrupt. It does the process of counting hours, minutes and seconds
void TimerIntHandler(void){

    //As the period of the clock is 200ms, we need to wait 4 periods to enable the interruption so that it enables it every 1 second
    counter++;

    //Clear the timer interrupt
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    if (counter > 4){
        counter = 0;

        //Disables the interruption while making the calculations
        IntMasterDisable();
        sec++;

        if (sec > 59){
            sec = 0;
            min++;
        }
        if (min > 59){
            min = 0;
            sec = 0;
            hour++;
        }
        if (hour > 23){
            hour = 0;
            min = 0;
            sec = 0;
        }
        /*if(sec < 10 ){

        }*/
        USART_putString("\x1b[0J"); //Clear the screen from cursor to the end
        sprintf(str_time, "\r %02d : %02d : %02d ", hour, min, sec);
        USART_putString(str_time);

        //Enable again the interruption
        IntMasterEnable();
    }
}

int main(void){

    //Definition of variables
    char c;

    //Initialization
    USART_init();
    timer_init();

    //Program starts
    USART_putString("\r Write s to start the program ");

    // Loop forever while the timers run
    while(1){
        //Take the char written in the terminal
        c = USART_getChar();
        USART_putChar(c);
        USART_putChar('\r');
        //With character s we start the stop watch
        if (c == 's'){
            start();
        }
        //With character e we stop the stop watch
        if (c == 'e'){
            stop();
        }
        //With character r we reset the stop watch
        if (c == 'r'){
            reset();
        }

    }
}
