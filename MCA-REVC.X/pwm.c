/******************************************************************************/
/* File:   pwm.c                                                              */
/* Author: Miroslav Bozic                                                     */
/*                                                                            */
/* MCA Driver Power Control PWM and CCP PWM COnfiguration                     */
/******************************************************************************/
#include "pwm.h"

void pwm_init(BYTE psu_voltage, BYTE motor_nom_voltage, BYTE brake_nom_voltage)
{
    float ccp1_pwm_factor = psu_voltage / 512.0;
    long dcb_pwm_duty = brake_nom_voltage / ccp1_pwm_factor;
    pwm_init_ccp1(dcb_pwm_duty);
}

void pwm_init_ccp1(long pwm_duty)
{
    setup_ccp1(CCP_OFF);
    setup_timer_2(T2_DIV_BY_4, 127, 1);
    set_pwm1_duty(pwm_duty);
}
