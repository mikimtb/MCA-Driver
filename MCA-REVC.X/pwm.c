/******************************************************************************/
/* File:   pwm.c                                                              */
/* Author: Miroslav Bozic                                                     */
/*                                                                            */
/* MCA Driver Power Control PWM and CCP PWM COnfiguration                     */
/******************************************************************************/
#include "pwm.h"

// Defines
#define PPWM_PERIOD     512
#define PPWM_HALF_DUTY  (PPWM_PERIOD * 2)
// Variables
signed int16 ppwm_duty_max = 0;
signed int16 ppwm_duty_min = 0;

/**
 * Initialize CCP1 PWM and Power PWM modules
 * @param psu_voltage - High Voltage that is connected to J9 PIN 1-2
 * @param motor_nom_voltage - Nominal voltage of a selected DC motor
 * @param brake_nom_voltage - Nominal voltage of a selected DC brake, 0 - No Brake
 */
void pwm_init(BYTE psu_voltage, BYTE motor_nom_voltage, BYTE brake_nom_voltage)
{
    // Compute and init CCP1 module for DC Brake control
    float ccp1_pwm_factor = psu_voltage / 512.0;
    long dcb_pwm_duty = brake_nom_voltage / ccp1_pwm_factor;
    // If DC brake is not used CCP1 PWM init should be avoided
    if (brake_nom_voltage != 0)
        pwm_init_ccp1(dcb_pwm_duty);
    
    // Compute and init Power PWM module for DC Motor control
    pwm_init_ppwm();
    
    if (psu_voltage <= motor_nom_voltage)
    {
      // If power supply voltage isn't higher than motor nominal voltage
      // maximal voltage on the motor is 95% of selected power supply output
      ppwm_duty_max = PPWM_HALF_DUTY * 95 / 100;
      ppwm_duty_min = ~(ppwm_duty_max);
   }
   else
   {
      // elsewhere selected voltage is calculated
      float x = (float)motor_nom_voltage / (float)psu_voltage;
      ppwm_duty_max = PPWM_HALF_DUTY * x;
      ppwm_duty_min = ~(ppwm_duty_max);
   }
}

/**
 * Init CCP1 PWM module
 * @param pwm_duty - Duty cycle that correspond to the selected DC brake voltage
 */
void pwm_init_ccp1(long pwm_duty)
{
    setup_ccp1(CCP_OFF);
    setup_timer_2(T2_DIV_BY_4, 127, 1);
    set_pwm1_duty(pwm_duty);
}

/**
 * Init Power PWM module 
 */
void pwm_init_ppwm()
{
    output_low(PIN_D7);                     // Disable PWM output
    // Enable PPWM channel 1 and 2 
    setup_power_pwm_pins(PWM_COMPLEMENTARY,
                        PWM_COMPLEMENTARY,
                        PWM_PINS_DISABLED,
                        PWM_PINS_DISABLED);
    
   // Mode = Center Aligned, pPWM clk div 4, DEAD_TIME clk div 2  
   // Postscale = 1         (1-16) Timebase output postscaler 
   // TimeBase = 0          (0-65355) Initial value of PWM Timebase 
   // Period = 512          (0-4095) Max value of PWM TimeBase 
   // Compare = 0           (Timebase value for special event trigger) 
   // Compare Postscale = 1 (Postscaler for Compare value) 
   // Dead Time = 40        (Dead time is Fosc/2 * 40 = 2us)     
    setup_power_pwm(PWM_UP_DOWN | PWM_CLOCK_DIV_4 | PWM_DEAD_CLOCK_DIV_2, 1, 0, PPWM_PERIOD, 0, 1, 40);
    
    set_power_pwm0_duty(PPWM_HALF_DUTY);
    set_power_pwm2_duty(PPWM_HALF_DUTY);
}

/**
 * Set Power PWM duty cycle
 * @param duty - (-1023 : +1023), positive number - CW; negative number - CCW 
 */
void pwm_set_ppwm_duty(signed int16 duty)
{
    // Check over voltage select exception
    if (duty > ppwm_duty_max)
    {
      duty = ppwm_duty_max;
    }
    else if (duty < ppwm_duty_min)
    {
      duty = ppwm_duty_min;
    }
    // Update PPWM resulting duty cycle
    set_power_pwm0_duty(PPWM_HALF_DUTY + duty);
    set_power_pwm2_duty(PPWM_HALF_DUTY - duty);
}
