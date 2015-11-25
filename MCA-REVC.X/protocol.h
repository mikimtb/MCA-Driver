/******************************************************************************/
/* File:   protocol.h                                                         */
/* Author: Miroslav Bozic                                                     */
/******************************************************************************/

#ifndef PROTOCOL_H
#define	PROTOCOL_H

//Function prototypes
int protocol_cmd_parse_request(BYTE);
void set_address_hdlr(void);
void drv_enable_hdlr(void);

#endif	/* PROTOCOL_H */

