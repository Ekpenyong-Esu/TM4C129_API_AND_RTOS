
#include <drivers/Uart.h>
#include <stdint.h>

int main ()
{

    USART_reset();
    // Initialising the uart function.
    UART_Init();

    // Starting our forever loop
    while(1)
    {
        USART_putString("\r\nEnter string and press Enter: ");
        USART_getString(myString);
        USART_putString("\r\nString entered is : ");
        USART_putString(myString);
        USART_putString("\r\nPress ESC to exit or No to continue: \r\n");
        c = USART_getChar();
        //USART_putChar(c);
        //  USART_putChar('\r');

        if (c == 0x1b)
        {
            USART_putString("\r\nUART Reset and Initialisation done \r\n");
            USART_reset();
            //UART_Init();
        }
    }


}
