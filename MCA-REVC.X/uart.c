/******************************************************************************/
/* File:   uart.c                                                             */
/* Author: Miroslav Bozic                                                     */
/*                                                                            */
/* Library for UART receive and transmit data using custom protocol           */                          
/* Data package structure:                                                    */
/*   # [DEV_ADR] [MESSAGE_ID] [LENGTH] [DATA<0>,...DATA<LENGTH-1>] [CRC] $    */
/* User parameters:                                                           */
/*  UART_RX_TIMEOUT - UART RX timeout defined in ms (max 5ms)                 */
/*  UART_BUFFER_SIZE - UART circular RX and TX buffers size                   */
/*  DATA_BUFFER_SIZE - DATA buffer size                                       */
/*                                                                            */
/* User functions:                                                            */
/* void uart_init(unsigned int baudrate, BYTE dev_id) - Must be called before */
/* other function in library                                                  */
/*                                                                            */
/* void parse_uart_data() - call in while endless loop to start FSM parser    */
/*                                                                            */
/******************************************************************************/

#include "uart.h"

//Defines
#define UART_RX_TIMEOUT     1                   // Timeout in mS
#define TIMER1_REFRESH      (long)(65536 - (UART_RX_TIMEOUT * 10000))
#define UART_BUFFER_SIZE    64
#define DATA_BUFFER_SIZE    32    
#define uart_bkbhit (in.next_in!=in.next_out)

// Type definitions
typedef struct buffer
{
    BYTE uart_buffer[UART_BUFFER_SIZE];
    BYTE next_in;
    BYTE next_out;
} t_buffer;

typedef struct package
{
    BYTE data_buffer[DATA_BUFFER_SIZE];
    BYTE dev_address;
    BYTE ID;
    BYTE data_length;
    BYTE crc;
} t_package;

typedef void (*t_fptr)(void);
//Variables
//UART
t_buffer in = {{0}, 0, 0};
t_buffer out = {{0}, 0, 0};
//Message parser
t_package data = {{0}, 0, 0, 0};

//Pointer to a function that takes no parameters and returns nothing.
t_fptr parse_next = wait_for_start;          // Initialize the pointer to point to first state
int count = 0;                               // Variable used in parse_data function
// Functions
/**
 * UART receive interrupt handler
 */
#int_rda
void serial_rcv_isr()
{
    int t;
    
    in.uart_buffer[in.next_in] = getc();
    t = in.next_in;
    in.next_in = (in.next_in + 1) % UART_BUFFER_SIZE;
    if (in.next_in == in.next_out)
        in.next_in = t;                                // Buffer full
}
/**
 * UART transmit interrupt handler
 */
#int_tbe
void serial_td_isr()
{
    if (out.next_in != out.next_out)
    {
        putc(out.uart_buffer[out.next_out]);
        out.next_out = (out.next_out + 1) % UART_BUFFER_SIZE;
    }
    else
        disable_interrupts(int_tbe);
}
/**
 * TIMER1 overflow interrupt handler
 */
#int_timer1
void timer1_overflow_isr()
{
    parse_next = wait_for_start;
    disable_interrupts(int_timer1);
    
    /* Request for retransmission should be added here if it's needed */
    /* printf(uart_bputc, "Timeout occurred!\r\n");                  */
}
/**
 * uart_bgetc, Function return one byte from uart input buffer
 * @return , first buffer that is written to the uart input buffer.
 */
BYTE uart_bgetc()
{
    BYTE c;
    
    c = in.uart_buffer[in.next_out];
    in.next_out = (in.next_out + 1) % UART_BUFFER_SIZE;
    return (c);
}
/**
 * bputc, Function add one byte to uart output buffer
 * @param c, byte that will be added in uart output buffer.
 */
