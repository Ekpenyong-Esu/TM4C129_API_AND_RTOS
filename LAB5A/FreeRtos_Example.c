#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/buttons.h"
#include "drivers/pinout.h"
#include "FreeRTOS.h"
#include "driverlib/pin_map.h"
#include "task.h"
#include "semphr.h"

// The error routine that is called if the driver library
// encounters an error
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

SemaphoreHandle_t xSemLed1, xSemLed2, xSemLed3, xSemLed4, xSemUART;

char stringTime[10];

// All task Reference handle of the LEDS are set to zero.
TaskHandle_t xHandleBlink1 = 0;
TaskHandle_t xHandleBlink2 = 0;
TaskHandle_t xHandleBlink3 = 0;
TaskHandle_t xHandleBlink4 = 0;



//All Task reference Handle of the buttons pj0 and pj1 are set to 0;

TaskHandle_t xHandleButton1 = 0;
TaskHandle_t xHandleButton2 = 0;



// this function is use to configure UARTO so that string can be seen at the terminal.
void ConfigureUART(uint32_t system_clock, uint32_t baud_rate)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    UARTConfigSetExpClk(UART0_BASE, system_clock, baud_rate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}



// This RTOS UARTStringPut funcntion is use to send string through UART0.
void UARTStringPut(char *string)
{

//    This is use prevents race condition on the UART when it uses the Semaphore.
    while(!xSemaphoreTake(xSemUART, 0));


    while(*string)
    {
        UARTCharPut(UART0_BASE,*string++);
    }

    xSemaphoreGive(xSemUART);
}



//This is use to Light LED1 for it designated times.

void vStartLED1(void* pvInput){

    TickType_t xDelay = (uint32_t) 100/ portTICK_PERIOD_MS;    //1000 s delay

    TickType_t xLastWakeTime = xTaskGetTickCount(); //Used for xTask Delay Until

    while(1)
    {
        LEDWrite(1, 1); //Light up the LED
        UARTStringPut("\r\nLED1 ON");
        vTaskDelayUntil(&xLastWakeTime, xDelay);    //Wait one second
        LEDWrite(1, 0);
        UARTStringPut("\r\nLED1 OFF");
        vTaskDelayUntil(&xLastWakeTime, xDelay);    //Repeat
    }
}



// This is use to Light the LED2 for it designated times.
void vStartLED2(void* pvInput){

    TickType_t xDelay = (uint32_t) 200/ portTICK_PERIOD_MS;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        LEDWrite(2, 2);
        UARTStringPut("\r\nLED2 ON");
        vTaskDelayUntil(&xLastWakeTime, xDelay);
        LEDWrite(2, 0);
        UARTStringPut("\r\nLED2 OFF");
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}


//WE Light the LED3 for their designated times 3ms.
void vStartLED3(void* pvInput){

    TickType_t xDelay = (uint32_t) 300/ portTICK_PERIOD_MS;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        LEDWrite(4, 4);
        UARTStringPut("\r\nLED3 ON");
        vTaskDelayUntil(&xLastWakeTime, xDelay);
        LEDWrite(4, 0);
        UARTStringPut("\r\nLED3 OFF");
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}


//Lighting the LED4 for it designated time.
void vStartLED4(void* pvInput)
{
// declaring our loca vaiable for the LED4 function.
    TickType_t xDelay = (uint32_t) 400/ portTICK_PERIOD_MS;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1){
        LEDWrite(8, 8);
        UARTStringPut("\r\nLED4 ON");
        vTaskDelayUntil(&xLastWakeTime, xDelay);
        LEDWrite(8, 0);
        UARTStringPut("\r\nLED4 OFF");
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}



