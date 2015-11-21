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

#endif	/* UART_H */

