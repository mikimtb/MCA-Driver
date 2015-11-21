/* 
 * File:   main.c
 * Author: Miroslav
 *
 * Created on November 19, 2015, 11:22 PM
 */

#include "main.h"
#include "uart.h"
#include "uart.c"


void main()
{
    uart_init(2, 120); 
    enable_interrupts(GLOBAL);
    
    printf(uart_bputc, "\r\n\Running...\r\n");
    
    while (1)
    {
        //delay_ms(5000);
        //printf("\r\nBuffered data => \r\n");
        while (uart_bkbhit)
            uart_bputc(uart_bgetc());
    }
}

