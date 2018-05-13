/************************************************************************
 *
 * def21160.h
 *
 * (c) Copyright 2001-2004 Analog Devices, Inc.  All rights reserved.
 * $Revision: 1.10 $
 ************************************************************************/

/* -----------------------------------------------------------------------------
def21160.h - SYSTEM & IOP REGISTER BIT & ADDRESS DEFINITIONS FOR ADSP-21160

Last Modification on: $Date: 2004/03/11 10:31:19 $

This include file contains a list of macro "defines" to enable the programmer
to use symbolic names for the following ADSP-21160 facilities:
 - instruction condition codes
 - system register bit definitions
 - IOP register map
 - *some* IOP register bit definitions

Here are some example uses:

   bit set mode1 BR0|IRPTEN|ALUSTAT;

   ustat1=BSO|HPM01|HMSWF;
   DM(SYSCON)=ustat1;

Modified on 8/12/02 - 'Completed' bit definitions. Changed to bit names rather
than hex values.
--------------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* ADSP-21160 definitions */

#ifndef __DEF21160_H_
#define __DEF21160_H_

#define BIT_0 0x00000001
#define BIT_1 0x00000002
#define BIT_2 0x00000004
#define BIT_3 0x00000008
#define BIT_4 0x00000010
#define BIT_5 0x00000020
#define BIT_6 0x00000040
#define BIT_7 0x00000080
#define BIT_8 0x00000100
#define BIT_9 0x00000200
#define BIT_10 0x00000400
#define BIT_11 0x00000800
#define BIT_12 0x00001000
#define BIT_13 0x00002000
#define BIT_14 0x00004000
#define BIT_15 0x00008000
#define BIT_16 0x00010000
#define BIT_17 0x00020000
#define BIT_18 0x00040000
#define BIT_19 0x00080000
#define BIT_20 0x00100000
#define BIT_21 0x00200000
#define BIT_22 0x00400000
#define BIT_23 0x00800000
#define BIT_24 0x01000000
#define BIT_25 0x02000000
#define BIT_26 0x04000000
#define BIT_27 0x08000000
#define BIT_28 0x10000000
#define BIT_29 0x20000000
#define BIT_30 0x40000000
#define BIT_31 0x80000000

/*------------------------------------------------------------------------------*/
/*                     I/O Processor Register Map                             	*/
/*------------------------------------------------------------------------------*/
#define SYSCON 0x00        	/* System configuration register                   	*/
#define VIRPT  0x01        	/* Vector interrupt register                       	*/
#define WAIT   0x02        	/* External Port Wait register - renamed to EPCON  	*/
#define EPCON  0x02        	/* External Port configuration register            	*/
#define SYSTAT 0x03        	/* System status register                          	*/
/* the upper 32-bits of the 64-bit epbxs are only accessible as 64-bit reference*/
#define EPB0   0x04        	/* External port DMA buffer 0                      	*/
#define EPB1   0x06        	/* External port DMA buffer 1                      	*/
#define MSGR0  0x08        	/* Message register 0                              	*/
#define MSGR1  0x09        	/* Message register 1                              	*/
#define MSGR2  0x0a        	/* Message register 2                              	*/
#define MSGR3  0x0b        	/* Message register 3                              	*/
#define MSGR4  0x0c        	/* Message register 4                              	*/
#define MSGR5  0x0d        	/* Message register 5                              	*/
#define MSGR6  0x0e        	/* Message register 6                              	*/
#define MSGR7  0x0f        	/* Message register 7                              	*/

/* IOP shadow registers of the core control regs                              	*/
#define PC_SHDW    0x10	   	/* PC IOP shadow register (PC[23-0])               	*/
#define MODE2_SHDW 0x11	   	/* Mode2 IOP shadow register (MODE2[31-25])        	*/
#define EPB2   0x14        	/* EXternal port DMA buffer 2                      	*/
#define EPB3   0x16        	/* External port DMA buffer 3                      	*/
#define BMAX   0x18	   	   	/* Bus time-out maximum			      			  	*/
#define BCNT   0x19	       	/* Bus time-out counter			      			  	*/
#define ELAST  0x1b	       	/* Address of last external access for page detect 	*/
#define DMAC10 0x1c	       	/* EP DMA10 control register			      		*/
#define DMAC11 0x1d	       	/* EP DMA11 control register			      		*/
#define DMAC12 0x1e	       	/* EP DMA12 control register			      		*/
#define DMAC13 0x1f	       	/* EP DMA13 Control register			      		*/

#define II4    0x30	   		/* Internal DMA4 memory address		      	 		*/
#define IM4    0x31	   		/* Internal DMA4 memory access modifier	     		*/
#define C4     0x32	   		/* Contains number of DMA4 transfers remaining  	*/
#define CP4    0x33	   		/* Points to next DMA4 parameters	      			*/
#define GP4    0x34	   		/* DMA4 General purpose / 2-D DMA	      			*/
#define DB4    0x35	   		/* DMA4 General purpose / 2-D DMA	      			*/
#define DA4    0x36	   		/* DMA4 General purpose / 2-D DMA	      			*/

/* DMA Channel 4 Corresponds to LBUF 0 */
#define IILB0    0x30	   		/* Internal DMA4 memory address		      	 		*/
#define IMLB0    0x31	   		/* Internal DMA4 memory access modifier	     		*/
#define CLB0     0x32	   		/* Contains number of DMA4 transfers remaining  	*/
#define CPLB0    0x33	   		/* Points to next DMA4 parameters	      			*/
#define GPLB0    0x34	   		/* DMA4 General purpose / 2-D DMA	      			*/
#define DBLB0    0x35	   		/* DMA4 General purpose / 2-D DMA	      			*/
#define DALB0    0x36	   		/* DMA4 General purpose / 2-D DMA	      			*/

#define DMASTAT 0x37	   	/* DMA channel status register		      			*/

#define II5    0x38	   		/* Internal DMA5 memory address		      			*/
#define IM5    0x39	   		/* Internal DMA5 memory access modifier	      		*/
#define C5     0x3a	   		/* Contains number of DMA5 transfers remainnig     	*/
#define CP5    0x3b	   		/* Points to next DMA5 parameters		      		*/
#define GP5    0x3c	   		/* DMA5 General purpose / 2-D DMA		      		*/
#define DB5    0x3d	   		/* DMA5 General pu	rpose / 2-D DMA		      		*/
#define DA5    0x3e	   		/* DMA5 General purpose / 2-D DMA		      		*/

/* DMA Channel 5 Corresponds to LBUF 1 */
#define IILB1    0x38	   		/* Internal DMA5 memory address		      			*/
#define IMLB1    0x39	   		/* Internal DMA5 memory access modifier	      		*/
#define CLB1     0x3a	   		/* Contains number of DMA5 transfers remainnig     	*/
#define CPLB1    0x3b	   		/* Points to next DMA5 parameters		      		*/
#define GPLB1    0x3c	   		/* DMA5 General purpose / 2-D DMA		      		*/
#define DBLB1    0x3d	   		/* DMA5 General pu	rpose / 2-D DMA		      		*/
#define DALB1    0x3e	   		/* DMA5 General purpose / 2-D DMA		      		*/

#define II10   0x40	   		/* Internal DMA10 memory address		      		*/
#define IM10   0x41	   		/* Internal DMA10 memory access modifier	      	*/
#define C10    0x42	   		/* Contains number of DMA10 transfers remainnig    	*/
#define CP10   0x43	   		/* Points to next DMA10 parameters		      		*/
#define GP10   0x44	   		/* DMA10 General purpose			      			*/
#define EI10   0x45	   		/* External DMA10 address			      			*/
#define EM10   0x46	   		/* External DMA10 address modifier		      		*/
#define EC10   0x47	   		/* External DMA10 counter			      			*/

/* DMA Channel 10 Corresponds to EPB 0 */
#define IIEP0   0x40	   		/* Internal DMA10 memory address		      		*/
#define IMEP0   0x41	   		/* Internal DMA10 memory access modifier	      	*/
#define CEP0    0x42	   		/* Contains number of DMA10 transfers remainnig    	*/
#define CPEP0   0x43	   		/* Points to next DMA10 parameters		      		*/
#define GPEP0   0x44	   		/* DMA10 General purpose			      			*/
#define EIEP0   0x45	   		/* External DMA10 address			      			*/
#define EMEP0   0x46	   		/* External DMA10 address modifier		      		*/
#define ECEP0   0x47	   		/* External DMA10 counter			      			*/

#define II11   0x48	   		/* Internal DMA11 memory address		      		*/
#define IM11   0x49	   		/* Internal DMA11 memory access modifier	      	*/
#define C11    0x4a	   		/* Contains number of DMA11 transfers remainnig    	*/
#define CP11   0x4b	   		/* Points to next DMA11 parameters		      		*/
#define GP11   0x4c	   		/* DMA11 General purpose			      			*/
#define EI11   0x4d	   		/* External DMA11 address			      			*/
#define EM11   0x4e	   		/* External DMA11 address modifier		      		*/
#define EC11   0x4f	   		/* External DMA counter			      				*/

/* DMA Channel 11 Corresponds to EPB 1 */
#define IIEP1   0x48	   		/* Internal DMA11 memory address		      		*/
#define IMEP1   0x49	   		/* Internal DMA11 memory access modifier	      	*/
#define CEP1    0x4a	   		/* Contains number of DMA11 transfers remainnig    	*/
#define CPEP1   0x4b	   		/* Points to next DMA11 parameters		      		*/
#define GPEP1   0x4c	   		/* DMA11 General purpose			      			*/
#define EIEP1   0x4d	   		/* External DMA11 address			      			*/
#define EMEP1   0x4e	   		/* External DMA11 address modifier		      		*/
#define ECEP1   0x4f	   		/* External DMA counter			      				*/

#define II12   0x50	   		/* Internal DMA12 memory address		      		*/
#define IM12   0x51	   		/* Internal DMA12 memory access modifier	      	*/
#define C12    0x52	   		/* Contains number of DMA12 transfers remainnig    	*/
#define CP12   0x53	   		/* Points to next DMA12 parameters		      		*/
#define GP12   0x54	   		/* DMA12 General purpose			      			*/
#define EI12   0x55	   		/* External DMA12 address			      			*/
#define EM12   0x56	   		/* External DMA12 address modifier		      		*/
#define EC12   0x57	   		/* External DMA12 counter			      			*/

