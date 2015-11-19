/* 
 * File:   main.h
 * Author: Miroslav
 *
 * Created on November 19, 2015, 11:22 PM
 */

#ifndef main_H
#define main_H

#include <18F4431.h>

#device     ADC = 10 
#fuses      H4
#fuses      NOFCMEN
#fuses      NOIESO
#fuses      NOPUT
#fuses      NOBROWNOUT
#fuses      NOWDT
#fuses      NOWINEN
#fuses      NOPWMPIN
#fuses      HPOL_HIGH
#fuses      LPOL_HIGH
#fuses      T1LOWPOWER
#fuses      FLTAD4
#fuses      SSP_RC
#fuses      NOPWMPIN
#fuses      MCLR
#fuses      NOLVP
#fuses      NODEBUG

#define     RX_PIN      PIN_C7
#define     TX_PIN      PIN_C6
#define     RS485_RW    PIN_C0

#use delay(clock = 40MHZ)
#use rs232(xmit=TX_PIN, rcv=RX_PIN, enable=RS485_RW)
 
// UART Registers and bits
#byte TXSTA=0xFAC
#bit  BRGH=TXSTA.2
#bit  SYNC=TXSTA.4

#byte RCSTA=0xFAB
#bit  ADDEN=RCSTA.3
#bit  RX9=RCSTA.6

#byte BAUDCTL=0xFAA
#bit  BRG16=BAUDCTL.3

int16 SPBRG; 
#locate SPBRG=0xFAF


#endif


