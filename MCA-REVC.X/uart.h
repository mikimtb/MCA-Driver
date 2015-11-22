/* 
 * File:   uart.h
 * Author: Miroslav
 *
 * Created on November 20, 2015, 12:17 AM
 */

#ifndef UART_H
#define	UART_H

void serial_rcv_isr(void);
void serial_td_isr(void);
BYTE uart_bgetc(void);
void uart_bputc(BYTE);
void uart_init(unsigned int, BYTE);
/* Uart parser state machine states */
void parse_uart_data(void); 
void wait_for_start(void);  // Waits for the # character
void wait_for_adr(void);    // Wait for device address
void parse_id(void);        // Reads the 3 char identifier, stores it in data_id
void parse_length(void);    // Reads [LENGTH] field, stores it in data_length
void parse_data(void);      // Reads the data into data_buffer[]
void parse_crc(void);       // Reads the crc checksum (and checks the data)
void parse_end(void);       // End of data packet (#) handling.. start all over.
short crc_check(void);      

#endif	/* UART_H */

