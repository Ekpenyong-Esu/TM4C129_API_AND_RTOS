/* DISPLAYING SEVEN SEGMENT USING PUSH BUTTON8*/

#include <stdio.h>
#include <stdint.h>
#include <tm4c129encpdt.h>


#define  GPIO_PIN_1              0x00000002
#define  GPIO_PIN_0              0x00000001
volatile unsigned uLoop = 20000;
unsigned int      i,j;


void led1init();


/* we are displaying only number 14 ( 1 and four in the seven segment)*/


#define SYSTEM_CLOCK_FEQ        25000000
#define DELAY_VALUE             SYSTEM_CLOCK_FEQ/25
#define DELAY_REBOUNCE          SYSTEM_CLOCK_FEQ/1000


int main  (){


    unsigned char Seven_seg_pattern[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

    // enable portA and portB clock so that we can make use of it
    SYSCTL_RCGCGPIO_R |= (SYSCTL_RCGCGPIO_R8|SYSCTL_RCGCGPIO_R1);

    // allow the clock to settle
    while (!(SYSCTL_RCGCGPIO_R & (SYSCTL_RCGCGPIO_R8|SYSCTL_RCGCGPIO_R1))){};


    // we sett port b pins as output from 0 to 7 so that we can use it for the segment
    GPIO_PORTB_AHB_DIR_R |= (0xFF);  // you can also use (255<<0) left shift and start from zero

    // enable the digital enable register so that we can write to it
    GPIO_PORTB_AHB_DEN_R |= (255<<0);

    // setting up the push buttons in port j and pin 0

    // setting the gpio of portj as input
    GPIO_PORTJ_AHB_DIR_R &= ~(GPIO_PIN_0);


    // enabble the gpioden of portj
    GPIO_PORTJ_AHB_DEN_R = (GPIO_PIN_0);


    // setting the alternate function sellect of pin pj0 and pj1 to 0;

    GPIO_PORTJ_AHB_AFSEL_R &= ~(GPIO_PIN_0);

    // set the pull up resistors of pj0 and pj1 to high = 1

    GPIO_PORTJ_AHB_PUR_R |= (GPIO_PIN_0);





    // setting up the clock of gpio port n
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;


    // allow the clock to stabillize
    while(!( SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R12)){};

    // enabling the gpio port n as output by setting the gpiodir to 1
    GPIO_PORTN_DIR_R |= (GPIO_PIN_1);

    // enable the digital enable of portn so tha we can use it digital function
    GPIO_PORTN_DEN_R |= (GPIO_PIN_1);



    // setting the port to the default value
    GPIO_PORTB_AHB_DATA_R = 0x00;




    // entering the forever loop

    while (1){

        // if button is off (note tha the buttons are active high)
        if ((GPIO_PORTJ_AHB_DATA_R & GPIO_PIN_0 )==0x0){


            // calling the led  for debugging
            GPIO_PORTN_DATA_R |= GPIO_PIN_1;


            i++;


            // we now delay for 20Ms
            for( j=0; j<DELAY_REBOUNCE; ++j){};

            // when i is greater than 9, return i to 0
            if (i>9){
                i=0;
            }
        }

        else{

            GPIO_PORTN_DATA_R |= ~GPIO_PIN_1;

        }
        // update the display of the seven segment
        GPIO_PORTB_AHB_DATA_R |= Seven_seg_pattern[i];



        // calling the led function for debugging
        //led1init();

        // delay again for 20Ms
        for( j=0; j<uLoop; ++j){};


    }

}

/*void led1init(){

     // setting up the clock of gpio port n
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;


    // allow the clock to stabillize
    while(!( SYSCTL_RCGCGPIO_R & SYSCTL_RCGCGPIO_R12)){};

   // enabling the gpio port n as output by setting the gpiodir to 1
    GPIO_PORTN_DIR_R |= (GPIO_PIN_1);

  // enable the digital enable of portn so tha we can use it digital function
     GPIO_PORTN_DEN_R |= (GPIO_PIN_1);

     // entering forever loop for led D1
    // while(1){

         // write to the gpio data
         // lit the D1 = gpio portn 1
          GPIO_PORTN_DATA_R |= GPIO_PIN_1;

          // delay for 5MS
          for( i=0; i<10; ++i){};


    // }



}
 */
