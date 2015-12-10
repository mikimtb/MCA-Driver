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
void pwm_init_ppwm(void);
#inline void pwm_enable_ppwm_output() { output_high(PIN_D7); }
#inline void pwm_disable_ppwm_output() { output_low(PIN_D7); }
void pwm_set_ppwm_duty(signed int16);
#endif	/* PWM_H */