/* DMA Channel 12 Corresponds to EPB 2 */
#define IIEP2   0x50	   		/* Internal DMA12 memory address		      		*/
#define IMEP2   0x51	   		/* Internal DMA12 memory access modifier	      	*/
#define CEP2    0x52	   		/* Contains number of DMA12 transfers remainnig    	*/
#define CPEP2   0x53	   		/* Points to next DMA12 parameters		      		*/
#define GPEP2   0x54	   		/* DMA12 General purpose			      			*/
#define EIEP2   0x55	   		/* External DMA12 address			      			*/
#define EMEP2   0x56	   		/* External DMA12 address modifier		      		*/
#define ECEP2   0x57	   		/* External DMA12 counter			      			*/

#define II13   0x58	   		/* Internal DMA13 memory address		      		*/
#define IM13   0x59	   		/* Internal DMA13 memory access modifier	      	*/
#define C13    0x5a	   		/* Contains number of DMA13 transfers remainnig    	*/
#define CP13   0x5b	   		/* Points to next DMA13 parameters		      		*/
#define GP13   0x5c	   		/* DMA13 General purpose			      			*/
#define EI13   0x5d	   		/* External DMA13 address			      			*/
#define EM13   0x5e	   		/* External DMA13 address modifier		      		*/
#define EC13   0x5f	   		/* External DMA13 counter			      			*/

/* DMA Channel 13 Corresponds to EPB 3 */
#define IIEP3   0x58	   		/* Internal DMA13 memory address		      		*/
#define IMEP3   0x59	   		/* Internal DMA13 memory access modifier	      	*/
#define CEP3    0x5a	   		/* Contains number of DMA13 transfers remainnig    	*/
#define CPEP3   0x5b	   		/* Points to next DMA13 parameters		      		*/
#define GPEP3   0x5c	   		/* DMA13 General purpose			      			*/
#define EIEP3   0x5d	   		/* External DMA13 address			      			*/
#define EMEP3   0x5e	   		/* External DMA13 address modifier		      		*/
#define ECEP3   0x5f	   		/* External DMA13 counter			      			*/

#define II0    0x60	   		/* Internal DMA0 memory address		      			*/
#define IM0    0x61	   		/* Internal DMA0 memory access modifier	      		*/
#define C0     0x62	   		/* Contains number of DMA0 transfers remainnig     	*/
#define CP0    0x63	   		/* Points to next DMA0 parameters		      		*/
#define GP0    0x64	   		/* DMA0 General purpose / 2-D DMA		      		*/
#define DB0    0x65	   		/* DMA0 General purpose / 2-D DMA		      		*/
#define DA0    0x66	   		/* DMA0 General purpose / 2-D DMA		      		*/

/* DMA Channel 0 Corresponds to SPORT 0 RX */
#define IIRX0    0x60	   		/* Internal DMA0 memory address		      			*/
#define IMRX0    0x61	   		/* Internal DMA0 memory access modifier	      		*/
#define CRX0     0x62	   		/* Contains number of DMA0 transfers remainnig     	*/
#define CPRX0    0x63	   		/* Points to next DMA0 parameters		      		*/
#define GPRX0    0x64	   		/* DMA0 General purpose / 2-D DMA		      		*/
#define DBRX0    0x65	   		/* DMA0 General purpose / 2-D DMA		      		*/
#define DARX0    0x66	   		/* DMA0 General purpose / 2-D DMA		      		*/

#define II1    0x68	   		/* Internal DMA1 memory address		      			*/
#define IM1    0x69	   		/* Internal DMA1 memory access modifier	      		*/
#define C1     0x6a	   		/* Contains number of DMA1 transfers remainnig     	*/
#define CP1    0x6b	   		/* Points to next DMA1 parameters		      		*/
#define GP1    0x6c	   		/* DMA1 General purpose / 2-D DMA		      		*/
#define DB1    0x6d	   		/* DMA1 General purpose / 2-D DMA		      		*/
#define DA1    0x6e	   		/* DMA1 General purpose / 2-D DMA		      		*/

/* DMA Channel 1 Corresponds to SPORT 1 RX */
#define IIRX1    0x68	   		/* Internal DMA1 memory address		      			*/
#define IMRX1    0x69	   		/* Internal DMA1 memory access modifier	      		*/
#define CRX1     0x6a	   		/* Contains number of DMA1 transfers remainnig     	*/
#define CPRX1    0x6b	   		/* Points to next DMA1 parameters		      		*/
#define GPRX1    0x6c	   		/* DMA1 General purpose / 2-D DMA		      		*/
#define DBRX1    0x6d	   		/* DMA1 General purpose / 2-D DMA		      		*/
#define DARX1    0x6e	   		/* DMA1 General purpose / 2-D DMA		      		*/

#define II2    0x70	   		/* Internal DMA2 memory address		      			*/
#define IM2    0x71	   		/* Internal DMA2 memory access modifier	      		*/
#define C2     0x72	   		/* Contains number of DMA2 transfers remainnig     	*/
#define CP2    0x73	   		/* Points to next DMA2 parameters		      		*/
#define GP2    0x74	   		/* DMA2 General purpose / 2-D DMA		      		*/
#define DB2    0x75	   		/* DMA2 General purpose / 2-D DMA		      		*/
#define DA2    0x76	   		/* DMA2 General purpose / 2-D DMA		      		*/

/* DMA Channel 2 Corresponds to SPORT 0 TX */
#define IITX0    0x70	   		/* Internal DMA2 memory address		      			*/
#define IMTX0    0x71	   		/* Internal DMA2 memory access modifier	      		*/
#define CTX0     0x72	   		/* Contains number of DMA2 transfers remainnig     	*/
#define CPTX0    0x73	   		/* Points to next DMA2 parameters		      		*/
#define GPTX0    0x74	   		/* DMA2 General purpose / 2-D DMA		      		*/
#define DBTX0    0x75	   		/* DMA2 General purpose / 2-D DMA		      		*/
#define DATX0    0x76	   		/* DMA2 General purpose / 2-D DMA		      		*/

#define II3    0x78	   		/* Internal DMA3 memory address		      			*/
#define IM3    0x79	   		/* Internal DMA3 memory access modifier	      		*/
#define C3     0x7a	   		/* Contains number of DMA3 transfers remainnig     	*/
#define CP3    0x7b	   		/* Points to next DMA3 parameters		      		*/
#define GP3    0x7c	   		/* DMA3 General purpose / 2-D DMA		      		*/
#define DB3    0x7d	   		/* DMA3 General purpose / 2-D DMA		      		*/
#define DA3    0x7e	   		/* DMA3 General purpose / 2-D DMA		      		*/

/* DMA Channel 3 Corresponds to SPORT 1 TX */
#define IITX1    0x78	   		/* Internal DMA3 memory address		      			*/
#define IMTX1    0x79	   		/* Internal DMA3 memory access modifier	      		*/
#define CTX1     0x7a	   		/* Contains number of DMA3 transfers remainnig     	*/
#define CPTX1    0x7b	   		/* Points to next DMA3 parameters		      		*/
#define GPTX1    0x7c	   		/* DMA3 General purpose / 2-D DMA		      		*/
#define DBTX1    0x7d	   		/* DMA3 General purpose / 2-D DMA		      		*/
#define DATX1    0x7e	   		/* DMA3 General purpose / 2-D DMA		      		*/

#define II6    0x80	   		/* Internal DMA6 memory address		      			*/
#define IM6    0x81	   		/* Internal DMA6 memory access modifier	      		*/
#define C6     0x82	  		/* Contains number of DMA6 transfers remainnig     	*/
#define CP6    0x83	   		/* Points to next DMA6 parameters		      		*/
#define GP6    0x84	   		/* DMA6 General purpose / 2-D DMA		      		*/
#define DB6    0x85	   		/* DMA6 General purpose / 2-D DMA		      		*/
#define DA6    0x86	   		/* DMA6 General purpose / 2-D DMA		      		*/

/* DMA Channel 6 Corresponds to LBUF 2 */
#define IILB2    0x80	   		/* Internal DMA6 memory address		      			*/
#define IMLB2    0x81	   		/* Internal DMA6 memory access modifier	      		*/
#define CLB2     0x82	  		/* Contains number of DMA6 transfers remainnig     	*/
#define CPLB2    0x83	   		/* Points to next DMA6 parameters		      		*/
#define GPLB2    0x84	   		/* DMA6 General purpose / 2-D DMA		      		*/
#define DBLB2    0x85	   		/* DMA6 General purpose / 2-D DMA		      		*/
#define DALB2    0x86	   		/* DMA6 General purpose / 2-D DMA		      		*/

#define II7    0x88	   		/* Internal DMA7 memory address		      			*/
#define IM7    0x89	   		/* Internal DMA7 memory access modifier	      		*/
#define C7     0x8a	   		/* Contains number of DMA7 transfers remainnig     	*/
#define CP7    0x8b	   		/* Points to next DMA7 parameters		      		*/
#define GP7    0x8c	   		/* DMA7 General purpose / 2-D DMA		      		*/
#define DB7    0x8d	   		/* DMA7 General purpose / 2-D DMA		      		*/
#define DA7    0x8e	   		/* DMA7 General purpose / 2-D DMA		      		*/

/* DMA Channel 7 Corresponds to LBUF 3 */
#define IILB3    0x88	   		/* Internal DMA7 memory address		      			*/
#define IMLB3    0x89	   		/* Internal DMA7 memory access modifier	      		*/
#define CLB3     0x8a	   		/* Contains number of DMA7 transfers remainnig     	*/
#define CPLB3    0x8b	   		/* Points to next DMA7 parameters		      		*/
#define GPLB3    0x8c	   		/* DMA7 General purpose / 2-D DMA		      		*/
#define DBLB3    0x8d	   		/* DMA7 General purpose / 2-D DMA		      		*/
#define DALB3    0x8e	   		/* DMA7 General purpose / 2-D DMA		      		*/

#define II8    0x90	   		/* Internal DMA8 memory address		      			*/
#define IM8    0x91	   		/* Internal DMA8 memory access modifier	      		*/
#define C8     0x92	   		/* Contains number of DMA8 transfers remainnig     	*/
#define CP8    0x93	   		/* Points to next DMA8 parameters		      		*/
#define GP8    0x94	   		/* DMA8 General purpose / 2-D DMA		      		*/
#define DB8    0x95	   		/* DMA8 General purpose / 2-D DMA		      		*/
#define DA8    0x96	   		/* DMA8 General purpose / 2-D DMA		      		*/

