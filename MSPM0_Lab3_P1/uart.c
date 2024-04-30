// *****************************************************************************
// ***************************    C Source Code     ****************************
// *****************************************************************************
//   DESIGNER NAME:  Bruce Link
//
//       FILE NAME:  UARTbusywait.c
//
//-----------------------------------------------------------------------------
// DESCRIPTION
//    This module provides functions to initialize and control UART0 on 
//    the MSPM0G3507 Launchpad development board. UART0 is configured  
//    for a baud rate of 115200, no parity, 8 data bits, and 1 stop bit 
//    (115200N81).
//
//    Pin Configuration:
//        - UART0 Tx: PA.10 (connected to XDS Rx)
//         - UART0 Rx: PA.11 (connected to XDS Tx)
//
//    Required Jumper Configuration:
//      - J25: Connects PA10 to XDS_UART
//      - J26: Connects PA11 to XDS_UART
//
//    Note: The code for this module is adapted from TI's sample project 
//    'uart_rw_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang'.
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
#include <stdio.h>


//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>
#include "ti/devices/msp/m0p/mspm0g350x.h"

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------

#include "uart.h"
#include "Clock.h"


//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This configures UART0 for communication with specified settings 
//    initialize UART for 115200N81. It also configures the GPIO pins for
//    UART0 transmission and reception, selects the clock source based on 
//    the bus clock, sets the baud rate divisor, and enables UART0.
//
//    NOTE: This function assumes the system clock frequency to be 
//    either 40MHz, 32MHz, or 80MHz. If the clock frequency is different, 
//    error handling should be implemented.
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
void UART_Init(void)
{
  // Configure UART0 Reset Control Register 
  UART0->GPRCM.RSTCTL = UART_RSTCTL_KEY_UNLOCK_W | 
                        UART_RSTCTL_RESETSTKYCLR_CLR | 
                        UART_RSTCTL_RESETASSERT_ASSERT;
  
  // Enable power to UART0 peripheral
  UART0->GPRCM.PWREN = UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_ENABLE;

   // time for uart to power up
   clock_delay(24);
  
  // configure PA11 and PA10 as alternate UART0 function
  IOMUX->SECCFG.PINCM[IOMUX_PINCM21]  = IOMUX_PINCM_PC_MASK | 
                                        IOMUX_PINCM21_PF_UART0_TX;
  IOMUX->SECCFG.PINCM[IOMUX_PINCM22]  = IOMUX_PINCM_INENA_ENABLE | 
                                        IOMUX_PINCM_PC_MASK | 
                                        IOMUX_PINCM22_PF_UART0_RX;

  // Configrue the clock source for UART0
  UART0->CLKSEL = UART_CLKSEL_BUSCLK_SEL_ENABLE | 
                  UART_CLKSEL_MFCLK_SEL_DISABLE | UART_CLKSEL_LFCLK_SEL_DISABLE;

  // Configrue the divide ratio of the functional clock
  UART0->CLKDIV = UART_CLKDIV_RATIO_DIV_BY_1;

  // Configure UART0 but do not enable it yet
  UART0->CTL0 = UART_CTL0_FEN_ENABLE | UART_CTL0_HSE_OVS16 | 
                UART_CTL0_CTSEN_DISABLE | UART_CTL0_CTSEN_ENABLE | 
                UART_CTL0_RTS_CLR | UART_CTL0_MODE_UART | 
                UART_CTL0_TXE_ENABLE| UART_CTL0_RXE_ENABLE | 
                UART_CTL0_LBE_DISABLE | UART_CTL0_ENABLE_DISABLE;

  // Get the current clock frequency so we can configure the
  // UART Integer Baud-Rate Divisor
  // BRD = UARTclk / (OVS * BAUD Rate)
  if(get_bus_clock_freq() == 40000000)
  {
    // 20000000/16 = 1,250,000 Hz
    // Baud = 115200
    //   1,250,000/115200 = 10.850694
    //   divider = 10+54/64 = 10.84375
    UART0->IBRD = 10;
    UART0->FBRD = 54; // baud =1,250,000/10.84375 = 115,274
  } /* if */
  else if (get_bus_clock_freq() == 32000000)
  {
    // 32000000/16 = 2,000,000
    // Baud = 115200
    //   2,000,000/115200 = 17.361
    //   divider = 17+23/64 = 17.359
    UART0->IBRD = 17;
    UART0->FBRD = 23;
  } /* else if */
  else if (get_bus_clock_freq() == 80000000)
  {
     // 40000000/16 = 2,500,000 Hz
     // Baud = 115200
      //    2,500,000/115200 = 21.701388
      //   divider = 21+45/64 = 21.703125
    UART0->IBRD = 21;
    UART0->FBRD = 45; // baud =2,500,000/21.703125 = 115,191
  }/* else if */
  else
  {
    // We have an issue here
    // TO DO: add error handling
    return;
  } /* else */
  
  // Any changes to the baud-rate divisor must be followed by a 
  // write to the UARTLCRH register  
  UART0->LCRH = UART_LCRH_WLEN_DATABIT8 | UART_LCRH_STP2_DISABLE | 
                UART_LCRH_EPS_ODD | UART_LCRH_PEN_DISABLE | 
                UART_LCRH_BRK_DISABLE;

  // Now enable UART0
  UART0->CTL0 |= UART_CTL0_ENABLE_ENABLE;
} /* UART_Init */



//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This function continuously checks the receive FIFO status of UART0 
//    until it is not empty. Once data is available, it retrieves a single 
//    character from the receive FIFO and returns it.
//
// INPUT PARAMETERS:
//   none
//
// OUTPUT PARAMETERS:
//   none
//
// RETURN:
//   ASCII code for key entered
// -----------------------------------------------------------------------------
char UART_InChar(void)
{
  while((UART0->STAT & UART_STAT_RXFE_MASK) == UART_STAT_RXFE_SET);

  return((char)(UART0->RXDATA));
} /* UART_InChar */


//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This function sends the specified character over UART0 after waiting 
//    for the transmission FIFO to have space for new data. It blocks until 
//    there is space available in the transmission FIFO. Once space is 
//    available, the character is written to the UART0 transmit data register.
//
// INPUT PARAMETERS:
//   data - letter is an 8-bit ASCII character to be transferred
//
// OUTPUT PARAMETERS:
//   none
//
// RETURN:
//   none
// -----------------------------------------------------------------------------
void UART_OutChar(char data)
{
  // wait if TxFifo full
  while((UART0->STAT & UART_STAT_TXFF_MASK) == UART_STAT_TXFF_SET);

  UART0->TXDATA = data;
} /* UART_OutChar */


//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This function sends the specified NULL terminated string over UART0.
// TODO: FIX decription
//
// INPUT PARAMETERS:
//   data - letter is an 8-bit ASCII character to be transferred
//
// OUTPUT PARAMETERS:
//   none
//
// RETURN:
//   none
// -----------------------------------------------------------------------------
void msp_printf(char* buffer, unsigned int value)
{
  unsigned int i = 0;
  unsigned int len = 0;
  char  string[80];

  len = sprintf(string, buffer, value);

  // Walk through array to send each character to serial port
  for (i = 0; i< len; i++)
  {
    UART_OutChar(string[i]);      
  } /* for */
  
} /* msp_printf */
