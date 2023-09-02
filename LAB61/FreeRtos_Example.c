#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "drivers/pinout.h"
#include "FreeRTOS.h"
#include "task.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "semphr.h"


int stockCount = 0;
const int stockSize = 10;
char buffer[11];
uint32_t system_clock;

SemaphoreHandle_t xSemUART = 0;
SemaphoreHandle_t xSemCount = 0;

TaskHandle_t xHandleTaskProd = 0;
TaskHandle_t xHandleTaskCons = 0;

// This BLOCK IS USE TO INIALIZE THE UART0
void ConfigureUART(uint32_t system_clock, uint32_t baud_rate)
{
    // ENABLE PERIPHERAL FOR UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // ENABLE PORT A FOR UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // ENABLE PIN0 AND PIN1 IN PORTA FOR TX AND RX
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    UARTConfigSetExpClk(UART0_BASE, system_clock, baud_rate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}



// THIS BLOCK IS USE TO SEND UART THROUGH THE TERMINAL
void UARTprintStr(char *str, bool continued)
{
    if(continued)
        UARTCharPutNonBlocking(UART0_BASE, '\r');
    while(*str)
        UARTCharPut(UART0_BASE,*str++);
}


// THIS BLOCK IS USED TO PRODUCE AN ITEM TO FILL THE BUFFER

void TaskProducer(void* pvParameters)
{
   // COUNTING SEMAPHORE HERE ENSURES THAT THE PRODUCER WILL NOT PRODUCE AND ITEM WHEN THE BUFFER IS FULL
    while(1)
    {
        // IF THERE IS NO THIS WHILE SEMARPHORE GIVE, THE BUFFER WILL OVERFLOW AND LOST INFORMATION.
      while(!xSemaphoreGive(xSemCount)){};    //HENCE COMENTING OR REMOVING THIS LINE WILL SHOW PRODUCER PROBLEM
        if (stockCount != stockSize)
        {
            buffer[stockCount++] = '1';

            while(!xSemaphoreTake(xSemUART, 0)){};
            UARTprintStr(buffer,1);
            xSemaphoreGive(xSemUART);
        }
        else
        {
            while(!xSemaphoreTake(xSemUART, 0)){};
            UARTprintStr(buffer,1);
            UARTprintStr(" Production Error!!!\r",0);
            xSemaphoreGive(xSemUART);
        }
        vTaskDelay(200);
    }
}


// THIS BLOCK CONSUMES THE AN ITEM AND UNLOAD THE BUFFER


void TaskConsumer(void* pvParameters)
{

    //  THE COUNTING SEMAPHORE MAKES SURE THAT THE CONSUMER WILL NOT CONSUME AN ITEM WHEN THE BUFFER IS EMPTY
    while(1)
    {
       while(!xSemaphoreTake(xSemCount, 0)){}; //HENCE COMENTING THIS LINE WILL GIVE RISE TO CONSUMER PROBLEM
        if (stockCount != 0)
        {
            buffer[--stockCount] = '0';

            while(!xSemaphoreTake(xSemUART, 0)){};
            UARTprintStr(buffer,1);
            xSemaphoreGive(xSemUART);
        }
        else
        {
            while(!xSemaphoreTake(xSemUART, 0)){};
            UARTprintStr(buffer,1);
            UARTprintStr(" Consumer Error!!!\r",0);
            xSemaphoreGive(xSemUART);
        }
        vTaskDelay(250);
    }
}

// Main Function.
int main(void)
{
    // SETTING UP THE SYSTEM CLOCK
    system_clock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                                 SYSCTL_OSC_MAIN |
                                                 SYSCTL_USE_PLL |
                                                 SYSCTL_CFG_VCO_480), 120000000);
    // InitializING THE UART0 BAUDRATE OF 9600
    ConfigureUART(system_clock, 9600);

    int i;
    for(i=0; i<10; i++)
        buffer[i]= '0';

    buffer[i]= 0;


    // WE SET UP OUR SEMAPHORE HERE

    // WE USE SEMUART TO CONTROLL ACCESS TO UART0.
    xSemUART = xSemaphoreCreateMutex();

    // WE USE SEMCOUNT TO ACCESS AND CONTROL THE QUEUE
    xSemCount = xSemaphoreCreateCounting( stockSize, 0 );

    //WE CREATE TASK
    xTaskCreate(TaskProducer, "Task Producer", 1000, 0, 1, &xHandleTaskProd);
    xTaskCreate(TaskConsumer, "Task Consumer", 1000, 0, 1, &xHandleTaskCons);

    // HAND OVER TH PROCESS TO THE SCHEDULER
    vTaskStartScheduler();
}
