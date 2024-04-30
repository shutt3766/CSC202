//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//
//  DESIGNER NAME:  Bruce Link
//
//       LAB NAME:  Lab 3, part 1
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program serves as a practical exercise for students to understand
//    the sizes of various C data types on the MSPM0G3507 microcontroller and
//    how multiple-byte data types are stored in memory. 
//
//    The program defines a structure 'test_struct' and a union 'union_32' 
//    to hold different data types. It initializes local variables of these 
//    types and initializes a long integer within a union. 
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------
#include <stdio.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>
#include "Clock.h"
#include "uart.h"


//-----------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------




//-----------------------------------------------------------
// Define global variable and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------

// Define a structure to hold different data types
typedef struct
{
  unsigned  long  int  LA;
  signed    long  int  LB;
  unsigned        int  IA[2];
  signed          int  IB[2];
  unsigned  short int  SA[2];
  signed    short int  SB[2];
  unsigned       char  CA[4];
  signed         char  CB[4];
} test_struct;


// Define a union to hold different 32 bit values
typedef union
{
  unsigned  long  int  LA;
  signed    long  int  LB;
  unsigned        int  IA;
  signed          int  IB;
  unsigned  short int  SA[2];
  signed    short int  SB[2];
  unsigned       char  CA[4];
  signed         char  CB[4];
} union_32;


int main(void)
{
  // define local variable
  test_struct struct_var;
  union_32    union_var;

  // Initialize long integer
  union_var.LA = 0x12345678;
  
  clock_init_40mhz();
  UART_Init();

  msp_printf(" *** PROGRAM RUNNING ***\r\n\r\n",0);
  msp_printf("CSC-202 MSP M0 Data Type Tester\r\n",0);
  msp_printf("--------------------------------------------------------\r\n",0);
  msp_printf(" unsigned long int = 0x%x bytes\r\n", sizeof(struct_var.LA));
  msp_printf("   signed long int = 0x%x bytes\r\n", sizeof(struct_var.LB));
  msp_printf("      unsigned int = 0x%x bytes\r\n", sizeof(struct_var.IA[0]));
  msp_printf("        signed int = 0x%x bytes\r\n", sizeof(struct_var.IB[0]));
  msp_printf("unsigned short int = 0x%x bytes\r\n", sizeof(struct_var.SA[0]));
  msp_printf("  signed short int = 0x%x bytes\r\n", sizeof(struct_var.SB[0]));
  msp_printf("     unsigned char = 0x%x bytes\r\n", sizeof(struct_var.CA[0]));
  msp_printf("       signed char = 0x%x bytes\r\n", sizeof(struct_var.CB[0]));

  msp_printf("\r\n",0);
  msp_printf(" The structure = 0x%x bytes\r\n", sizeof(struct_var));
  msp_printf("     The union = 0x%x bytes\r\n", sizeof(union_var));
  msp_printf("\r\n",0);

  msp_printf("Dumping data values from Union\r\n",0);
  msp_printf("--------------------------------------------------------\r\n",0);
  msp_printf("    union unsigned long int LA = 0x%X\r\n", union_var.LA);
  msp_printf("      union signed long int LB = 0x%X\r\n", union_var.LB);
  msp_printf("\r\n",0);

  msp_printf("         union unsigned int IA = 0x%X\r\n", union_var.IA);
  msp_printf("           union signed int IB = 0x%X\r\n", union_var.IB);
  msp_printf("\r\n",0);

  msp_printf("union unsigned short int SA[0] = 0x%X\r\n", union_var.SA[0]);
  msp_printf("  union signed short int SB[0] = 0x%X\r\n", union_var.SB[0]);
  msp_printf("union unsigned short int SA[1] = 0x%X\r\n", union_var.SA[1]);
  msp_printf("  union signed short int SB[1] = 0x%X\r\n", union_var.SB[1]);
  msp_printf("\r\n",0);

  msp_printf("      union unsigned char CA[0] = 0x%X\r\n", union_var.CA[0]);
  msp_printf("      union unsigned char CA[1] = 0x%X\r\n", union_var.CA[1]);
  msp_printf("      union unsigned char CA[2] = 0x%X\r\n", union_var.CA[2]);
  msp_printf("      union unsigned char CA[3] = 0x%X\r\n", union_var.CA[3]);
  msp_printf("\r\n",0);

  msp_printf("        union signed char CB[0] = 0x%X\r\n", union_var.CB[0]);
  msp_printf("        union signed char CB[1] = 0x%X\r\n", union_var.CB[1]);
  msp_printf("        union signed char CB[2] = 0x%X\r\n", union_var.CB[2]);
  msp_printf("        union signed char CB[3] = 0x%X\r\n", union_var.CB[3]);
  msp_printf("\r\n",0);
  msp_printf("\r\n",0);

  msp_printf(" *** PROGRAM TERMINATED ***\r\n",0);
  
  for(;;);

} /* main */

