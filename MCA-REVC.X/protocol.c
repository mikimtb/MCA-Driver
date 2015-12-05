/******************************************************************************/
/* File:   protocol.c                                                         */
/* Author: Miroslav Bozic                                                     */
/*                                                                            */
/* High level definition of a protocol messages and message parsing           */
/******************************************************************************/

#include "protocol.h"
#include "uart.h"

// Type definitions
typedef void (*t_cmdHdlr)(void);

typedef enum
{
    CMD_WAKE_UP = 0,
    CMD_GET_FIRMWARE_VERSION,
    CMD_SET_ADDRESS,
    CMD_SET_DRV_ENABLE
} e_cmdID;

typedef struct cmd_info
{
    e_cmdID cmd_id;             // Command ID
    t_cmdHdlr   cmd_hdlr;       // Command handler
} t_cmd_info;

// Variables
t_cmd_info cmd_hdlrs[] = 
{
    {CMD_WAKE_UP, wake_up_hdlr},
    {CMD_GET_FIRMWARE_VERSION, get_firmware_version_hdlr},
    {CMD_SET_ADDRESS, set_address_hdlr},
    {CMD_SET_DRV_ENABLE, set_drv_enable_hdlr}
};

// Functions
int protocol_cmd_parse_request(BYTE msg_ID)
{
    int i=0;
    // Get cmd_hdlrs_array length
    int cmd_hdlrs_count = sizeof(cmd_hdlrs) / sizeof(cmd_hdlrs[0]);
    // Find handler for given message ID
    while(i < cmd_hdlrs_count && cmd_hdlrs[i].cmd_id != msg_ID)
        i++;
    // If there is no proper handler return error code -1
    if (i == cmd_hdlrs_count)
        return -1;
    // Callback calling
    //printf(uart_bputc, "Callback calling.\r\n");
    (*cmd_hdlrs[i].cmd_hdlr)();
    
    return 0;
}
/**
 * Return driver presentation on communication bus
 */
void wake_up_hdlr()
{
    printf(uart_bputc, "Wakeup handler called.\r\n");
}
/**
 * Return firmware version
 */
void get_firmware_version_hdlr()
{
    printf(uart_bputc, "Firmware version handler called.\r\n");
}
/**
 * Set new address of a device
 */
void set_address_hdlr()
{
    printf(uart_bputc, "Address handler called.\r\n");
}
/**
 * Enable IR2110 H bridge drivers output
 */
void set_drv_enable_hdlr()
{
    printf(uart_bputc, "Enable handler called.\r\n");
}
