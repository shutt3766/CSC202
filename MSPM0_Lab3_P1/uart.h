//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//
//  DESIGNER NAME: Bruce Link
//
//      FILE NAME:  uart.h
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This module provides functions to initialize and control UART0 on 
//    the MSPM0G3507 Launchpad development board. UART0 is configured  
//    for a baud rate of 115200, no parity, 8 data bits, and 1 stop bit 
//    (115200N81).
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

#ifndef __UART_H__
#define __UART_H__



// --------------------------------------------------------------------------
// Prototype for Launchpad support functions
// --------------------------------------------------------------------------

void UART_Init(void);
void UART_OutChar(char data);
void msp_printf(char* buffer, unsigned int value);


#endif /* __UART_H__ */