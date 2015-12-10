/******************************************************************************/
/* File:   input.c                                                            */
/* Author: Miroslav Bozic                                                     */
/*                                                                            */
/* MCA Driver digital and analog data reading                                 */
/******************************************************************************/

#include "input.h"
// Defines
#define C1  PIN_D5
#define C2  PIN_D6

/**
 * Read digital input C1
 * @return HIGH - input is activated, LOW - input isn't activated
 */
short input_read_c1()
{
    return !input(C1);
}

/**
 * Read digital input C2
 * @return HIGH - input is activated, LOW - input isn't activated
 */
short input_read_c2()
{
    return input(C2);
}