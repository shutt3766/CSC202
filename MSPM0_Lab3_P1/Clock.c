// *****************************************************************************
// ***************************    C Source Code     ****************************
// *****************************************************************************
//
//   DESIGNER NAME:  Bruce Link
//
//       FILE NAME:  clock.c   4/12/2024
//
//-----------------------------------------------------------------------------
// DESCRIPTION
// Derived from sysctl_hfxt_run_LP_MSPM0G3507_nortos_ticlang
//              systick_periodic_timer_LP_MSPM0G3507_nortos_ticlang
//              sysctl_mclk_syspll_LP_MSPM0G3507_nortos_ticlang

//
//-----------------------------------------------------------------------------
// DISCLAIMER
//    This code was developed for educational purposes only.
//
//    It is provided "as is" without warranties of any kind, whether express,
//    implied, or statutory.
//
//    The author and organization do not warrant the accuracy, completeness, or
//    reliability of the code. The author and organization shall not be liable
//    for any direct, indirect, incidental, special, exemplary, or consequential
//    damages arising out of the use of or inability to use the code, even if
//    advised of the possibility of such damages.
//
// Copyright (c) 2024 by TBD
//    You may use, edit, run or distribute this file as long as the above
//    copyright notice remains
// *****************************************************************************
//******************************************************************************

//-----------------------------------------------------------------------------
// Load standard C include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>
#include "Clock.h"


//-----------------------------------------------------------------------------
// global signal to track status of bus clock
//-----------------------------------------------------------------------------
uint32_t volatile g_bus_clock_freq = 32000000; 

//-----------------------------------------------------------------------------
// DESCRIPTION:
//   This function returns current configured bus clock frequency for the 
//   Launchpad development board
//
// INPUT PARAMETERS:
//   none
//
// OUTPUT PARAMETERS:
//   none
//
// RETURN:
//   current bus clock frequency as a uint32 value
// -----------------------------------------------------------------------------
uint32_t get_bus_clock_freq(void)
{
  return g_bus_clock_freq;
} /* get_bus_clock_freq */

