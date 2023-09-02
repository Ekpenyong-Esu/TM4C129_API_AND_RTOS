#include <drivers/ButtonInitandLedInitdrivers.h>


int main (){

    Led_pin();
    buttons_init();

    // Forever loop
    while(1){

       if (!(GPIO_PORTJ_AHB_DATA_R & GPIO_PIN_0 )){


            LED21 = GPIO_PIN_1;  LED43 = GPIO_PIN_4;

            LED21 = GPIO_PIN_0;   LED43 = GPIO_PIN_0;



        }

       else{

            LED21 = 0;    LED43 = 0;

            LED21 = 0;    LED43 = 0;


       }


    }



}
