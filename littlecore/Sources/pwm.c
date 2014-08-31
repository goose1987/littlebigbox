/*
 * pwm.c
 *
 *  Created on: Aug 30, 2014
 *      Author: hoang.pham
 */
//#include "MKE02Z4.h"
#include "Cpu.h"
#include "pwm.h"

/*
 * pwm.c
 *
 *  Created on: 11 Nov 2011
 *      Author: Charles
 */


#define FTM2_CLK_PRESCALE 1
#define FTM2_OVERFLOW_FREQUENCY (32000)

void InitPWM(void)
{

//Enable the Clock to the FTM2 Module
SIM_SCGC |= SIM_SCGC_FTM2_MASK;

//Pin control Register (MUX allowing user to route the desired signal to the pin.
/*
PORTC_PCR1   = PORT_PCR_MUX(0x4);
PORTC_PCR2   = PORT_PCR_MUX(0x4);
PORTC_PCR3   = PORT_PCR_MUX(0x4);
PORTC_PCR4   = PORT_PCR_MUX(0x4);
PORTD_PCR4   = PORT_PCR_MUX(0x4);
PORTD_PCR5   = PORT_PCR_MUX(0x4);
*/

//FTM2_MODE[WPDIS] = 1; //Disable Write Protection - enables changes to QUADEN, DECAPEN, etc.
FTM2_MODE |= FTM_MODE_WPDIS_MASK;

//FTMEN is bit 0, need to set to zero so DECAPEN can be set to 0
FTM2_MODE &= ~FTM_MODE_FTMEN_MASK;


//Set Edge Aligned PWM


// Also need to setup the FTM2C0SC channel control register
FTM2_CNT = 0x0; //FTM Counter Value - reset counter to zero
//FTM2_MOD = (PERIPHERAL_BUS_CLOCK/(1<<FTM2_CLK_PRESCALE))/FTM2_OVERFLOW_FREQUENCY ;  //Set the overflow rate
FTM2_MOD = 0xff;
FTM2_CNTIN = 0; //Set the Counter Initial Value to 0

// FTMx_CnSC - contains the channel-interrupt status flag control bits
FTM2_C0SC |= FTM_CnSC_ELSB_MASK; //Edge or level select
FTM2_C0SC &= ~FTM_CnSC_ELSA_MASK; //Edge or level Select
FTM2_C0SC |= FTM_CnSC_MSB_MASK; //Channel Mode select

// FTMx_CnSC - contains the channel-interrupt status flag control bits
FTM2_C1SC |= FTM_CnSC_ELSB_MASK; //Edge or level select
FTM2_C1SC &= ~FTM_CnSC_ELSA_MASK; //Edge or level Select
FTM2_C1SC |= FTM_CnSC_MSB_MASK; //Channel Mode select


//Edit registers when no clock is fed to timer so the MOD value, gets pushed in immediately
FTM2_SC = 0; //Make sure its Off!


//FTMx_CnV contains the captured FTM counter value, this value determines the pulse width
FTM2_C0V = FTM2_MOD/2;
FTM2_C1V = FTM2_MOD/2;

FTM2_COMBINE = FTM_COMBINE_COMBINE1_MASK;
FTM2_COMBINE = FTM_COMBINE_COMBINE0_MASK|FTM_COMBINE_COMP0_MASK|/*FTM_COMBINE_DECAPEN0_MASK|FTM_COMBINE_DECAP0_MASK|*/
               FTM_COMBINE_DTEN0_MASK|FTM_COMBINE_SYNCEN0_MASK|
               FTM_COMBINE_COMBINE1_MASK|FTM_COMBINE_COMP1_MASK|/*FTM_COMBINE_DECAPEN0_MASK|FTM_COMBINE_DECAP0_MASK|*/
               FTM_COMBINE_DTEN1_MASK|FTM_COMBINE_SYNCEN1_MASK|
               FTM_COMBINE_COMBINE2_MASK|FTM_COMBINE_COMP2_MASK|/*FTM_COMBINE_DECAPEN0_MASK|FTM_COMBINE_DECAP0_MASK|*/
               FTM_COMBINE_DTEN2_MASK|FTM_COMBINE_SYNCEN2_MASK;

FTM2_DEADTIME = FTM_DEADTIME_DTVAL(50); //About 5usec
/* SWOCTRL Bit Fields */
   //FTM2_SWOCTRL|= FTM_SWOCTRL_CH0OC_MASK|FTM_SWOCTRL_CH1OC_MASK|FTM_SWOCTRL_CH2OC_MASK|FTM_SWOCTRL_CH3OC_MASK|FTM_SWOCTRL_CH4OC_MASK|FTM_SWOCTRL_CH5OC_MASK;


//Status and Control bits
FTM2_SC =  FTM_SC_CLKS(1); // Selects Clock source to be "system clock" or (01)
//sets pre-scale value see details below
FTM2_SC |= FTM_SC_PS(FTM2_CLK_PRESCALE);

/******begin FTM_SC_PS details ****************************
* Sets the Prescale value for the Flex Timer Module which divides the
* Peripheral bus clock -> 48Mhz by the set amount
* Peripheral bus clock set up in clock.h
*
* The value of the prescaler is selected by the PS[2:0] bits.
* (FTMx_SC field bits 0-2 are Prescale bits -  set above in FTM_SC Setting)
*
*  000 - 0 - No divide
*  001 - 1 - Divide by 2
*  010 - 2 - Divide by 4
*  011 - 3 - Divide by 8
*  100 - 4 - Divide by 16
*  101 - 5 - Divide by 32
*  110 - 6 - Divide by 64 -
*  111 - 7 - Divide by 128
*  ******end FTM_SC_PS details*****************************/

// Interrupts
//FTM2_SC |= FTM_SC_TOIE_MASK; //Enable the interrupt mask.  timer overflow interrupt.. enables interrupt signal to come out of the module itself...  (have to enable 2x, one in the peripheral and once in the NVIC

}

void UpdatePWM(unsigned short pwm1,unsigned short pwm2,unsigned short pwm3)
{
    unsigned mod = FTM2_MOD/2;
    FTM2_C0V = mod-pwm1;
    FTM2_C1V = mod+pwm1;

    FTM2_PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;
}
