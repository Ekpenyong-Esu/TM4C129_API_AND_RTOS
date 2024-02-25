
#include <drivers/Uart.h>
#include <stdint.h>

int main ()
{

    USART_reset();
    UART_Init();

    while(1)
    {
        //c = USART_getChar();
        //   USART_putChar(c);
        USART_putString("\r\nEnter string and press Enter\r\n");
        USART_getString(myString, 100);
        USART_putString("\r\nString entered is : ");
        USART_putString(myString);
        USART_putString("\r\nPress y to reset or N to continue ? \r\n");
        c = USART_getChar();
        USART_putChar(c);

        if (c == 'y')
        {
            USART_reset();
            UART_Init();
            USART_putString("\r\nUART Reset and Initialization done \r\n");
            USART_end();
        }

    }

}