/* DMA Channel 8 Corresponds to LBUF 4 */
#define IILB4    0x90	   		/* Internal DMA8 memory address		      			*/
#define IMLB4    0x91	   		/* Internal DMA8 memory access modifier	      		*/
#define CLB4     0x92	   		/* Contains number of DMA8 transfers remainnig     	*/
#define CPLB4    0x93	   		/* Points to next DMA8 parameters		      		*/
#define GPLB4    0x94	   		/* DMA8 General purpose / 2-D DMA		      		*/
#define DBLB4    0x95	   		/* DMA8 General purpose / 2-D DMA		      		*/
#define DALB4    0x96	   		/* DMA8 General purpose / 2-D DMA		      		*/

#define II9    0x98	   		/* Internal DMA9 memory address		      			*/
#define IM9    0x99	   		/* Internal DMA9 memory access modifier	      		*/
#define C9     0x9a	   		/* Contains number of DMA9 transfers remainnig     	*/
#define CP9    0x9b	   		/* Points to next DMA9 parameters		      		*/
#define GP9    0x9c	   		/* DMA9 General purpose / 2-D DMA		      		*/
#define DB9    0x9d	   		/* DMA9 General purpose / 2-D DMA		      		*/
#define DA9    0x9e	   		/* DMA9 General purpose / 2-D DMA		      		*/

/* DMA Channel 9 Corresponds to LBUF 5 */
#define IILB5    0x98	   		/* Internal DMA9 memory address		      			*/
#define IMLB5    0x99	   		/* Internal DMA9 memory access modifier	      		*/
#define CLB5     0x9a	   		/* Contains number of DMA9 transfers remainnig     	*/
#define CPLB5    0x9b	   		/* Points to next DMA9 parameters		      		*/
#define GPLB5    0x9c	   		/* DMA9 General purpose / 2-D DMA		      		*/
#define DBLB5    0x9d	   		/* DMA9 General purpose / 2-D DMA		      		*/
#define DALB5    0x9e	   		/* DMA9 General purpose / 2-D DMA		      		*/

/* Emulation/Breakpoint Registers (remapped from UREG space) 					*/
/*  NOTES:
       - These registers are ONLY accessible by the core
       - It is *highly* recommended that these facilities be accessed only
         through the ADI emulator routines
*/
/* Core Emulation HWBD Registers */
#define PSA1S  0xa0        /* Instruction address start #1                    	*/
#define PSA1E  0xa1        /* Instruction address end   #1                    	*/
#define PSA2S  0xa2        /* Instruction address start #2                    	*/
#define PSA2E  0xa3        /* Instruction address end   #2                    	*/
#define PSA3S  0xa4        /* Instruction address start #3                    	*/
#define PSA3E  0xa5        /* Instruction address end   #3                    	*/
#define PSA4S  0xa6        /* Instruction address start #4                    	*/
#define PSA4E  0xa7        /* Instruction address end   #4                    	*/
#define PMDAS  0xa8        /* Program Data address start                      	*/
#define PMDAE  0xa9        /* Program Data address end                        	*/
#define DMA1S  0xaa        /* Data address start #1                           	*/
#define DMA1E  0xab        /* Data address end   #1                           	*/
#define DMA2S  0xac        /* Data address start #2                           	*/
#define DMA2E  0xad        /* Data address end   #2                           	*/
#define EMUN   0xae        /* hwbp hit-count register                         	*/

/* IOP Emulation HWBP Bounds Registers */
#define	IOAS	0xb0		/* IOA Upper Bounds Register                       	*/
#define	IOAE	0xb1		/* IOA Lower Bounds Register                       	*/
#define	EPAS	0xb2		/* EPA Upper Bounds Register                       	*/
#define	EPAE	0xb3		/* EPA Lower Bounds Register                       	*/

#define LBUF0  0xc0			/* Link buffer 0				      				*/
#define LBUF1  0xc2	   		/* Link buffer 1				      				*/
#define LBUF2  0xc4	   		/* Link buffer 2				      				*/
#define LBUF3  0xc6	   		/* Link buffer 3				      				*/
#define LBUF4  0xc8	   		/* Link buffer 4				      				*/
#define LBUF5  0xca	   		/* Link buffer 5				      				*/
#define LCTL0  0xcc	   		/* Link buffer control			      				*/
#define LCTL1  0xcd	   		/* Link buffer control			      				*/
#define LCOM   0xce	   		/* Link common control			      				*/
#define LAR    0xcf	   		/* Link assignment register			      			*/
#define LSRQ   0xd0	   		/* Link service request and mask register	      	*/
#define LPATH1 0xd1	   		/* Link path register 1			      				*/
#define LPATH2 0xd2	   		/* Link path register 2			      				*/
#define LPATH3 0xd3	   		/* Link path register 3			      				*/
#define LPCNT  0xd4	   		/* Link path counter				      			*/
#define CNST1  0xd5	   		/* Link port constant 1 register		      		*/
#define CNST2  0xd6	   		/* Link port constant 2 register		      		*/

#define STCTL0	0xe0       /* Serial Port 0 Transmit Control Register          	*/
#define SRCTL0	0xe1       /* Serial Port 0 Receive  Control Register           */
#define TX0   	0xe2       /* Serial Port 0 Transmit Data Buffer                */
#define RX0   	0xe3       /* Serial Port 0 Receive Data Buffer                 */
#define TDIV0 	0xe4       /* Serial Port 0 Transmit Divisor                    */
#define TCNT0 	0xe5       /* Serial Port 0 Transmit Count Reg                  */
#define RDIV0 	0xe6       /* Serial Port 0 Receive Divisor                     */
#define RCNT0 	0xe7       /* Serial Port 0 Receive Count Reg                   */
#define MTCS0 	0xe8       /* Serial Port 0 Mulitchannel Transmit Selector      */
#define MRCS0 	0xe9       /* Serial Port 0 Mulitchannel Receive Selector       */
#define MTCCS0	0xea       /* Serial Port 0 Mulitchannel Transmit Selector      */
#define MRCCS0	0xeb       /* Serial Port 0 Mulitchannel Receive Selector       */
#define KEYWD0  0xec       /* Serial Port 0 Receive Comparison Register         */
#define KEYMASK0 0xed      /* Serial Port 0 Receive Comparison Mask Register    */
#define SPATH0	0xee       /* Serial Port 0 Path Length (Mesh Multiprocessing)  */
#define SPCNT0	0xef       /* Serial Port 0 Path Counter (Mesh Multiprocessing) */

#define STCTL1	0xf0       /* Serial Port 1 Transmit Control Register           */
#define SRCTL1	0xf1       /* Serial Port 1 Receive  Control Register           */
#define TX1   	0xf2       /* Serial Port 1 Transmit Data Buffer                */
#define RX1   	0xf3       /* Serial Port 1 Receive Data Buffer                 */
#define TDIV1 	0xf4       /* Serial Port 1 Transmit Divisor                    */
#define TCNT1 	0xf5       /* Serial Port 1 Transmit Count Reg                  */
#define RDIV1 	0xf6       /* Serial Port 1 Receive Divisor                     */
#define RCNT1 	0xf7       /* Serial Port 1 Receive Count Reg                   */
#define MTCS1 	0xf8       /* Serial Port 1 Mulitchannel Transmit Selector      */
#define MRCS1 	0xf9       /* Serial Port 1 Mulitchannel Receive Selector       */
#define MTCCS1	0xfa       /* Serial Port 1 Mulitchannel Transmit Selector      */
#define MRCCS1	0xfb       /* Serial Port 1 Mulitchannel Receive Selector       */
#define KEYWD1  0xfc       /* Serial Port 1 Receive Comparison Register         */
#define KEYMASK1 0xfd      /* Serial Port 1 Receive Comparison Mask Register    */
#define SPATH1	0xfe       /* Serial Port 1 Path Length (Mesh Multiprocessing)  */
#define SPCNT1	0xff       /* Serial Port 1 Path Counter (Mesh Multiprocessing) */


/*------------------------------------------------------------------------------*/
/*                System Register bit definitions                             	*/
/*------------------------------------------------------------------------------*/
/* MODE1 and MMASK registers */
#define	BR8		BIT_0	/* Bit  0: Bit-reverse for I8						*/
#define	BR0		BIT_1	/* Bit  1: Bit-reverse for I0 (uses DMS0- only )	*/
#define	SRCU	BIT_2	/* Bit  2: Alt. register select for comp. units		*/
#define	SRD1H	BIT_3	/* Bit  3: DAG1 alt. register select (7-4)			*/
#define	SRD1L	BIT_4	/* Bit  4: DAG1 alt. register select (3-0)			*/
#define	SRD2H	BIT_5	/* Bit  5: DAG2 alt. register select (15-12)		*/
#define	SRD2L	BIT_6	/* Bit  6: DAG2 alt. register select (11-8)			*/
#define	SRRFH	BIT_7	/* Bit  7: Register file alt. select for R(15-8)	*/
#define	SRRFL	BIT_10	/* Bit 10: Register file alt. select for R(7-0)		*/
#define	NESTM	BIT_11	/* Bit 11: Interrupt nesting enable					*/
#define	IRPTEN	BIT_12	/* Bit 12: Global interrupt enable					*/
#define	ALUSAT	BIT_13	/* Bit 13: Enable ALU fixed-pt. saturation			*/
#define	SSE		BIT_14	/* Bit 14: Enable short word sign extension			*/
#define	TRUNCATE	BIT_15	/* Bit 15: 1=fltg-pt. truncation 0=Rnd to nearest	*/
#define	RND32	BIT_16	/* Bit 16: 1=32-bit fltg-pt.rounding 0=40-bit rnd	*/
#define	CSEL	(BIT_17|BIT_18)	/* Bit 17-18: CSelect: Bus Mastership			   	*/
#define	PEYEN	BIT_21	/* Bit 21: Processing Element Y enable			   	*/
#define	SIMD	BIT_21	/* Bit 21: Enable SIMD Mode						   	*/
#define	BDCST9	BIT_22	/* Bit 22: Load Broadcast for I9				   	*/
#define	BDCST1	BIT_23	/* Bit 23: Load Broadcast for I1				   	*/
#define	CBUFEN	BIT_24	/* Bit 23: Circular Buffer Enable				   	*/

