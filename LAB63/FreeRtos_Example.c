#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "drivers/pinout.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// DECLARING ARRAYS FOR THE ADC
uint32_t ADC0S0Value[8];
uint32_t ADC0S1Value[8];
uint32_t ADC0S2Value[8];

//DECLARING SENSOR VALUES GLOBAL
int micValue;
int joystick[2];
//int joystickX, joystickY;

int acc[3];
//int accX, accY, accZ;


// DECLARING MESAGES QUEUE TO HOLD ALL THE VALUES FOR THE PERIPHERAL
QueueHandle_t xQueueMic = NULL;
QueueHandle_t xQueueJoy = NULL;

//QueueHandle_t xQueueJoyX = NULL;
//QueueHandle_t xQueueJoyY = NULL;

QueueHandle_t xQueueAcc = NULL;

//QueueHandle_t xQueueAccX = NULL;
//QueueHandle_t xQueueAccY = NULL;
//QueueHandle_t xQueueAccZ = NULL;



//INITIALIZING THE UART0
void ConfigureUART(uint32_t system_clock, uint32_t baud_rate)
{
    // ENABLING THE PERIPHERAL FOR UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // ENABLING THE PORTA PIN0 AND PIN1 AND PIN0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // PIN0 IN PORTA IS FOR RX
    GPIOPinConfigure(GPIO_PA0_U0RX);

    // PIN1 IN PORTA IS FOR TX
    GPIOPinConfigure(GPIO_PA1_U0TX);

    // SETTING THE SYSTEM CLOCK WITH BAUDRATE OF 115200 WITH ONE STOP BIT WITH NO PARITY BIT
    UARTConfigSetExpClk(UART0_BASE, system_clock, baud_rate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}


// WE USE THIS FUNCTION TO SEND STRING TO THE UART.
void UARTprintStr(char *str, bool continued)
{
    if(continued)
        UARTCharPutNonBlocking(UART0_BASE, '\r');
    while(*str)
        UARTCharPut(UART0_BASE,*str++);
}


// CONVERTING AN INTEGER VALUE TO STRING USING ATOI
void intToStr(int digits, char* string)
{
    bool isNegative = false;

    memset(string, 0, sizeof string);
    if(digits == 0)
    {
        string[0] = '0';
    }
    else{

        if(digits < 0)
        {
            digits = abs(digits);
            isNegative = true;
        }
        int tempDigits = digits;
        int i=0;

        while(tempDigits > 0)
        {
            i++;
            tempDigits /= 10;
        }

        if (isNegative) {
            i++;
        }

        string[i] = 0;
        while (i != 0)
        {
            if(isNegative && i==1)
            {
                string[--i]='-';
                break;
            }
            string[--i] = 48 + (digits % 10);
            digits /= 10;
        }
    }

}


// GATE KEEPER FUNCTION
// WE DISPLAY THE VALUE AS THE GATE KEEPER TASKS ALLOWS IT
void displayOutput(int micFinalVal, int joystickFinalVal[], int accFinalVal[])
{
    char buffer[16];

   // GATE KEEPER FOR MIC
    UARTprintStr("\e[1;1H\e[2J",false);
    intToStr(micFinalVal, buffer);
    UARTprintStr("Microphone: ", false);
    UARTprintStr(buffer, false);
    UARTprintStr("db", false);

     // GATEKEEPER FOR JOYSTIC
    UARTprintStr("\r\nJoystick: ", false);
    intToStr(joystickFinalVal[0], buffer);
    UARTprintStr(buffer, false);
    UARTprintStr(" , ", false);
    intToStr(joystickFinalVal[1], buffer);
    UARTprintStr(buffer, false);

    // GATEKEEPER FOR ACCELEROMETER
    UARTprintStr("\r\nAccelerometer: ", false);
    intToStr(accFinalVal[0], buffer);
    UARTprintStr(buffer, false);
    UARTprintStr(" , ", false);
    intToStr(accFinalVal[1], buffer);
    UARTprintStr(buffer, false);
    UARTprintStr(" , ", false);
    intToStr(accFinalVal[2], buffer);
    UARTprintStr(buffer, false);
}

// THIS TASKS GETS THE MICROPHONE SENSOR VALUE
void vTaskMicrophone(void *pvParameters)
{
    TickType_t xLastWakeTime;

    //    vTaskDelay(pdMS_TO_TICKS(0));

    while(1)
    {
        xLastWakeTime = xTaskGetTickCount();

        ADCProcessorTrigger(ADC0_BASE, 0);
        ADCSequenceDataGet(ADC0_BASE, 0, ADC0S0Value);

        micValue  = ADC0S0Value[0];

        xQueueSendToBack(xQueueMic,&micValue, portMAX_DELAY);

        vTaskDelayUntil(&xLastWakeTime, 5);
    }
}



// THIS TASKS GET THE JOYSTIC SENSOR VALUE
void vTaskJoystick(void *pvParameters)
{
    TickType_t xLastWakeTime;

    //    vTaskDelay(pdMS_TO_TICKS(2));

    while(1)
    {
        xLastWakeTime = xTaskGetTickCount();

        ADCProcessorTrigger(ADC0_BASE, 1);
        ADCSequenceDataGet(ADC0_BASE, 1, ADC0S1Value);

        //        joystick = ADC0S1Value;

        memcpy(joystick, ADC0S1Value, sizeof joystick);

        //        joystickX = ADC0S1Value[0];
        //        joystickY = ADC0S1Value[1];

        xQueueSendToBack(xQueueJoy, &joystick, portMAX_DELAY);

        vTaskDelayUntil(&xLastWakeTime, 10);
    }
}



// THIS TASKS GET THE ACCELEROMETER SENSOR VALUE
void vTaskAcc(void *pvParameters)
{
    TickType_t xLastWakeTime;

    //    vTaskDelay(pdMS_TO_TICKS(3));

    while(1) {
        xLastWakeTime = xTaskGetTickCount();

        ADCProcessorTrigger(ADC0_BASE, 2);
        ADCSequenceDataGet(ADC0_BASE, 2, ADC0S2Value);

        memcpy(acc, ADC0S2Value, sizeof acc);

        //        accX = ADC0S2Value[0];
        //        accY = ADC0S2Value[1];
        //        accZ = ADC0S2Value[2];

        xQueueSendToBack(xQueueAcc, &acc, portMAX_DELAY);
        vTaskDelayUntil(&xLastWakeTime, 20);
    }
}

// GATEKEEPER TASK GET ONE SENSOR VALUE AT A TIME
void vTaskGatekeeper(void *pvParameters)
{
    TickType_t xLastWakeTime;

    //    vTaskDelay(45/portTICK_PERIOD_MS);


    int micFinalVal;
    int joystickFinalVal[2];
    int accFinalVal[3];

    int micMappedVal;
    int joystickMappedVal[2];
    int accMappedVal[3];

    int i;

    while(1)
    {
        xLastWakeTime = xTaskGetTickCount();


        micFinalVal = 0;
        joystickFinalVal[0] = 0;
        joystickFinalVal[1] = 0;
        accFinalVal[0] = 0;
        accFinalVal[1] = 0;
        accFinalVal[2] = 0;


        for(i = 0; i < 8; i++)
        {
            xQueueReceive(xQueueMic, &micValue, portMAX_DELAY);
            micFinalVal = micFinalVal + micValue ;
        }
        micFinalVal = micFinalVal/i;


        for(i = 0; i < 4; i++)
        {
            xQueueReceive(xQueueJoy, &joystick, portMAX_DELAY);
            joystickFinalVal[0] = joystickFinalVal[0] + joystick[0];
            joystickFinalVal[1] = joystickFinalVal[1] + joystick[1];
        }
        joystickFinalVal[0] = joystickFinalVal[0] / i;
        joystickFinalVal[1] = joystickFinalVal[1] / i;

        for(i = 0; i < 2; i++)
        {
            xQueueReceive(xQueueAcc, &acc, portMAX_DELAY);
            accFinalVal[0] = accFinalVal[0] + acc[0];
            accFinalVal[1] = accFinalVal[0] + acc[1];
            accFinalVal[2] = accFinalVal[0] + acc[2];
        }
        accFinalVal[0] = accFinalVal[0] / i;
        accFinalVal[1] = accFinalVal[1] / i;
        accFinalVal[2] = accFinalVal[2] / i;


        // THIS FUNCTION SENDS THE AVERAGE VALUE TO THE DISPLAY
        displayOutput(micFinalVal, joystickFinalVal, accFinalVal);

        vTaskDelayUntil(&xLastWakeTime, (45));

    }
}


// THE MAIN PROGRAM
int main(void)
{

    // SETTING UP THE SYSTEM CLOCK WITH 120MHz
    uint32_t system_clock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    // INITIALIZING THE UART0 WITH A BAUDRATE OF 115200
    ConfigureUART(system_clock, 115200);

    // CONFIQURING THE ADC
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // SETTING THE PINOUTSETTINGS TO FALSE IN ORDER TO USE IT AS GPIO
    PinoutSet(false, false);

    // SETTING UP PORTE PIN7 TO PIN0 FOR ADC
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 |
                   GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0);

    //DISABLING THE ADC SEQUENCE FROM 0 TO 2 IN ORDER TO CONFIQURE IT
    ADCSequenceDisable(ADC0_BASE, 0);
    ADCSequenceDisable(ADC0_BASE, 1);
    ADCSequenceDisable(ADC0_BASE, 2);

     // CONFIGURING THE ADCs FOR MIC, JOYSTIC, AND ACCELEROMETER
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);

    //CONFIQURING THE ADC FOR MIC VALUE
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH8 | ADC_CTL_END |ADC_CTL_IE);

    //CONFIQURING THE ADC FOR JOYSTICKS (X AND Y AXIS)
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH9);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH0 | ADC_CTL_END |ADC_CTL_IE);


    //CONFIQURING THE ADC FOR ACCELEROMETER (X, Y AND Z AXIS)
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH3);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH1 | ADC_CTL_END |ADC_CTL_IE);

    ADCSequenceEnable(ADC0_BASE, 0);
    ADCSequenceEnable(ADC0_BASE, 1);
    ADCSequenceEnable(ADC0_BASE, 2);

    // CREATNG A BASETYPE QUEUE  TO HOLD THE MIC
    xQueueMic = xQueueCreate(8, 8);

    //CREATING A BASETYPE QUEUE TO HOLD THE MIC TWO VALUES X AND Y
    xQueueJoy = xQueueCreate(4, 16);
    //   xQueueJoyY = xQueueCreate(4, 8);

    // CREATING QUEUE TO HOLD THE ACCELEROMETER VALUE
    // create queue to hold accelerometer's 3 values (x,y,z)
    xQueueAcc = xQueueCreate(2, 24);
    //   xQueueAccY = xQueueCreate(2, 8);
    //   xQueueAccZ = xQueueCreate(2, 8);


    // SETTING UP THE GATE KEEPER TASK AND GIVING IT THE HIGHEST PRIORITY
    xTaskCreate(vTaskGatekeeper, "Gatekeep", 300, 0, 2, NULL);

    // SCHEDULING THE MICROPHONE, JOYSTICS, AND ACCELEROMETER GIVING THEM EQUALL PRIORITY
    xTaskCreate(vTaskMicrophone,"Microphone", 500, 0, 1, NULL);
    xTaskCreate(vTaskJoystick, "Joystick", 500, 0, 1, NULL);
    xTaskCreate(vTaskAcc, "Accelerometer", 500, 0, 1, NULL);


    // HANDLING THE TASKS TO THE SCHEDULLER
    vTaskStartScheduler();

    // THE PROGRAM WILL ONLY REACH HERE IF THE SCHEDULER IS NOT WORKING
    while (1)
    {
    }
}
