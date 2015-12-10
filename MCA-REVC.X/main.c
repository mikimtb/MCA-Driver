/* 
 * File:   main.c
 * Author: Miroslav
 *
 * Created on November 19, 2015, 11:22 PM
 */

#include "main.h"
#include "uart.h"
#include "uart.c"
#include "protocol.h"
#include "protocol.c"
#include "input.h"
#include "input.c"
#include "pwm.h"
#include "pwm.c"


void main()
{
    uart_init(2, 120); 
    pwm_init(13, 10, 12);
    //pwm_brake_release();
    //pwm_init_ccp1(256);
    enable_interrupts(GLOBAL);
    
    printf(uart_bputc, firmware);
    
    while (1)
    {
        //delay_ms(5000);
        //printf("\r\nBuffered data => \r\n");
        while (uart_bkbhit)
            parse_uart_data();
        if (NEW_MESSAGE_RECEIVE)
        {
            protocol_cmd_parse_request(data.ID);
            NEW_MESSAGE_RECEIVE = FALSE;
        }
        
    }
}