/* MODE2 register */
#define	IRQ0E	BIT_0	/* Bit  0: IRQ0- 1=edge sens. 0=level sens.		   	*/
#define	IRQ1E	BIT_1	/* Bit  1: IRQ1- 1=edge sens. 0=level sens.		   	*/
#define	IRQ2E	BIT_2	/* Bit  2: IRQ2- 1=edge sens. 0=level sens.		   	*/
#define	CADIS	BIT_4	/* Bit  4: Cache disable						   	*/
#define	TIMEN	BIT_5	/* Bit  5: Timer enable                            	*/
#define	BUSLK	BIT_6	/* Bit  6: External bus lock                       	*/
#define	FLG0O	BIT_15	/* Bit 15: FLAG0 1=output 0=input                  	*/
#define	FLG1O	BIT_16	/* Bit 16: FLAG1 1=output 0=input                  	*/
#define	FLG2O	BIT_17	/* Bit 17: FLAG2 1=output 0=input                  	*/
#define	FLG3O	BIT_18	/* Bit 18: FLAG3 1=output 0=input                  	*/
#define	CAFRZ	BIT_19	/* Bit 19: Cache freeze                            	*/
#define	IIRAE	BIT_20	/* Bit 20: Illegal IOP Register Access Enable      	*/
#define	U64MAE	BIT_21	/* Bit 21: Unaligned 64-bit Memory Access Enable   	*/
/* bits 31-30, 27-25 are Processor Type[4:0], read only, value: 0b01001
   bits 29-28    are silicon revision[1:0], read only, value: 0

   These bits (only) are routed to Mode2 Shadow register (IOP register ox11)
*/

/* FLAGS register */
#define FLG0    BIT_0 /* Bit 0: FLAG0 value                              	*/
#define FLG1    BIT_1 /* Bit 1: FLAG1 value                              	*/
#define FLG2    BIT_2 /* Bit 2: FLAG2 value                              	*/
#define FLG3    BIT_3 /* Bit 3: FLAG3 value                              	*/

/* ASTATx and ASTATy registers */
#ifdef SUPPORT_DEPRECATED_USAGE
/* Several of these (AV, AC, MV, SV, SZ) are assembler-reserved keywords,
   so this style is now deprecated.  If these are defined, the assembler-
   reserved keywords are still available in lowercase, e.g.,
           IF sz JUMP LABEL1.
*/
#  define AZ    BIT_0 /* Bit  0: ALU result zero or fltg-pt. underflow */
#  define AV    BIT_1 /* Bit  1: ALU overflow                          */
#  define AN    BIT_2 /* Bit  2: ALU result negative                   */
#  define AC    BIT_3 /* Bit  3: ALU fixed-pt. carry                   */
#  define AS    BIT_4 /* Bit  4: ALU X input sign (ABS and MANT ops)   */
#  define AI    BIT_5 /* Bit  5: ALU fltg-pt. invalid operation        */
#  define MN    BIT_6 /* Bit  6: Multiplier result negative            */
#  define MV    BIT_7 /* Bit  7: Multiplier overflow                   */
#  define MU    BIT_8 /* Bit  8: Multiplier fltg-pt. underflow         */
#  define MI    BIT_9 /* Bit  9: Multiplier fltg-pt. invalid operation */
#  define AF    BIT_10 /* Bit 10: ALU fltg-pt. operation                */
#  define SV    BIT_11 /* Bit 11: Shifter overflow                      */
#  define SZ    BIT_12 /* Bit 12: Shifter result zero                   */
#  define SS    BIT_13 /* Bit 13: Shifter input sign                    */
#  define BTF   BIT_18 /* Bit 18: Bit test flag for system registers    */
#  define CACC0 BIT_24 /* Bit 24: Compare Accumulation Bit 0            */
#  define CACC1 BIT_25 /* Bit 25: Compare Accumulation Bit 1            */
#  define CACC2 BIT_26 /* Bit 26: Compare Accumulation Bit 2            */
#  define CACC3 BIT_27 /* Bit 27: Compare Accumulation Bit 3            */
#  define CACC4 BIT_28 /* Bit 28: Compare Accumulation Bit 4            */
#  define CACC5 BIT_29 /* Bit 29: Compare Accumulation Bit 5            */
#  define CACC6 BIT_30 /* Bit 30: Compare Accumulation Bit 6            */
#  define CACC7 BIT_31 /* Bit 31: Compare Accumulation Bit 7            */

#endif

#define ASTAT_AZ      BIT_0 /* Bit  0: ALU result zero or fltg-pt. u'flow*/
#define ASTAT_AV      BIT_1 /* Bit  1: ALU overflow                      */
#define ASTAT_AN      BIT_2 /* Bit  2: ALU result negative               */
#define ASTAT_AC      BIT_3 /* Bit  3: ALU fixed-pt. carry               */
#define ASTAT_AS      BIT_4 /* Bit  4: ALU X input sign(ABS and MANT ops)*/
#define ASTAT_AI      BIT_5 /* Bit  5: ALU fltg-pt. invalid operation    */
#define ASTAT_MN      BIT_6 /* Bit  6: Multiplier result negative        */
#define ASTAT_MV      BIT_7 /* Bit  7: Multiplier overflow               */
#define ASTAT_MU      BIT_8 /* Bit  8: Multiplier fltg-pt. underflow     */
#define ASTAT_MI      BIT_9 /* Bit  9: Multiplier fltg-pt. invalid op.   */
#define ASTAT_AF      BIT_10 /* Bit 10: ALU fltg-pt. operation            */
#define ASTAT_SV      BIT_11 /* Bit 11: Shifter overflow                  */
#define ASTAT_SZ      BIT_12 /* Bit 12: Shifter result zero               */
#define ASTAT_SS      BIT_13 /* Bit 13: Shifter input sign                */
#define ASTAT_BTF     BIT_18 /* Bit 18: Bit test flag for system registers*/
#define ASTAT_CACC0   BIT_24 /* Bit 24: Compare Accumulation Bit 0        */
#define ASTAT_CACC1   BIT_25 /* Bit 25: Compare Accumulation Bit 1        */
#define ASTAT_CACC2   BIT_26 /* Bit 26: Compare Accumulation Bit 2        */
#define ASTAT_CACC3   BIT_27 /* Bit 27: Compare Accumulation Bit 3        */
#define ASTAT_CACC4   BIT_28 /* Bit 28: Compare Accumulation Bit 4        */
#define ASTAT_CACC5   BIT_29 /* Bit 29: Compare Accumulation Bit 5        */
#define ASTAT_CACC6   BIT_30 /* Bit 30: Compare Accumulation Bit 6        */
#define ASTAT_CACC7   BIT_31 /* Bit 31: Compare Accumulation Bit 7        */

/* STKYx and STKYy registers */
#define AUS     BIT_0 /* Bit  0: ALU fltg-pt. underflow                  	*/
#define AVS     BIT_1 /* Bit  1: ALU fltg-pt. overflow                   	*/
#define AOS     BIT_2 /* Bit  2: ALU fixed-pt. overflow                  	*/
#define AIS     BIT_5 /* Bit  5: ALU fltg-pt. invalid operation          	*/
#define MOS     BIT_6 /* Bit  6: Multiplier fixed-pt. overflow           	*/
#define MVS     BIT_7 /* Bit  7: Multiplier fltg-pt. overflow            	*/
#define MUS     BIT_8 /* Bit  8: Multiplier fltg-pt. underflow           	*/
#define MIS     BIT_9 /* Bit  9: Multiplier fltg-pt. invalid operation   	*/
#define CB7S    BIT_17 /* Bit 17: DAG1 circular buffer 7 overflow         	*/
#define CB15S   BIT_18 /* Bit 18: DAG2 circular buffer 15 overflow        	*/
#define PCFL    BIT_21 /* Bit 21: PC stack full                           	*/
#define PCEM    BIT_22 /* Bit 22: PC stack empty                          	*/
#define SSOV    BIT_23 /* Bit 23: Status stack overflow (MODE1 and ASTAT) 	*/
#define SSEM    BIT_24 /* Bit 24: Status stack empty                      	*/
#define LSOV    BIT_25 /* Bit 25: Loop stack overflow                     	*/
#define LSEM    BIT_26 /* Bit 26: Loop stack empty                        	*/

/* STKYx register *ONLY* */
#define IIRA    BIT_19 /* Bit 19: Illegal IOP Register Access             	*/
#define U64MA   BIT_20 /* Bit 20: Unaligned 64-bit Memory Access          	*/

