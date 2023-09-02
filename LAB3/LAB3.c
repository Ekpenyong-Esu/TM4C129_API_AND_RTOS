
#include    <USART.h>

//Global variables are define here

// Here we declare and initialize variable that we will use for time
int hour = 23, min = 59, sec = 50;

// This variable will will save the value of string representing time
char str_time[256] = {'\0'};

// setting up our counter of the clock from zero
int counter = 0;

// This function initializes the timer and the interruption associated
void timer_init(){

    //Definition of the clock
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_OSC), 16000000);

    //here we enable the peripheral for timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //Wait for Timer0 to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}

    //Configure the TIMER0 as a half periodic count up timer
    TimerConfigure(TIMER0_BASE, (TIMER_CFG_PERIODIC_UP | TIMER_CFG_SPLIT_PAIR));

    //Set the load time, choosing a period of 200ms (Period = (Prescaler/Frec)*Load)
    TimerLoadSet(TIMER0_BASE, TIMER_A, 64000);
    TimerPrescaleSet(TIMER0_BASE, TIMER_A, 50);

    // Enable the interrupt for the timer
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //Register the interrupt handler function for the timer
    TimerIntRegister(TIMER0_BASE, TIMER_A, TimerIntHandler);

    // Enable the processor interrupt
    IntMasterEnable();

}

// This function start our stop watch and enable our timer to start counting
void start(){

    TimerEnable(TIMER0_BASE, TIMER_A);
}

// This function stops the  watch and disable the timer
void stop(){

    TimerDisable(TIMER0_BASE, TIMER_A);

}

// This function stops and reset the initial values of the stop watch and also disable timer
void reset(){


    TimerDisable(TIMER0_BASE, TIMER_A);

    //Reset to the initial values
    hour = 0;
    min = 0;
    sec = 0;

    // printing the value of hour, min, and sec in 2 decimal place
    UARTprintf("\r %02d : %02d : %02d ", hour, min, sec);
    USART_putString(str_time);

}

//This is the interrupt handler for the timer interrupt. It does the process of counting hours, minutes and seconds
void TimerIntHandler(void){


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


        // this function clear the cursor
        USART_putString("\x1b[0J");
        UARTprintf("\r %02d : %02d : %02d ", hour, min, sec);
        USART_putString(str_time);

        //Enable again the interruption
        IntMasterEnable();
    }
}
// our main program start here


int main(void){

    //Definition of variables
    char c;

    //Initialization our usart init function and timer init
    USART_init();
    timer_init();

    // we use this function to as users for inputt
    USART_putString("\r Write s to start the program ");

    // the we begin the almighty forever loop
    while(1){

        //Take the char written in the terminal from the user.
        c = USART_getChar();
        USART_putChar(c);
        USART_putChar('\r');


        //when user type s the timer starts.
        if (c == 's' || c== 'S'){
            start();
        }

        //when user type t we stop the timer(we use t here because s is occupied wit start)
        if (c == 't'|| c== 'T'){
            stop();
        }


        //when user type r the timer resets.
        if (c == 'r'|| c=='R'){
            reset();
        }

    }
}