//This function Light  LED1 for 10ms when the left button is pressed.
void vLightLED1(void* pvInput){

    unsigned char ucDelta, ucState;

    TickType_t xDelay = 1000/ portTICK_PERIOD_MS;

    while(1){

        //WE Poll buttons
       ucState = ButtonsPoll(&ucDelta, 0);

       // If BUTTON PRESSED and is the Left(PJ0) button ON the LED1 for 10 milli seconds

       if(BUTTON_PRESSED(LEFT_BUTTON, ucState, ucDelta))
       {

           vTaskSuspend(xHandleBlink1);
           LEDWrite(1, 1);
           UARTStringPut("\r\nButton LED1 ON");
           vTaskDelay(xDelay);
           LEDWrite(1, 0);
           UARTStringPut("\r\nButton LED1 OFF");
       }

       vTaskResume(xHandleBlink1);
       vTaskDelay(49/ portTICK_PERIOD_MS);
   }
}


//This function Light The LED2 for 10s when The RIGHT button is pressed.

void vLightLED2(void* pvInput){

    unsigned char ucDelta, ucState;

    TickType_t xDelay = 1000/ portTICK_PERIOD_MS;

    while(1){
        // We Poll buttons
       ucState = ButtonsPoll(&ucDelta, 0);

       // If BUTTON PRESSED and is the RIGHT bUtton(PJ1) ON the LED2 for 10milli seconds

       if(BUTTON_PRESSED(RIGHT_BUTTON, ucState, ucDelta))
       {
           vTaskSuspend(xHandleBlink2);
           LEDWrite(2, 2);
           UARTStringPut("\r\nButton LED2 ON");
           vTaskDelay(xDelay);
           LEDWrite(2, 0);
           UARTStringPut("\r\nButton LED2 OFF");
       }

       vTaskResume(xHandleBlink2);
       vTaskDelay(50/ portTICK_PERIOD_MS);
   }
}



//this  Int handler function Will start the button  when they are pressed.
void ButtonIntHandler(void)
{
    if (GPIOIntStatus(GPIO_PORTJ_BASE, false) & GPIO_PIN_0)
    {
        xTaskResumeFromISR(xHandleButton1);
    }
    if (GPIOIntStatus(GPIO_PORTJ_BASE, false) & GPIO_PIN_1)
    {
        xTaskResumeFromISR(xHandleButton2);
    }
    // Clear interrupt flags and update the scheduler.
    GPIOIntClear(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    vTaskDelay(0/ portTICK_PERIOD_MS);
}



// The MAIN PROGRAMM.

int main(void)
{
// Enabling UART0 PERIPHERAL
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){}

    // To use the LEDs as output we need to set the pinout as false

    PinoutSet(false, false);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

    // we call the buttons function
    ButtonsInit();

    // Setting up interrupt for for our pins in PORTJ PIN_0 and PIN1
    GPIOIntDisable(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOIntClear(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOIntRegister(GPIO_PORTJ_BASE, ButtonIntHandler);
    GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    uint32_t systemClock;
    systemClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 16000);
    ConfigureUART(systemClock, 115200);


    //Setting up semaphore for UART
    xSemUART= xSemaphoreCreateMutex();


    //Create tasks
    xTaskCreate(vStartLED1, "LED1 Task", configMINIMAL_STACK_SIZE, (void*) 0, tskIDLE_PRIORITY + 1, &xHandleBlink1);
    xTaskCreate(vStartLED2, "LED2 Task", configMINIMAL_STACK_SIZE, (void*) 0, tskIDLE_PRIORITY + 1, &xHandleBlink2);
    xTaskCreate(vStartLED3, "LED3 Task", configMINIMAL_STACK_SIZE, (void*) 0, tskIDLE_PRIORITY + 1, &xHandleBlink3);
    xTaskCreate(vStartLED4, "LED4 Task", configMINIMAL_STACK_SIZE, (void*) 0, tskIDLE_PRIORITY + 1, &xHandleBlink4);
    xTaskCreate(vLightLED1, "KeepL1 Lit", configMINIMAL_STACK_SIZE, (void*) 0, tskIDLE_PRIORITY + 2, &xHandleButton1);
    xTaskCreate(vLightLED2, "KeepL2 Lit", configMINIMAL_STACK_SIZE, (void*) 0, tskIDLE_PRIORITY + 2, &xHandleButton2);


    //Handing the task to the Scheduler
    vTaskStartScheduler();
}