/* IRPTL and IMASK and IMASKP registers */
#define EMUI    BIT_0 /* Bit  0: Offset: 00: Emulator Interrupt          	*/
#define RSTI    BIT_1 /* Bit  1: Offset: 04: Reset                       	*/
#define IICDI   BIT_2 /* Bit  2: Offset: 08: Illegal Input Condition Detected */
#define SOVFI   BIT_3 /* Bit  3: Offset: 0c: Stack overflow              	*/
#define TMZHI   BIT_4 /* Bit  4: Offset: 10: Timer = 0 (high priority)   	*/
#define VIRPTI  BIT_5 /* Bit  5: Offset: 14: Vector interrupt            	*/
#define IRQ2I   BIT_6 /* Bit  6: Offset: 18: IRQ2- asserted              	*/
#define IRQ1I   BIT_7 /* Bit  7: Offset: 1c: IRQ1- asserted              	*/
#define IRQ0I   BIT_8 /* Bit  8: Offset: 20: IRQ0- asserted              	*/
#define SPR0I	BIT_10 /* Bit 10: Offset: 28: SPORT0 receive DMA channel  	*/
#define SPR1I	BIT_11 /* Bit 11: Offset: 2c: SPORT1 receive DMA channel  	*/
#define SPT0I	BIT_12 /* Bit 12: Offset: 30: SPORT0 transmit DMA channel 	*/
#define SPT1I	BIT_13 /* Bit 13: Offset: 34: SPORT1 transmit DMA channel 	*/
#define LPISUMI	BIT_14 /* Bit 14: Offset: na: LPort Interrupt Summary     	*/
#define EP0I	BIT_15 /* Bit 15: Offset: 50: External port channel 0 DMA 	*/
#define EP1I	BIT_16 /* Bit 16: Offset: 54: External port channel 1 DMA 	*/
#define EP2I	BIT_17 /* Bit 17: Offset: 58: External port channel 2 DMA 	*/
#define EP3I	BIT_18 /* Bit 18: Offset: 5c: External port channel 3 DMA 	*/
#define LSRQI	BIT_19 /* Bit 19: Offset: 60: Link service request	      	*/
#define CB7I    BIT_20 /* Bit 20: Offset: 64: Circ. buffer 7 overflow     	*/
#define CB15I   BIT_21 /* Bit 21: Offset: 68: Circ. buffer 15 overflow    	*/
#define TMZLI   BIT_22 /* Bit 22: Offset: 6c: Timer = 0 (low priority)    	*/
#define FIXI    BIT_23 /* Bit 23: Offset: 70: Fixed-pt. overflow          	*/
#define FLTOI   BIT_24 /* Bit 24: Offset: 74: fltg-pt. overflow           	*/
#define FLTUI   BIT_25 /* Bit 25: Offset: 78: fltg-pt. underflow          	*/
#define FLTII   BIT_26 /* Bit 26: Offset: 7c: fltg-pt. invalid            	*/
#define SFT0I   BIT_27 /* Bit 27: Offset: 80: user software int 0         	*/
#define SFT1I   BIT_28 /* Bit 28: Offset: 84: user software int 1         	*/
#define SFT2I   BIT_29 /* Bit 39: Offset: 88: user software int 2         	*/
#define SFT3I   BIT_30 /* Bit 30: Offset: 8c: user software int 3         	*/

/* LIRPTL register */
#define LP0I	BIT_0 /* Bit  0: Offset: 38: Link port channel 0 DMA 		*/
#define LP1I	BIT_1 /* Bit  1: Offset: 3C: Link port channel 1 DMA 		*/
#define LP2I	BIT_2 /* Bit  2: Offset: 40: Link port channel 2 DMA 		*/
#define LP3I	BIT_3 /* Bit  3: Offset: 44: Link port channel 3 DMA 		*/
#define LP4I	BIT_4 /* Bit  4: Offset: 48: Link port channel 4 DMA 		*/
#define LP5I	BIT_5 /* Bit  5: Offset: 4C: Link port channel 5 DMA 		*/
#define LP0MSK	BIT_16 /* Bit 16: Link port channel 0 Interrupt Mask 		*/
#define LP1MSK	BIT_17 /* Bit 17: Link port channel 1 Interrupt Mask 		*/
#define LP2MSK	BIT_18 /* Bit 18: Link port channel 2 Interrupt Mask 		*/
#define LP3MSK	BIT_19 /* Bit 19: Link port channel 3 Interrupt Mask 		*/
#define LP4MSK	BIT_20 /* Bit 20: Link port channel 4 Interrupt Mask 		*/
#define LP5MSK	BIT_21 /* Bit 21: Link port channel 5 Interrupt Mask 		*/
#define LP0MSKP	BIT_24 /* Bit 24: Link port channel 0 Interrupt Mask Pointer*/
#define LP1MSKP	BIT_25 /* Bit 25: Link port channel 1 Interrupt Mask Pointer*/
#define LP2MSKP	BIT_26 /* Bit 26: Link port channel 2 Interrupt Mask Pointer*/
#define LP3MSKP	BIT_27 /* Bit 27: Link port channel 3 Interrupt Mask Pointer*/
#define LP4MSKP	BIT_28 /* Bit 28: Link port channel 4 Interrupt Mask Pointer*/
#define LP5MSKP	BIT_29 /* Bit 29: Link port channel 5 Interrupt Mask Pointer*/


/*------------------------------------------------------------------------------*/
/*                         IOP Register Bit Definitions                       	*/
/*------------------------------------------------------------------------------*/
/* SYSCON Register */
#define SRST   BIT_0  /* Soft Reset				      					*/
#define BSO    BIT_1  /* Boot Select Override			      				*/
#define IIVT   BIT_2  /* Internal Interrupt Vector Table		      		*/
#define IWT    BIT_3  /* Instruction word transfer (0 = data, 1 = inst)  	*/
#define HPM000 0x00000000  /* Host packing mode: None			      			*/
#define HPM001 BIT_4  /* Host packing mode: 16/48			      			*/
#define HPM010 BIT_5  /* Host packing mode: 16/64			      			*/
#define HPM011 (BIT_4|BIT_5)  /* Host packing mode: 32/48		      			*/
#define HPM100 BIT_6  /* Host packing mode: 32/64			      			*/
#define HMSWF  BIT_7  /* Host packing order (0 = LSW first, 1 = MSW)     	*/
#define HPFLSH BIT_8  /* Host pack flush				      				*/
#define IMDW0  BIT_9  /* Internal memory block 0, extended data (40 bit) 	*/
#define IMDW1  BIT_10  /* Internal memory block 1, extended data (40 bit) 	*/
#define ADREDY BIT_11  /* Active Drive Ready 								*/

#define BHD    BIT_16  /* Buffer Hand Disable			      				*/
#define EBPR00 0x00000000  /* External bus priority: Even		      			*/
#define EBPR01 BIT_17  /* External bus priority: Core has priority	      	*/
#define EBPR10 BIT_18  /* External bus priority: IO has priority	      	*/

#define DCPR   BIT_19  /* Select rotating access priority on DMA10 - DMA13	*/
#define LDCPR  BIT_20  /* Select rotating access priority on DMA4 - DMA9  	*/
#define PRROT  BIT_21  /* Select rotating prio between LPort and EPort    	*/
#define COD    BIT_22  /* Clock Out Disable                               	*/

#define IMGR   BIT_28  /* Internal memory block grouping (for the MSP)    	*/

/* SYSTAT Register */
#define HSTM   BIT_0  /* Host is the Bus Master			      			*/
#define BSYN   BIT_1  /* Bus arbitration logic is synchronized	      		*/
#define CRBM   (BIT_4|BIT_5|BIT_6)  /* Current ADSP21160 Bus Master		      			*/
#define IDC    (BIT_8|BIT_9|BIT_10)  /* ADSP21160 ID Code				      				*/
#define DWPD   BIT_12  /* Direct write pending (0 = none, 1 = pending)    	*/
#define VIPD   BIT_13  /* Vector interrupt pending (1 = pending)	      	*/
#define HPS    (BIT_14|BIT_15)  /* Host pack status				     				*/
#define CRAT   (BIT_16|BIT_17|BIT_18)  /* CLK_CFG(3-0), Core:CLKIN clock ratio	      		*/

/* WAIT Register */
#define EB0AM1	BIT_0	/* Synchronous, writes are 0 wait state */
#define EB0AM2	BIT_1	/* Synchronous, writes are 1 wait state */
#define EB0WS1	BIT_2	/* 1 Waitstate, no hold time cycle */
#define EB0WS2	BIT_3	/* 2 Waitstates, hold time cycle */
#define EB0WS3	(BIT_2|BIT_3)	/* 3 Waitstates, hold time cycle */
#define EB0WS4	BIT_4	/* 4 Waitstates, hold time cycle */
#define EB0WS5	(BIT_2|BIT_4) /* 5 Waitstates, hold time cycle */
#define EB0WS6	(BIT_3|BIT_4) /* 6 Waitstates, hold time cycle */
#define EB0WS7	(BIT_2|BIT_3|BIT_4) /* 7 Waitstates, hold time cycle */
#define EB1AM1	BIT_5	/* Synchronous, writes are 0 wait state */
#define EB1AM2	BIT_6	/* Synchronous, writes are 1 wait state */
#define EB1WS1	BIT_7	/* 1 Waitstate, no hold time cycle */
#define EB1WS2	BIT_8	/* 2 Waitstates, hold time cycle */
#define EB1WS3	(BIT_7|BIT_8)	/* 3 Waitstates, hold time cycle */
#define EB1WS4	BIT_9	/* 4 Waitstates, hold time cycle */
#define EB1WS5	(BIT_7|BIT_9) /* 5 Waitstates, hold time cycle */
#define EB1WS6	(BIT_8|BIT_9) /* 6 Waitstates, hold time cycle */
#define EB1WS7	(BIT_7|BIT_8|BIT_9) /* 7 Waitstates, hold time cycle */
#define EB2AM1	BIT_10	/* Synchronous, writes are 0 wait state */
#define EB2AM2	BIT_11	/* Synchronous, writes are 1 wait state */
#define EB2WS1	BIT_12	/* 1 Waitstate, no hold time cycle */
#define EB2WS2	BIT_13	/* 2 Waitstates, hold time cycle */
#define EB2WS3	(BIT_12|BIT_13)	/* 3 Waitstates, hold time cycle */
#define EB2WS4	BIT_14	/* 4 Waitstates, hold time cycle */
#define EB2WS5	(BIT_12|BIT_14) /* 5 Waitstates, hold time cycle */
#define EB2WS6	(BIT_13|BIT_14) /* 6 Waitstates, hold time cycle */
#define EB2WS7	(BIT_12|BIT_13|BIT_14) /* 7 Waitstates, hold time cycle */
#define EB3AM1	BIT_15	/* Synchronous, writes are 0 wait state */
#define EB3AM2	BIT_16	/* Synchronous, writes are 1 wait state */
#define EB3WS1	BIT_17	/* 1 Waitstate, no hold time cycle */
#define EB3WS2	BIT_18	/* 2 Waitstates, hold time cycle */
#define EB3WS3	(BIT_17|BIT_18)	/* 3 Waitstates, hold time cycle */
#define EB3WS4	BIT_19	/* 4 Waitstates, hold time cycle */
#define EB3WS5	(BIT_17|BIT_19) /* 5 Waitstates, hold time cycle */
#define EB3WS6	(BIT_18|BIT_19) /* 6 Waitstates, hold time cycle */
#define EB3WS7	(BIT_17|BIT_18|BIT_19) /* 7 Waitstates, hold time cycle */
#define UBAM1	BIT_20	/* Synchronous, writes are 0 wait state */
#define UBAM2	BIT_21	/* Synchronous, writes are 1 wait state */
#define UBWS1	BIT_22	/* 1 Waitstate, no hold time cycle */
#define UBWS2	BIT_23	/* 2 Waitstates, hold time cycle */
#define UBWS3	(BIT_22|BIT_23)	/* 3 Waitstates, hold time cycle */
#define UBWS4	BIT_24	/* 4 Waitstates, hold time cycle */
#define UBWS5	(BIT_22|BIT_24) /* 5 Waitstates, hold time cycle */
#define UBWS6	(BIT_23|BIT_24) /* 6 Waitstates, hold time cycle */
#define UBWS7	(BIT_22|BIT_23|BIT_24) /* 7 Waitstates, hold time cycle */
#define PAGSZ0	BIT_25		/* 512 word DRAM page size */
#define PAGSZ1	BIT_26		/* 1024 word (1K) DRAM page size */
#define PAGSZ2	(BIT_25|BIT_26) /* 2048 word (2K) DRAM page size */
#define PAGSZ4	BIT_27		/* 4096 word (4K) DRAM page size */
#define PAGSZ8	(BIT_25|BIT_27) /* 8192 word (8K) DRAM page size */
#define PAGSZ16	(BIT_26|BIT_27) /* 16384 word (16K) DRAM page size */
#define PAGSZ32	(BIT_25|BIT_26|BIT_27) /* 32768 word (32K) DRAM page size */
#define HIDMA	BIT_30		/* Handshake Idle Cycle for DMA */

