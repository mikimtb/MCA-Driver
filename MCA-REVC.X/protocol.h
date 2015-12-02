/******************************************************************************/
/* File:   protocol.h                                                         */
/* Author: Miroslav Bozic                                                     */
/******************************************************************************/

#ifndef PROTOCOL_H
#define	PROTOCOL_H

//Function prototypes
int protocol_cmd_parse_request(BYTE);
void wake_up_hdlr(void);
void get_firmware_version_hdlr(void);
void set_address_hdlr(void);
void set_drv_enable_hdlr(void);

#endif	/* PROTOCOL_H */

