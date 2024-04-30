//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//
//      FILE NAME:  clock.h
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This file was derived from gpio_toggle_LP_MSPM03507_nortos_ticlang
//    project template that runs on the LP_MSPM0G3507 LaunchPad board.

//
//    This code provides a test vehicle to understand the bitwise manipulation
//    using C and the MSPM0 microcontroller.
//
//*****************************************************************************
//*****************************************************************************

#ifndef __CLOCK_H__
#define __CLOCK_H__


// ----------------------------------------------------------------------------
// Prototype for support functions
// ----------------------------------------------------------------------------
void clock_init_80mhz(int enablePA14);
void clock_init_40mhz(void);
void clock_init(uint32_t freq);
uint32_t get_bus_clock_freq(void);
void clock_delay(uint32_t cycles) __attribute__((noinline));
void msec_delay(uint32_t ms);
void systick_init(void);

#endif /* __CLOCK_H__ */