/* LAR Register */
#define A0LB0	0x00000000 /*Assign link buffer 0 to link port 0.						*/
#define A0LB1	BIT_0 /*Assign link buffer 0 to link port 1.						*/
#define A0LB2	BIT_1 /*Assign link buffer 0 to link port 2.						*/
#define A0LB3	(BIT_0|BIT_1) /*Assign link buffer 0 to link port 3.						*/
#define A0LB4	BIT_2 /*Assign link buffer 0 to link port 4.						*/
#define A0LB5	(BIT_0|BIT_2) /*Assign link buffer 0 to link port 5.						*/
#define A1LB0	0x00000000 /*Assign link buffer 0 to link port 0.						*/
#define A1LB1	BIT_3 /*Assign link buffer 1 to link port 1.						*/
#define A1LB2	BIT_4 /*Assign link buffer 1 to link port 2.						*/
#define A1LB3	(BIT_3|BIT_4) /*Assign link buffer 1 to link port 3.						*/
#define A1LB4	BIT_5 /*Assign link buffer 1 to link port 4.						*/
#define A1LB5	(BIT_3|BIT_5) /*Assign link buffer 1 to link port 5.						*/
#define A2LB0	0x00000000 /*Assign link buffer 2 to link port 0.						*/
#define A2LB1	BIT_6 /*Assign link buffer 2 to link port 1.						*/
#define A2LB2	BIT_7 /*Assign link buffer 2 to link port 2.						*/
#define A2LB3	(BIT_6|BIT_7) /*Assign link buffer 2 to link port 3.						*/
#define A2LB4	BIT_8 /*Assign link buffer 2 to link port 4.						*/
#define A2LB5	(BIT_7|BIT_8) /*Assign link buffer 2 to link port 5.						*/
#define A3LB0	0x00000000 /*Assign link buffer 3 to link port 0.						*/
#define A3LB1	BIT_9 /*Assign link buffer 3 to link port 1.						*/
#define A3LB2	BIT_10 /*Assign link buffer 3 to link port 2.						*/
#define A3LB3	(BIT_9|BIT_10) /*Assign link buffer 3 to link port 3.						*/
#define A3LB4	BIT_11 /*Assign link buffer 3 to link port 4.						*/
#define A3LB5	(BIT_9|BIT_11) /*Assign link buffer 3 to link port 5.						*/
#define A4LB0	0x00000000 /*Assign link buffer 4 to link port 0.						*/
#define A4LB1	BIT_12 /*Assign link buffer 4 to link port 1.						*/
#define A4LB2	BIT_13 /*Assign link buffer 4 to link port 2.						*/
#define A4LB3	(BIT_12|BIT_13) /*Assign link buffer 4 to link port 3.						*/
#define A4LB4	BIT_14 /*Assign link buffer 4 to link port 4.						*/
#define A4LB5	(BIT_12|BIT_14) /*Assign link buffer 4 to link port 5.						*/
#define A5LB0	0x00000000 /*Assign link buffer 5 to link port 0.						*/
#define A5LB1	BIT_15 /*Assign link buffer 5 to link port 1.						*/
#define A5LB2	BIT_16 /*Assign link buffer 5 to link port 2.						*/
#define A5LB3	(BIT_15|BIT_16) /*Assign link buffer 5 to link port 3.						*/
#define A5LB4	BIT_17 /*Assign link buffer 5 to link port 4.						*/
#define A5LB5	(BIT_15|BIT_17) /*Assign link buffer 5 to link port 5.						*/


/* LCTL0 Register */
#define L0EN			BIT_0	/* LBUF0 Enable										*/
#define L0DEN			BIT_1	/* LBUF0 DMA Enable									*/
#define L0CHEN		BIT_2	/* LBUF0 DMA Chaining Enable						*/
#define L0TRAN		BIT_3	/* LBUF0 Transmit (1=Transmit, 0=Receive)			*/
#define L0EXT			BIT_4	/* LBUF0 Extended Word Size (1 = 48-bit, 0 = 32-bit)*/
#define L0CLKD0		BIT_5	/* LBUF0 Clock Divisor 0 (01 = 1, 10 = 2, 11 = 3)	*/
#define L0CLKD1		BIT_6	/* LBUF0 Clock Divisor 1 (00 = 4)					*/
#define L0DMA2D		BIT_7	/* LBUF0 2-Dimensional DMA Enable					*/
#define L0PDRDE		BIT_8	/* LPORT0 Pulldown Resistor Disable					*/
#define L0DPWID		BIT_9	/* LBUF0 Data Path Width (1 = 8-bit, 0 = 4-bit)		*/

#define L1EN			BIT_10	/* LBUF1 Enable										*/
#define L1DEN			BIT_11	/* LBUF1 DMA Enable									*/
#define L1CHEN		BIT_12	/* LBUF1 DMA Chaining Enable						*/
#define L1TRAN		BIT_13	/* LBUF1 Transmit (1=Transmit, 0=Receive)			*/
#define L1EXT			BIT_14	/* LBUF1 Extended Word Size (1 = 48-bit, 0 = 32-bit)*/
#define L1CLKD0		BIT_15	/* LBUF1 Clock Divisor 0 (01 = 1, 10 = 2, 11 = 3)	*/
#define L1CLKD1		BIT_16	/* LBUF1 Clock Divisor 1 (00 = 4)					*/
#define L1DMA2D		BIT_17	/* LBUF1 2-Dimensional DMA Enable					*/
#define L1PDRDE		BIT_18	/* LPORT1 Pulldown Resistor Disable					*/
#define L1DPWID		BIT_19	/* LBUF1 Data Path Width (1 = 8-bit, 0 = 4-bit)		*/

#define L2EN			BIT_20	/* LBUF2 Enable										*/
#define L2DEN			BIT_21	/* LBUF2 DMA Enable									*/
#define L2CHEN		BIT_22	/* LBUF2 DMA Chaining Enable						*/
#define L2TRAN		BIT_23	/* LBUF2 Transmit (1=Transmit, 0=Receive)			*/
#define L2EXT			BIT_24	/* LBUF2 Extended Word Size (1 = 48-bit, 0 = 32-bit)*/
#define L2CLKD0		BIT_25	/* LBUF2 Clock Divisor 0 (01 = 1, 10 = 2, 11 = 3)	*/
#define L2CLKD1		BIT_26	/* LBUF2 Clock Divisor 1 (00 = 4)					*/
#define L2DMA2D		BIT_27	/* LBUF2 2-Dimensional DMA Enable					*/
#define L2PDRDE		BIT_28	/* LPORT2 Pulldown Resistor Disable					*/
#define L2DPWID		BIT_29	/* LBUF2 Data Path Width (1 = 8-bit, 0 = 4-bit)		*/


/* LCTL1 Register */
#define L3EN			BIT_0	/* LBUF3 Enable										*/
#define L3DEN			BIT_1	/* LBUF3 DMA Enable									*/
#define L3CHEN		BIT_2	/* LBUF3 DMA Chaining Enable						*/
#define L3TRAN		BIT_3	/* LBUF3 Transmit (1=Transmit, 0=Receive)			*/
#define L3EXT			BIT_4	/* LBUF3 Extended Word Size (1 = 48-bit, 0 = 32-bit)*/
#define L3CLKD0		BIT_5	/* LBUF3 Clock Divisor 0 (01 = 1, 10 = 2, 11 = 3)	*/
#define L3CLKD1		BIT_6	/* LBUF3 Clock Divisor 1 (00 = 4)					*/
#define L3DMA2D		BIT_7	/* LBUF3 2-Dimensional DMA Enable					*/
#define L3PDRDE		BIT_8	/* LPORT3 Pulldown Resistor Disable					*/
#define L3DPWID		BIT_9	/* LBUF3 Data Path Width (1 = 8-bit, 0 = 4-bit)		*/

#define L4EN			BIT_10	/* LBUF4 Enable										*/
#define L4DEN			BIT_11	/* LBUF4 DMA Enable									*/
#define L4CHEN		BIT_12	/* LBUF4 DMA Chaining Enable						*/
#define L4TRAN		BIT_13	/* LBUF4 Transmit (1=Transmit, 0=Receive)			*/
#define L4EXT			BIT_14	/* LBUF4 Extended Word Size (1 = 48-bit, 0 = 32-bit)*/
#define L4CLKD0		BIT_15	/* LBUF4 Clock Divisor 0 (01 = 1, 10 = 2, 11 = 3)	*/
#define L4CLKD1		BIT_16	/* LBUF4 Clock Divisor 1 (00 = 4)					*/
#define L4DMA2D		BIT_17	/* LBUF4 2-Dimensional DMA Enable					*/
#define L4PDRDE		BIT_18	/* LPORT4 Pulldown Resistor Disable					*/
#define L4DPWID		BIT_19	/* LBUF4 Data Path Width (1 = 8-bit, 0 = 4-bit)		*/