void uart_bputc(BYTE c)
{
    short restart;
    int ni;
    
    restart = out.next_in == out.next_out;
    out.uart_buffer[out.next_in] = c;
    ni = (out.next_in + 1) % UART_BUFFER_SIZE;
    while(ni == out.next_out);
    out.next_in = ni;
    if (restart)
        enable_interrupts(int_tbe);
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
    
    // Timer 1 is used as timeout generator
    // The time is defined using UART_RX_TIMEOUT
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
    
    enable_interrupts(INT_RDA);
}
/**
 * Function that call Finite State Machine parser
 */
void parse_uart_data()
{
    (*parse_next)();
}
/**
 * Function wait and check start character of a package
 */
void wait_for_start()
{
    //* If there is no data available, return */
    if (!uart_bkbhit)
        return;
    
    /* If the data is start character, we update the state */
    if (uart_bgetc() == '#')
    {
        /* Initialize data variables, just in case */
        data.dev_address = 0;
        data.ID = 0;
        data.data_length = 0;
        data.crc = 0;
        count = 0;
        parse_next = wait_for_adr; // Next we parse the [ID] field
        
        set_timer1(TIMER1_REFRESH);
        clear_interrupt(int_timer1);
        enable_interrupts(int_timer1);
    }
}
/**
 * Function receive address of a device that should response on message
 */
void wait_for_adr()
{
    //* If there is no data available, return */
    if (!uart_bkbhit)
        return;
    
    data.dev_address = uart_bgetc();
    set_timer1(TIMER1_REFRESH);
    parse_next = parse_id;
    
}
/**
 * Function receive package ID identifier
 */
void parse_id()
{
    //* If there is no data available, return */
    if (!uart_bkbhit)
        return;
    
    data.ID = uart_bgetc();
    set_timer1(TIMER1_REFRESH);
    parse_next = parse_length;
}
/**
 * Function receive data length of a package
 */
void parse_length()
{
    //* If there is no data available, return */
    if (!uart_bkbhit)
        return;
    
    data.data_length = uart_bgetc();
    set_timer1(TIMER1_REFRESH);
    parse_next = parse_data;
}
/**
 * Function receive data part of a package
 */
void parse_data()
{
    //* If there is no data available, return */
    if (!uart_bkbhit)
        return;
    /* Add received byte to the data_length variable.
       The bytes arrive in Big Endian order. */
    data.data_buffer[count] = uart_bgetc();
    count++;
    set_timer1(TIMER1_REFRESH);
    /* State transition rule */
    if (count == data.data_length)
    {
        count = 0;  // Reset counter
        parse_next = parse_crc;
    }
}
/**
 * Function receive and check CRC of a package
 */
void parse_crc()
{
    //* If there is no data available, return */
    if (!uart_bkbhit)
        return;
    
    data.crc = uart_bgetc();
    set_timer1(TIMER1_REFRESH);
    // Check CRC
    if (!crc_check())
    {
        parse_next = wait_for_start;
        /* Request for retransmission should be added here if it's needed */
        /* printf(uart_bputc, "CRC failed!\r\n");                        */
        return;
    }
    parse_next = parse_end;
}
/**
 * Function receive and check the last one character in a package
 */
void parse_end()
{
    //* If there is no data available, return */
    if (!uart_bkbhit)
        return;
    
    disable_interrupts(int_timer1);
    // If last byte isn't stop character, something goes wrong in transmission
    // and package will be rejected
    if (uart_bgetc() != '$')
    {
        parse_next = wait_for_start;
        /* Request for retransmission should be added here if it's needed */
        /* printf(uart_bputc, "STOP character reception failed!\r\n");    */
        return;
    }
    parse_next = wait_for_start;
    // Handle data
    printf(uart_bputc, "Message receive done.\r\n");
}
/**
 * Calculate and check CRC8 for received message
 * @return TRUE if CRC check is valid or FALSE if CRC check is failed
 */
short crc_check()
{
    int i;
    int crc8 = 0;
    crc8 = crc8 ^ data.dev_address ^ data.ID ^ data.data_length;
    for(i=0; i<data.data_length; i++)
        crc8 = crc8 ^ data.data_buffer[i];
    
    return crc8 == data.crc;
}
