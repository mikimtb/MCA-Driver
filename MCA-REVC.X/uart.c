/*
 * File:   uart.c
 * Author: Miroslav
 *
 * Created on November 19, 2015, 11:22 PM
 */

#include "uart.h"

//Defines
#define BUFFER_SIZE 32
#define uart_bkbhit (next_in!=next_out)
//Variables
BYTE uart_buffer[BUFFER_SIZE] = {0};
BYTE next_in = 0;
BYTE next_out = 0;

#int_rda
void serial_isr()
{
    int t;
    
    uart_buffer[next_in] = getc();
    t = next_in;
    next_in = (next_in + 1) % BUFFER_SIZE;
    if (next_in == next_out)
        next_in = t;                                // Buffer full
}
/**
 * uart_bgetc, Function return one byte from uart circular buffer
 * @return , first buffer that is written to the buffer
 */
BYTE uart_bgetc()
{
    BYTE c;
    
    //while(!uart_bkbhit);                          // This should be checked
                                                    // in parser before state
                                                    //machine is called
    c = uart_buffer[next_out];
    next_out = (next_out + 1) % BUFFER_SIZE;
    return (c);
}
/**
 * uart_init, Function configure uart peripheral
 * @param baudrate, Possible baudrates are: 1 - 19200, 2 - 115200, 3 - 256000
 * @param dev_id, Device address used in RS485 communication
 */
void uart_init(unsigned int baudrate, BYTE dev_id)
{
    unsigned long divisor;
    
    switch (baudrate)
    {
        case 1:
            //setup_uart(19200);
            divisor = 520;
            break;
        case 2:
            //setup_uart(115200);
            divisor = 85;
            break;
        case 3:
            //setup_uart(256000);
            divisor = 38;
            break;
    }
    
    // Switch to 16bit mode
    SYNC = 0;
    BRGH = 1;
    BRG16 = 1;
    // Select 9-bit reception
    //RX9 = 0;
    // Disable uart address detecion on receive
    //ADDEN = 0;
    // Configure uart speed
    SPBRG = divisor;
    //setup_uart(UART_ADDRESS);
    enable_interrupts(INT_RDA);
}