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

short input_read_c1()
{
    return input(C1);
}

short input_read_c2()
{
    return input(C2);
}