//-----------------------------------------------------------------------------
// DESCRIPTION:
//   This function configures the bus clock for the to be 40MHz.
//
// INPUT PARAMETERS:
//   none
//
// OUTPUT PARAMETERS:
//   none
//
// RETURN:
//   none
// -----------------------------------------------------------------------------
void clock_init_40mhz(void)
{

  // Configure the SYSOSC configuration register to set the Target operating 
  // frequency (FREQ) for the system oscillator (SYSOSC)
  SYSCTL->SOCLOCK.SYSOSCCFG &= ~SYSCTL_SYSOSCCFG_FREQ_MASK;
  SYSCTL->SOCLOCK.SYSOSCCFG |=  SYSCTL_SYSOSCCFG_FREQ_SYSOSCBASE;
  

  // Set HFXT Range Select for the High-frequency clock (HFCLK) configuration
  SYSCTL->SOCLOCK.HFCLKCLKCFG &= ~SYSCTL_HFCLKCLKCFG_HFXTRSEL_MASK;
  SYSCTL->SOCLOCK.HFCLKCLKCFG |=  SYSCTL_HFCLKCLKCFG_HFXTRSEL_RANGE32TO48;

  // Clear and set HFXTTIME to specify the HFXT startup time in 64us resolution
  // 64us * 10 = 640 us
  SYSCTL->SOCLOCK.HFCLKCLKCFG &= ~SYSCTL_HFCLKCLKCFG_HFXTTIME_MASK;
  SYSCTL->SOCLOCK.HFCLKCLKCFG |=  0x00000000A;

  // Set HFXTEN to enable the high frequency crystal oscillator
  SYSCTL->SOCLOCK.HSCLKEN |= SYSCTL_HSCLKEN_HFXTEN_ENABLE;

  // Set HFCLKFLTCHK to enable the HFCLK startup monitor
  SYSCTL->SOCLOCK.HFCLKCLKCFG |= SYSCTL_HFCLKCLKCFG_HFCLKFLTCHK_ENABLE;

  // Wait until HSCLKGOOD is set to indict HFCLK
  // started correctly. NOTE Possible infinite loop if fails
  while ((SYSCTL->SOCLOCK.CLKSTATUS & SYSCTL_CLKSTATUS_HFCLKGOOD_MASK) != 
          SYSCTL_CLKSTATUS_HFCLKGOOD_TRUE);

  // Configure the HSCLK source (SYSPLL or HFCLK).
  SYSCTL->SOCLOCK.HSCLKCFG = SYSCTL_HSCLKCFG_HSCLKSEL_HFCLKCLK;

  // Wait here until HSCLKGOOD is set to indict HSCLK
  // started correctly. NOTE Possible infinite loop if fails
  while ((SYSCTL->SOCLOCK.CLKSTATUS & SYSCTL_CLKSTATUS_HSCLKGOOD_MASK) != 
          SYSCTL_CLKSTATUS_HSCLKGOOD_TRUE);

  // Configrue USEHSCLK, together with USELFCLK, to sets the MCLK source policy.
  // Set USEHSCLK to use HSCLK (HFCLK or SYSPLL) as the MCLK source in 
  // RUN and SLEEP modes.
  SYSCTL->SOCLOCK.MCLKCFG &= ~SYSCTL_MCLKCFG_USEHSCLK_MASK;
  SYSCTL->SOCLOCK.MCLKCFG |=  SYSCTL_MCLKCFG_USEHSCLK_ENABLE;

  // Wait until HSCLKMUX is set to indict MCLK is currently sourced from the 
  // highspeed clock (HSCLK). NOTE Possible infinite loop if fails
  while ((SYSCTL->SOCLOCK.CLKSTATUS & SYSCTL_CLKSTATUS_HSCLKMUX_MASK) != 
          SYSCTL_CLKSTATUS_HSCLKMUX_HSCLK);

  // MDIV may be used to divide the MCLK frequency when MCLK is sourced 
  // from SYSOSC.Note: MDIV=0h corresponds to /1 (no divider).
  SYSCTL->SOCLOCK.MCLKCFG &= ~SYSCTL_MCLKCFG_MDIV_MASK;

  // Configure UDIV to specify ULPCLK divider when MCLK is sourced from HSCLK. 
  // UDIV has no effect when MCLK is sourced from SYSOSC or LFCLK.
  SYSCTL->SOCLOCK.MCLKCFG &= ~SYSCTL_MCLKCFG_UDIV_MASK;
  SYSCTL->SOCLOCK.MCLKCFG |= SYSCTL_MCLKCFG_UDIV_DIVIDE2;

  // Configure FLASHWAIT for the number of flash wait states when MCLK is 
  // sourced from HSCLK. FLASHWAIT has no effect when MCLK is sourced 
  // from SYSOSC or LFCLK.
  SYSCTL->SOCLOCK.MCLKCFG &= ~SYSCTL_MCLKCFG_FLASHWAIT_MASK;
  SYSCTL->SOCLOCK.MCLKCFG |= SYSCTL_MCLKCFG_FLASHWAIT_WAIT2;

  // Configure EXCLKSRC for the source of the CLK_OUT external clock
  // output block. ULPCLK and MFPCLK require the CLK_OUT divider (EXCLKDIVEN) 
  // to be enabled
  SYSCTL->SOCLOCK.GENCLKCFG &= ~SYSCTL_GENCLKCFG_EXCLKSRC_MASK;
  SYSCTL->SOCLOCK.GENCLKCFG |=  SYSCTL_GENCLKCFG_EXCLKSRC_HFCLK;

  // Configure EXCLKDIVVAL for the divider value for the divider in the 
  // CLK_OUT external clock output block
  SYSCTL->SOCLOCK.GENCLKCFG &= ~SYSCTL_GENCLKCFG_EXCLKDIVVAL_MASK;
  SYSCTL->SOCLOCK.GENCLKCFG |= SYSCTL_GENCLKCFG_EXCLKDIVVAL_DIV4;

  // Set EXCLKDIVEN to enable the divider function of the CLK_OUT external 
  // clock output block
  SYSCTL->SOCLOCK.GENCLKCFG &= ~SYSCTL_GENCLKCFG_EXCLKDIVEN_MASK;
  SYSCTL->SOCLOCK.GENCLKCFG |= SYSCTL_GENCLKCFG_EXCLKDIVEN_ENABLE;

  // Set EXCLKEN to enable the CLK_OUT external clock output block
  SYSCTL->SOCLOCK.GENCLKEN |= SYSCTL_GENCLKEN_EXCLKEN_ENABLE;

  // Configure the BOR threshold selection.  The LEVEL specifies the desired 
  // BOR threshold and BOR mode.
  SYSCTL->SOCLOCK.BORTHRESHOLD = SYSCTL_BORTHRESHOLD_LEVEL_BORMIN;

  // Wait here until all clock status (HSCLKGOOD, LFOSCGOOD, and HFCLKGOOD)
  // are set by hardware to indict HSCLK, LFOSC, and SYSPLL are OK and ready
  // NOTE Possible infinite loop if fails
  while ((SYSCTL->SOCLOCK.CLKSTATUS & (SYSCTL_CLKSTATUS_HSCLKGOOD_MASK | 
          SYSCTL_CLKSTATUS_LFOSCGOOD_MASK | SYSCTL_CLKSTATUS_HFCLKGOOD_MASK)) !=
          (SYSCTL_CLKSTATUS_HSCLKGOOD_TRUE | SYSCTL_CLKSTATUS_LFOSCGOOD_TRUE | 
          SYSCTL_CLKSTATUS_HFCLKGOOD_TRUE));

  //SCB->SCR System control register, used to control low-power mode behavior
  // bit 2, 0 is sleep, 1 is deep sleep
  // bit 1, 0 do not sleep when returning to Thread mode, 1 enter
  // sleep/deepsleep on returning from ISR to thread
  SCB->SCR &= ~(0x04);            // sleep
  SCB->SCR &= ~(0x02);            // no sleep on exit
  
  // update the bus clock frequency
  g_bus_clock_freq = 40000000;

} /* clock_init_40mhz */


