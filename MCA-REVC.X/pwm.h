//******************************************************************************/
/* File:   pwm.h                                                              */
/* Author: Miroslav Bozic                                                     */
/******************************************************************************/

#ifndef PWM_H
#define	PWM_H

void pwm_init(BYTE, BYTE, BYTE);
void pwm_init_ccp1(long);
#inline void pwm_brake_release() { setup_ccp1(CCP_PWM); }
#inline void pwm_brake_catch() { setup_ccp1(CCP_OFF); }

#endif	/* PWM_H */

