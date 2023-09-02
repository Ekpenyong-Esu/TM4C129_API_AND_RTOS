
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
char stock[11];
uint32_t system_clock;

SemaphoreHandle_t xSemUART = 0;
SemaphoreHandle_t xSemCount = 0;

TaskHandle_t xHandleTaskProd = 0;
TaskHandle_t xHandleTaskCons = 0;

// Initialize the UART0
void ConfigureUART(uint32_t system_clock, uint32_t baud_rate)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    UARTConfigSetExpClk(UART0_BASE, system_clock, baud_rate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

// Send a String trough UART0.
void UARTprintStr(char *str, bool continued)
{
    if(continued)
        UARTCharPutNonBlocking(UART0_BASE, '\r');
    while(*str)
        UARTCharPut(UART0_BASE,*str++);
}


//this task produces an item and fill up the stock. counting semaphore ensures that this will not produce item when warehouse is full
void TaskProducer(void* pvParameters)
{
    while(1)
    {
        while(!xSemaphoreGive(xSemCount)){};    //comment this line to observe producer consumer problem
        if (stockCount != stockSize)
        {
            stock[stockCount++] = '1';

            while(!xSemaphoreTake(xSemUART, 0)){};
            UARTprintStr(stock,1);
            xSemaphoreGive(xSemUART);
        }
        else
        {
            while(!xSemaphoreTake(xSemUART, 0)){};
            UARTprintStr(stock,1);
            UARTprintStr(" Production Error!!!\r",0);
            xSemaphoreGive(xSemUART);
        }
        vTaskDelay(200);
    }
}


//this task consumes an item and unload the stock. counting semaphore ensures that this will not try to consume an item when warehouse is empty
void TaskConsumer(void* pvParameters)
{
    while(1)
    {
        while(!xSemaphoreTake(xSemCount, 0)){}; //comment this line to observe producer consumer problem
        if (stockCount != 0)
        {
            stock[--stockCount] = '0';

            while(!xSemaphoreTake(xSemUART, 0)){};
            UARTprintStr(stock,1);
            xSemaphoreGive(xSemUART);
        }
        else
        {
            while(!xSemaphoreTake(xSemUART, 0)){};
            UARTprintStr(stock,1);
            UARTprintStr(" Consumer Error!!!\r",0);
            xSemaphoreGive(xSemUART);
        }
        vTaskDelay(250);
    }
}

// Main Function.
int main(void)
{
    system_clock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                                 SYSCTL_OSC_MAIN |
                                                 SYSCTL_USE_PLL |
                                                 SYSCTL_CFG_VCO_480), 120000000);
    // Initialize UART0.
    ConfigureUART(system_clock, 9600);

    int i;
    for(i=0; i<10; i++)
        stock[i]= '0';

    stock[i]= 0;


    // Semaphores.
    xSemUART = xSemaphoreCreateMutex();               // Controls access to UART0.
    xSemCount = xSemaphoreCreateCounting( stockSize, 0 );    // Controls access to the queue.

    // Create the tasks.
    xTaskCreate(TaskProducer, "Task Producer", 1000, 0, 1, &xHandleTaskProd);
    xTaskCreate(TaskConsumer, "Task Consumer", 1000, 0, 1, &xHandleTaskCons);

    // Start the scheduler.
    vTaskStartScheduler();
}