#define L5EN			BIT_20	/* LBUF5 Enable										*/
#define L5DEN			BIT_21	/* LBUF5 DMA Enable									*/
#define L5CHEN		BIT_22	/* LBUF5 DMA Chaining Enable						*/
#define L5TRAN		BIT_23	/* LBUF5 Transmit (1=Transmit, 0=Receive)			*/
#define L5EXT			BIT_24	/* LBUF5 Extended Word Size (1 = 48-bit, 0 = 32-bit)*/
#define L5CLKD0		BIT_25	/* LBUF5 Clock Divisor 0 (01 = 1, 10 = 2, 11 = 3)	*/
#define L5CLKD1		BIT_26	/* LBUF5 Clock Divisor 1 (00 = 4)					*/
#define L5DMA2D		BIT_27	/* LBUF5 2-Dimensional DMA Enable					*/
#define L5PDRDE		BIT_28	/* LPORT5 Pulldown Resistor Disable					*/
#define L5DPWID		BIT_29	/* LBUF5 Data Path Width (1 = 8-bit, 0 = 4-bit)		*/

/* LCOM Register */
#define L0STAT0		BIT_0	/* LBUF0 Status 0 (11=full, 00=empty)				*/
#define L0STAT1		BIT_1	/* LBUF0 Status 1 (10=partially full, 01=reserved)	*/
#define L1STAT0		BIT_2	/* LBUF1 Status 0 (11=full, 00=empty)				*/
#define L1STAT1		BIT_3	/* LBUF1 Status 1 (10=partially full, 01=reserved)	*/
#define L2STAT0		BIT_4	/* LBUF2 Status 0 (11=full, 00=empty)				*/
#define L2STAT1		BIT_5	/* LBUF2 Status 1 (10=partially full, 01=reserved)	*/
#define L3STAT0		BIT_6	/* LBUF3 Status 0 (11=full, 00=empty)				*/
#define L3STAT1		BIT_7	/* LBUF3 Status 1 (10=partially full, 01=reserved)	*/
#define L4STAT0		BIT_8	/* LBUF2 Status 0 (11=full, 00=empty)				*/
#define L4STAT1		BIT_9	/* LBUF2 Status 1 (10=partially full, 01=reserved)	*/
#define L5STAT0		BIT_10	/* LBUF5 Status 0 (11=full, 00=empty)				*/
#define L5STAT1		BIT_11	/* LBUF5 Status 1 (10=partially full, 01=reserved)	*/
#define LRERR0		BIT_26	/* LBUF0 Rx Error Status (1=incomplete, 0=complete) */
#define LRERR1		BIT_27	/* LBUF1 Rx Error Status (1=incomplete, 0=complete) */
#define LRERR2		BIT_28	/* LBUF2 Rx Error Status (1=incomplete, 0=complete) */
#define LRERR3		BIT_29	/* LBUF3 Rx Error Status (1=incomplete, 0=complete) */
#define LRERR4		BIT_30	/* LBUF4 Rx Error Status (1=incomplete, 0=complete) */
#define LRERR5		BIT_31	/* LBUF5 Rx Error Status (1=incomplete, 0=complete) */

/* LSRQ Register */
#define L0TM		BIT_4	/* LPORT0 Transmit Mask								*/
#define L0RM		BIT_5	/* LPORT0 Receive Mask								*/
#define L1TM		BIT_6	/* LPORT1 Transmit Mask								*/
#define L1RM		BIT_7	/* LPORT1 Receive Mask								*/
#define L2TM		BIT_8	/* LPORT2 Transmit Mask								*/
#define L2RM		BIT_9	/* LPORT2 Receive Mask								*/
#define L3TM		BIT_10	/* LPORT3 Transmit Mask								*/
#define L3RM		BIT_11	/* LPORT3 Receive Mask								*/
#define L4TM		BIT_12	/* LPORT4 Transmit Mask								*/
#define L4RM		BIT_13	/* LPORT4 Receive Mask								*/
#define L5TM		BIT_14	/* LPORT5 Transmit Mask								*/
#define L5RM		BIT_15	/* LPORT5 Receive Mask								*/
#define L0TRQ		BIT_20	/* LPORT0 Transmit Request Status					*/
#define L0RRQ		BIT_21	/* LPORT0 Receive Request Status					*/
#define L1TRQ		BIT_22	/* LPORT1 Transmit Request Status					*/
#define L1RRQ		BIT_23	/* LPORT1 Receive Request Status					*/
#define L2TRQ		BIT_24	/* LPORT2 Transmit Request Status					*/
#define L2RRQ		BIT_25	/* LPORT2 Receive Request Status					*/
#define L3TRQ		BIT_26	/* LPORT3 Transmit Request Status					*/
#define L3RRQ		BIT_27	/* LPORT3 Receive Request Status					*/
#define L4TRQ		BIT_28	/* LPORT4 Transmit Request Status					*/
#define L4RRQ		BIT_29	/* LPORT4 Receive Request Status					*/
#define L5TRQ		BIT_30	/* LPORT5 Transmit Request Status					*/
#define L5RRQ		BIT_31	/* LPORT5 Receive Request Status					*/

