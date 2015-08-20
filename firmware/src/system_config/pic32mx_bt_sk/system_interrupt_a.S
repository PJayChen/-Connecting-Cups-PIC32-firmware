/*******************************************************************************
 System Interrupt Source File

  File Name:
    sys_interrupt_a.S

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the 
    interrupt sub-system.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END
/*
*********************************************************************************************************
*                                           INCLUDES
*********************************************************************************************************
*/
#include <xc.h>

#include "ISR_Support.h"



/* TMR Instance 0 Interrupt */

   .extern  IntHandlerDrvTmrInstance0

   .section	.vector_12,code, keep
   .equ     __vector_dispatch_12, IntVectorDrvTmrInstance0
   .global  __vector_dispatch_12
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance0

IntVectorDrvTmrInstance0:
    la    $26,  _IntHandlerDrvTmrInstance0         
    jr    $26
    nop
	.end	IntVectorDrvTmrInstance0

   .section	.DrvTmrInstance0_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerDrvTmrInstance0

_IntHandlerDrvTmrInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerDrvTmrInstance0




/* Sytem DMA Instance 0 Interrupt */

   .extern  IntHandlerSysDmaInstance0

   .section	.vector_40,code, keep
   .equ     __vector_dispatch_40, IntVectorSysDmaInstance0
   .global  __vector_dispatch_40
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorSysDmaInstance0

IntVectorSysDmaInstance0:
    la    $26,  _IntHandlerSysDmaInstance0         
    jr    $26
    nop
	.end	IntVectorSysDmaInstance0

   .section	.SysDmaInstance0_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerSysDmaInstance0

_IntHandlerSysDmaInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerSysDmaInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerSysDmaInstance0


/* Sytem DMA Instance 1 Interrupt */

   .extern  IntHandlerSysDmaInstance1

   .section	.vector_43,code, keep
   .equ     __vector_dispatch_43, IntVectorSysDmaInstance1
   .global  __vector_dispatch_43
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorSysDmaInstance1

IntVectorSysDmaInstance1:
    la    $26,  _IntHandlerSysDmaInstance1         
    jr    $26
    nop
	.end	IntVectorSysDmaInstance1

   .section	.SysDmaInstance1_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerSysDmaInstance1

_IntHandlerSysDmaInstance1:
    portSAVE_CONTEXT
    la    s6,  IntHandlerSysDmaInstance1
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerSysDmaInstance1


/* Sytem DMA Instance 2 Interrupt */

   .extern  IntHandlerSysDmaInstance2

   .section	.vector_41,code, keep
   .equ     __vector_dispatch_41, IntVectorSysDmaInstance2
   .global  __vector_dispatch_41
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorSysDmaInstance2

IntVectorSysDmaInstance2:
    la    $26,  _IntHandlerSysDmaInstance2         
    jr    $26
    nop
	.end	IntVectorSysDmaInstance2

   .section	.SysDmaInstance2_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerSysDmaInstance2

_IntHandlerSysDmaInstance2:
    portSAVE_CONTEXT
    la    s6,  IntHandlerSysDmaInstance2
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerSysDmaInstance2


/* Sytem DMA Instance 3 Interrupt */

   .extern  IntHandlerSysDmaInstance3

   .section	.vector_42,code, keep
   .equ     __vector_dispatch_42, IntVectorSysDmaInstance3
   .global  __vector_dispatch_42
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorSysDmaInstance3

IntVectorSysDmaInstance3:
    la    $26,  _IntHandlerSysDmaInstance3         
    jr    $26
    nop
	.end	IntVectorSysDmaInstance3

   .section	.SysDmaInstance3_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerSysDmaInstance3

_IntHandlerSysDmaInstance3:
    portSAVE_CONTEXT
    la    s6,  IntHandlerSysDmaInstance3
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerSysDmaInstance3

/*******************************************************************************
 End of File
 */