//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This function creates a simple delay function which delays couple 
//    bus cycles. It is implemented with a for-loop and is should only be 
//    used to create an approximatation of a cycle delay.
//
//    Note: This is a software implementation of a delay approximation.
//
// INPUT PARAMETERS:
//   cycles - a unit32 value that reprelsents the number bus cycles to wait
//
// OUTPUT PARAMETERS:
//   none
//
// RETURN:
//   none
// -----------------------------------------------------------------------------
void clock_delay(uint32_t cycles)
{
  /* There will be a 2 cycle delay here to fetch & decode instructions
    * if branch and linking to this function */

  /* Subtract 2 net cycles for constant offset: +2 cycles for entry jump,
    * +2 cycles for exit, -1 cycle for a shorter loop cycle on the last loop,
    * -1 for this instruction */
#ifdef __GNUC__
    __asm(".syntax unified");
#endif
    __asm volatile(
"            SUBS  R0, R0, #2; \n"
"Clock_Loop: SUBS  R0, R0, #4; \n" // C=1 if no overflow
"            NOP;              \n" // C=0 when R0 passes through 0
"            BHS   Clock_Loop; \n"
        /* Return: 2 cycles */
    );
} /* clock_delay */


//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This function create a simple delay function which approimates the 
//    delay of about 1 millisecond.
//
//    Note: This is a software implementation of a delay approximation.
//
// INPUT PARAMETERS:
//   ms_delay_count - the number of mSeconds to wait
//
// OUTPUT PARAMETERS:
//   none
//
// RETURN:
//   none
// -----------------------------------------------------------------------------
void msec_delay(uint32_t ms_delay_count)
{
  // each call to clock_delay is count cycles
  uint32_t count = g_bus_clock_freq/1000;
  
  while(ms_delay_count)
  {
    clock_delay(count);
  ms_delay_count--;
  } /* while */

} /* clock_delay1ms */


//-----------------------------------------------------------------------------
// DESCRIPTION:
//   This function ...
// initialize 24 bit timer
//
// INPUT PARAMETERS:
//   none
//
// OUTPUT PARAMETERS:
//   none
//
// RETURN:
//   none
// -----------------------------------------------------------------------------
void systick_init(void)
{
  SysTick->CTRL = 0;                       // 1) disable SysTick during setup
  SysTick->LOAD = 0;                       // 2) stops
  SysTick->VAL = 0;                        // 3) any write to current clears it
  SysTick->CTRL = 0x00000005;              // 4) enable SysTick with core clock
} /* SysTick_Init */



//-----------------------------------------------------------------------------
// DESCRIPTION:
//   This function ...
// The delay parameter is in units of the core clock.
// about a 50 cycle overhead
//
// INPUT PARAMETERS:
//   none
//
// OUTPUT PARAMETERS:
//   none
//
// RETURN:
//   none
// -----------------------------------------------------------------------------
void SysTick_Wait(uint32_t delay)
{
  SysTick->LOAD = delay-1;               // number of counts to wait
  SysTick->VAL = 0;                      // any value written to VAL clears
  
  while((SysTick->CTRL & 0x00010000) == 0);  // wait for count flag
} /* SysTick_Wait */