/* STCTL0, STCTL1, SRCTL0, SRCTL1 registers */
#define	SPEN	BIT_0	/* SPORT enable primary A channel 				*/
#define	DTYPE0	0x00000000	/* right justify, fill unused MSBs with 0s 		*/
#define DTYPE1	BIT_1	/* right justify, sign-extend into unused MSBs 	*/
#define DTYPE2	BIT_2	/* compand using mu law 						*/
#define DTYPE3	(BIT_1|BIT_2)	/* compand using a law 							*/
#define	SENDN	BIT_3	/* MSB or LSB first 							*/
#define	SLEN3	BIT_5	/* serial length 3 	*/
#define	SLEN4	(BIT_4|BIT_5)	/* serial length 4 	*/
#define	SLEN5	BIT_6	/* serial length 5 	*/
#define	SLEN6	(BIT_4|BIT_6)	/* serial length 6 	*/
#define	SLEN7	(BIT_5|BIT_6)	/* serial length 7 	*/
#define	SLEN8	(BIT_4|BIT_5|BIT_6)	/* serial length 8 	*/
#define	SLEN9	BIT_7	/* serial length 9 	*/
#define	SLEN10	(BIT_4|BIT_7)	/* serial length 10 */
#define	SLEN11	(BIT_5|BIT_7)	/* serial length 11 */
#define	SLEN12	(BIT_4|BIT_5|BIT_7)	/* serial length 12 */
#define	SLEN13	(BIT_6|BIT_7)	/* serial length 13 */
#define	SLEN14	(BIT_4|BIT_6|BIT_7)	/* serial length 14 */
#define	SLEN15	(BIT_5|BIT_6|BIT_7)	/* serial length 15 */
#define	SLEN16	(BIT_4|BIT_5|BIT_6|BIT_7)	/* serial length 16 */
#define	SLEN17	BIT_8	/* serial length 17 */
#define	SLEN18	(BIT_4|BIT_8)	/* serial length 18 */
#define	SLEN19	(BIT_5|BIT_8)	/* serial length 19 */
#define	SLEN20	(BIT_4|BIT_5|BIT_8)	/* serial length 20 */
#define	SLEN21	(BIT_6|BIT_8)	/* serial length 21 */
#define	SLEN22	(BIT_4|BIT_6|BIT_8)	/* serial length 22 */
#define	SLEN23	(BIT_5|BIT_6|BIT_8)	/* serial length 23 */
#define	SLEN24	(BIT_4|BIT_5|BIT_6|BIT_8)	/* serial length 24 */
#define	SLEN25	(BIT_7|BIT_8)	/* serial length 25 */
#define	SLEN26	(BIT_4|BIT_7|BIT_8)	/* serial length 26 */
#define	SLEN27	(BIT_5|BIT_7|BIT_8)	/* serial length 27 */
#define	SLEN28	(BIT_4|BIT_5|BIT_7|BIT_8)	/* serial length 28 */
#define	SLEN29	(BIT_6|BIT_7|BIT_8)	/* serial length 29 */
#define	SLEN30	(BIT_4|BIT_6|BIT_7|BIT_8)	/* serial length 30 */
#define	SLEN31	(BIT_5|BIT_6|BIT_7|BIT_8)	/* serial length 31 */
#define	SLEN32	(BIT_4|BIT_5|BIT_6|BIT_7|BIT_8)	/* serial length 32 */
#define PACK	BIT_9	/* 16-to-32 data packing 							*/
#define ICLK	BIT_10	/* internally ('1') or externally ('0') generated transmit or recieve SCLKx */
#define CKRE	BIT_12	/* Clock edge for data and frame sync sampling (rx) or driving (tx) */
#define TFSR	BIT_13	/* transmit frame sync (FSx) required 	*/
#define RFSR	BIT_13	/* receive frame sync (FSx) required 	*/
#define ITFS	BIT_14	/* internally generated transmit frame sync (FSx) */
#define IRFS	BIT_14	/* internally generated receive frame sync (FSx) */
#define DITFS	BIT_15	/* (I2S & DSP serial modes only) Data Independent 'tx' FSx when DDIR bit = 1 */
#define IMODE	BIT_15	/* Receive Mode comparison select (Multichannel only)*/
#define	LTFS	BIT_16	/* Active Low transmit frame sync (FSx) */
#define	LRFS	BIT_16	/* Active Low receive frame sync (FSx) */
#define	LAFS	BIT_17	/* Late (vs early) frame sync FSx */
#define SDEN	BIT_18	/* SPORT TX DMA enable*/
#define SCHEN	BIT_19	/* SPORT TX DMA chaining enable */
#define	IMAT	BIT_20	/* Multichannel Only - used in conjunction with IMODE bit */
#define D2FDMA	BIT_21	/* Two Dimension DMA Array Enable*/
#define SPL		BIT_22	/* Serial Port Loopback Enable*/
#define MCE		BIT_23	/* Multichannel Enable*/
#define MFD1	BIT_20	/* Multichannel Transmit Frame Sync Delay 1*/
#define MFD2	BIT_21	/* Multichannel Transmit Frame Sync Delay 2*/
#define MFD3	(BIT_20|BIT_21)	/* Multichannel Transmit Frame Sync Delay 3*/
#define MFD4	BIT_22	/* Multichannel Transmit Frame Sync Delay 4*/
#define MFD5	(BIT_20|BIT_22)	/* Multichannel Transmit Frame Sync Delay 5*/
#define MFD6	(BIT_21|BIT_22)	/* Multichannel Transmit Frame Sync Delay 6*/
#define MFD7	(BIT_20|BIT_21|BIT_22)	/* Multichannel Transmit Frame Sync Delay 7*/
#define MFD8	BIT_23	/* Multichannel Transmit Frame Sync Delay 8*/
#define MFD9	(BIT_20|BIT_23)	/* Multichannel Transmit Frame Sync Delay 2*/
#define MFD10	(BIT_21|BIT_23)	/* Multichannel Transmit Frame Sync Delay 3*/
#define MFD11	(BIT_20|BIT_21|BIT_23)	/* Multichannel Transmit Frame Sync Delay 4*/
#define MFD12	(BIT_22|BIT_23)	/* Multichannel Transmit Frame Sync Delay 5*/
#define MFD13	(BIT_20|BIT_21|BIT_23)	/* Multichannel Transmit Frame Sync Delay 6*/
#define MFD14	(BIT_21|BIT_22|BIT_23)	/* Multichannel Transmit Frame Sync Delay 7*/
#define MFD15	(BIT_20|BIT_21|BIT_22|BIT_23)	/* Multichannel Transmit Frame Sync Delay 8*/
#define	NCH0	0x00000000	/* 1 Channel */
#define	NCH1	BIT_24	/* 2 Channels */
#define	NCH2	BIT_25	/* 3 Channels */
#define	NCH3	(BIT_24|BIT_25)	/* 4 Channels */
#define	NCH4	BIT_26	/* 5 Channels */
#define	NCH5	(BIT_24|BIT_26)	/* 6 Channels */
#define	NCH6	(BIT_25|BIT_26)	/* 7 Channels */
#define	NCH7	(BIT_24|BIT_25|BIT_26)	/* 8 Channels */
#define	NCH8	BIT_27	/* 9 Channels */
#define	NCH9	(BIT_24|BIT_27)	/* 10 Channels */
#define	NCH10	(BIT_25|BIT_27)	/* 11 Channels */
#define	NCH11	(BIT_24|BIT_25|BIT_27)	/* 12 Channels */
#define	NCH12	(BIT_26|BIT_27)	/* 13 Channels */
#define	NCH13	(BIT_24|BIT_26|BIT_27)	/* 14 Channels */
#define	NCH14	(BIT_25|BIT_26|BIT_27)	/* 15 Channels */
#define	NCH15	(BIT_24|BIT_25|BIT_26|BIT_27)	/* 16 Channels */
#define	NCH16	BIT_28	/* 17 Channels */
#define	NCH17	(BIT_24|BIT_28)	/* 18 Channels */
#define	NCH18	(BIT_25|BIT_28)	/* 19 Channels */
#define	NCH19	(BIT_24|BIT_25|BIT_28)	/* 20 Channels */
#define	NCH20	(BIT_26|BIT_28)	/* 21 Channels */
#define	NCH21	(BIT_24|BIT_26|BIT_28)	/* 22 Channels */
#define	NCH22	(BIT_25|BIT_26|BIT_28)	/* 23 Channels */
#define	NCH23	(BIT_24|BIT_25|BIT_26|BIT_28)	/* 24 Channels */
#define	NCH24	(BIT_27|BIT_28)	/* 25 Channels */
#define	NCH25	(BIT_24|BIT_27|BIT_28)	/* 26 Channels */
#define	NCH26	(BIT_25|BIT_27|BIT_28)	/* 27 Channels */
#define	NCH27	(BIT_24|BIT_25|BIT_27|BIT_28)	/* 28 Channels */
#define	NCH28	(BIT_26|BIT_27|BIT_28)	/* 29 Channels */
#define	NCH29	(BIT_24|BIT_26|BIT_27|BIT_28)	/* 30 Channels */
#define	NCH30	(BIT_25|BIT_26|BIT_27|BIT_28)	/* 31 Channels */
#define	NCH31	(BIT_24|BIT_25|BIT_26|BIT_27|BIT_28)	/* 32 Channels */
#define	CHNL0	0x00000000	/* Channel 0 Selected (Read Only)*/
#define	CHNL1	BIT_24	/* Channel 1 Selected (Read Only)*/
#define	CHNL2	BIT_25	/* Channel 2 Selected (Read Only)*/
#define	CHNL3	(BIT_24|BIT_25)	/* Channel 3 Selected (Read Only)*/
#define	CHNL4	BIT_26	/* Channel 4 Selected (Read Only)*/
#define	CHNL5	(BIT_24|BIT_26)	/* Channel 5 Selected (Read Only)*/
#define	CHNL6	(BIT_25|BIT_26)	/* Channel 6 Selected (Read Only)*/
#define	CHNL7	(BIT_24|BIT_25|BIT_26)	/* Channel 7 Selected (Read Only)*/
#define	CHNL8	BIT_27	/* Channel 8 Selected (Read Only)*/
#define	CHNL9	(BIT_24|BIT_27)	/* Channel 9 Selected (Read Only)*/
#define	CHNL10	(BIT_25|BIT_27)	/* Channel 10 Selected (Read Only)*/
#define	CHNL11	(BIT_24|BIT_25|BIT_27)	/* Channel 11 Selected (Read Only)*/
#define	CHNL12	(BIT_26|BIT_27)	/* Channel 12 Selected (Read Only)*/
#define	CHNL13	(BIT_24|BIT_26|BIT_27)	/* Channel 13 Selected (Read Only)*/
#define	CHNL14	(BIT_25|BIT_26|BIT_27)	/* Channel 14 Selected (Read Only)*/
#define	CHNL15	(BIT_24|BIT_25|BIT_26|BIT_27)	/* Channel 15 Selected (Read Only)*/
#define	CHNL16	BIT_28	/* Channel 16 Selected (Read Only)*/
#define	CHNL17	(BIT_24|BIT_28)	/* Channel 17 Selected (Read Only)*/
#define	CHNL18	(BIT_25|BIT_28)	/* Channel 18 Selected (Read Only)*/
#define	CHNL19	(BIT_24|BIT_25|BIT_28)	/* Channel 19 Selected (Read Only)*/
#define	CHNL20	(BIT_26|BIT_28)	/* Channel 20 Selected (Read Only)*/
#define	CHNL21	(BIT_24|BIT_26|BIT_28)	/* Channel 21 Selected (Read Only)*/
#define	CHNL22	(BIT_25|BIT_26|BIT_28)	/* Channel 22 Selected (Read Only)*/
#define	CHNL23	(BIT_24|BIT_25|BIT_26|BIT_28)	/* Channel 23 Selected (Read Only)*/
#define	CHNL24	(BIT_27|BIT_28)		/* Channel 24 Selected (Read Only)*/
#define	CHNL25	(BIT_24|BIT_27|BIT_28)	/* Channel 25 Selected (Read Only)*/
#define	CHNL26	(BIT_25|BIT_27|BIT_28)	/* Channel 26 Selected (Read Only)*/
#define	CHNL27	(BIT_24|BIT_25|BIT_27|BIT_28)	/* Channel 27 Selected (Read Only)*/
#define	CHNL28	(BIT_26|BIT_27|BIT_28)	/* Channel 28 Selected (Read Only)*/
#define	CHNL29	(BIT_24|BIT_26|BIT_27|BIT_28)	/* Channel 29 Selected (Read Only)*/
#define	CHNL30	(BIT_25|BIT_26|BIT_27|BIT_28)	/* Channel 30 Selected (Read Only)*/
#define	CHNL31	(BIT_24|BIT_25|BIT_26|BIT_27|BIT_28)	/* Channel 31 Selected (Read Only)*/
#define ROVF	BIT_29	/* Receive Overflow Status (Read Only)*/
#define RXSEmpty	0x00000000	/* Transmit Data Buffer Empty (Read Only)*/
#define RXSParial	BIT_31	/* Transmit Data Partially Full (Read Only)*/
#define RXSFull		(BIT_30|BIT_31)	/* Transmit Data Full (Read Only)*/
#define TUVF	BIT_29	/* Transmit Underflow Status (Read Only)*/
#define TXSEmpty	0x00000000	/* Transmit Data Buffer Empty (Read Only)*/
#define TXSParial	BIT_31	/* Transmit Data Partially Full (Read Only)*/
#define TXSFull		(BIT_30|BIT_31)	/* Transmit Data Full (Read Only)*/

/*DMACx Registers*/
#define DEN		BIT_0	/* External port DMA enable */
#define CHEN	BIT_1	/* External port DMA chaining enable */
#define TRAN	BIT_2	/* External port transmit/receive select */
#define PS0		0x00000000	/* External port packing status, pack complete (Read only) */
#define PS1		BIT_3	/* External port packing status, 1st stage pack (Read only) */
#define PS2		BIT_4	/* External port packing status, 2nd stage multi-stage pack (Read only) */
#define DTYPE	BIT_5	/* External port data type select */
#define PMODE0	0x00000000	/* External port packing mode - no pack */
#define PMODE1	BIT_6	/* External port packing mode - 16 external to 32/64 internal */
#define PMODE2	BIT_7	/* External port packing mode - 16 external to 48 internal */
#define PMODE3	(BIT_6|BIT_7)	/* External port packing mode - 32 external to 48 internal */
#define PMODE4	BIT_8	/* External port packing mode - 32 external to 32/64 internal */
#define MSWF	BIT_9	/* Most significant 16-bit word first during packing */
#define MASTER	BIT_10	/* Master mode enable */
#define HSHAKE	BIT_11	/* Handshake mode enable */
#define INTIO	BIT_12	/* Single-word interrupt enable */
#define EXT_HANDSHAKE_EN	BIT_13	/* External handshake mode enable */
#define FLSH	BIT_14	/* Flush DMA buffers and status */
#define PRIO	BIT_15	/* External port bus priority */
#define FSe		0x00000000	/* External port FIFO buffer status - empty (read only) */
#define FSnf	BIT_16	/* External port FIFO buffer status - not full (read only) */
#define FSne	BIT_17	/* External port FIFO buffer status - not empty (read only) */
#define FSf		(BIT_16|BIT_17)	/* External port FIFO buffer status - full (read only) */
#define INT32	BIT_18	/* Internal memory 32-bit tranfers select */
#define MAXBL	BIT_19	/* Maximum burst length select */

#endif


