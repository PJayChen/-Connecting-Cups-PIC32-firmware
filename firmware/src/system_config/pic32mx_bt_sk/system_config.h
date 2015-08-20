/*******************************************************************************
  MPLAB Harmony System Configuration Header

  File Name:
    system_config.h

  Summary:
    Build-time configuration header for the system defined by this MPLAB Harmony
    project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options that are not instantiated
    until used by another MPLAB Harmony module or application.
    
    Created with MPLAB Harmony Version 1.04
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H

/* This is a temporary workaround for an issue with the peripheral library "Exists"
   functions that causes superfluous warnings.  It "nulls" out the definition of
   The PLIB function attribute that causes the warning.  Once that issue has been
   resolved, this definition should be removed. */
#define _PLIB_UNSUPPORTED


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/

#include "bsp_config.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Common System Service Configuration Options
*/
#define SYS_VERSION_STR           "1.04"
#define SYS_VERSION               10400

// *****************************************************************************
/* Clock System Service Configuration Options
*/
#define SYS_CLK_FREQ                        48000000ul
#define SYS_CLK_BUS_PERIPHERAL_1            48000000ul
#define SYS_CLK_UPLL_BEFORE_DIV2_FREQ       96000000ul
#define SYS_CLK_CONFIG_PRIMARY_XTAL         12000000ul
#define SYS_CLK_CONFIG_SECONDARY_XTAL       0ul

/*** Interrupt System Service Configuration ***/
#define SYS_INT                     true

/*** Ports System Service Configuration ***/

#define SYS_PORT_A_ANSEL        0x1c
#define SYS_PORT_A_TRIS         0x79c
#define SYS_PORT_A_LAT          0x0
#define SYS_PORT_A_ODC          0x0
#define SYS_PORT_A_CNPU         0x0
#define SYS_PORT_A_CNPD         0x0
#define SYS_PORT_A_CNEN         0x0

#define SYS_PORT_B_ANSEL        0xec1f
#define SYS_PORT_B_TRIS         0xef9f
#define SYS_PORT_B_LAT          0x0
#define SYS_PORT_B_ODC          0x0
#define SYS_PORT_B_CNPU         0x0
#define SYS_PORT_B_CNPD         0x0
#define SYS_PORT_B_CNEN         0x0
#define SYS_PORT_C_ANSEL        0x7
#define SYS_PORT_C_TRIS         0x1ff
#define SYS_PORT_C_LAT          0x0
#define SYS_PORT_C_ODC          0x0
#define SYS_PORT_C_CNPU         0x0
#define SYS_PORT_C_CNPD         0x0
#define SYS_PORT_C_CNEN         0x0
/*** Timer System Service Configuration ***/
#define SYS_TMR_POWER_STATE             SYS_MODULE_POWER_RUN_FULL
#define SYS_TMR_DRIVER_INDEX            DRV_TMR_INDEX_0
#define SYS_TMR_MAX_CLIENT_OBJECTS      3
#define SYS_TMR_FREQUENCY               100
#define SYS_TMR_FREQUENCY_TOLERANCE     5
#define SYS_TMR_UNIT_RESOLUTION         1000
#define SYS_TMR_CLIENT_TOLERANCE        1
#define SYS_TMR_INTERRUPT_NOTIFICATION  true

// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************

/*** Timer Driver Configuration ***/
#define DRV_TMR_INSTANCES_NUMBER           2
#define DRV_TMR_CLIENTS_NUMBER             2
#define DRV_TMR_INTERRUPT_MODE             true

/*** Timer Driver 0 Configuration ***/
#define DRV_TMR_PERIPHERAL_ID_IDX0          TMR_ID_3
#define DRV_TMR_INTERRUPT_SOURCE_IDX0       INT_SOURCE_TIMER_3
#define DRV_TMR_INTERRUPT_VECTOR_IDX0       INT_VECTOR_T3
#define DRV_TMR_ISR_VECTOR_IDX0             _TIMER_3_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX0     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX0 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX0           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX0               TMR_PRESCALE_VALUE_8
#define DRV_TMR_OPERATION_MODE_IDX0         DRV_TMR_OPERATION_MODE_16_BIT
#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX0     false
#define DRV_TMR_POWER_STATE_IDX0            SYS_MODULE_POWER_RUN_FULL


 // *****************************************************************************
/* USART Driver Configuration Options
*/

#define DRV_USART_INTERRUPT_MODE                    true
#define DRV_USART_BYTE_MODEL_SUPPORT                false
#define DRV_USART_READ_WRITE_MODEL_SUPPORT          true
#define DRV_USART_BUFFER_QUEUE_SUPPORT              true
#define DRV_USART_QUEUE_DEPTH_COMBINED              4
#define DRV_USART_CLIENTS_NUMBER                    2
#define DRV_USART_SUPPORT_TRANSMIT_DMA              true
#define DRV_USART_SUPPORT_RECEIVE_DMA               true
#define DRV_USART_INSTANCES_NUMBER                  2

#define DRV_USART_PERIPHERAL_ID_IDX0                USART_ID_2
#define DRV_USART_OPER_MODE_IDX0                    DRV_USART_OPERATION_MODE_NORMAL
#define DRV_USART_OPER_MODE_DATA_IDX0               0x00
#define DRV_USART_INIT_FLAG_WAKE_ON_START_IDX0      false
#define DRV_USART_INIT_FLAG_AUTO_BAUD_IDX0          false
#define DRV_USART_INIT_FLAG_STOP_IN_IDLE_IDX0       false
#define DRV_USART_INIT_FLAGS_IDX0                   0
#define DRV_USART_BRG_CLOCK_IDX0                    48000000
#define DRV_USART_BAUD_RATE_IDX0                    115200
#define DRV_USART_LINE_CNTRL_IDX0                   DRV_USART_LINE_CONTROL_8NONE1
#define DRV_USART_HANDSHAKE_MODE_IDX0               DRV_USART_HANDSHAKE_FLOWCONTROL
#define DRV_USART_XMIT_INT_SRC_IDX0                 INT_SOURCE_USART_2_TRANSMIT
#define DRV_USART_RCV_INT_SRC_IDX0                  INT_SOURCE_USART_2_RECEIVE
#define DRV_USART_ERR_INT_SRC_IDX0                  INT_SOURCE_USART_2_ERROR
#define DRV_USART_XMIT_QUEUE_SIZE_IDX0              1
#define DRV_USART_RCV_QUEUE_SIZE_IDX0               1
#define DRV_USART_XMIT_DMA_CH_IDX0                  DMA_CHANNEL_0
#define DRV_USART_XMIT_DMA_INT_SRC_IDX0             INT_SOURCE_DMA_0
#define DRV_USART_RCV_DMA_CH_IDX0                   DMA_CHANNEL_3
#define DRV_USART_RCV_DMA_INT_SRC_IDX0              INT_SOURCE_DMA_3
#define DRV_USART_POWER_STATE_IDX0                  SYS_MODULE_POWER_RUN_FULL

#define DRV_USART_PERIPHERAL_ID_IDX1                USART_ID_1
#define DRV_USART_OPER_MODE_IDX1                    DRV_USART_OPERATION_MODE_NORMAL
#define DRV_USART_OPER_MODE_DATA_IDX1               0x00
#define DRV_USART_INIT_FLAG_WAKE_ON_START_IDX1      false
#define DRV_USART_INIT_FLAG_AUTO_BAUD_IDX1          false
#define DRV_USART_INIT_FLAG_STOP_IN_IDLE_IDX1       false
#define DRV_USART_INIT_FLAGS_IDX1                   0
#define DRV_USART_BRG_CLOCK_IDX1                    48000000
#define DRV_USART_BAUD_RATE_IDX1                    9600
#define DRV_USART_LINE_CNTRL_IDX1                   DRV_USART_LINE_CONTROL_8NONE1
#define DRV_USART_HANDSHAKE_MODE_IDX1               DRV_USART_HANDSHAKE_FLOWCONTROL
#define DRV_USART_XMIT_INT_SRC_IDX1                 INT_SOURCE_USART_1_TRANSMIT
#define DRV_USART_RCV_INT_SRC_IDX1                  INT_SOURCE_USART_1_RECEIVE
#define DRV_USART_ERR_INT_SRC_IDX1                  INT_SOURCE_USART_1_ERROR
#define DRV_USART_XMIT_QUEUE_SIZE_IDX1              1
#define DRV_USART_RCV_QUEUE_SIZE_IDX1               1
#define DRV_USART_XMIT_DMA_CH_IDX1                  DMA_CHANNEL_1
#define DRV_USART_XMIT_DMA_INT_SRC_IDX1                INT_SOURCE_DMA_1
#define DRV_USART_RCV_DMA_CH_IDX1                DMA_CHANNEL_2
#define DRV_USART_RCV_DMA_INT_SRC_IDX1                INT_SOURCE_DMA_2
#define DRV_USART_POWER_STATE_IDX1                  SYS_MODULE_POWER_RUN_FULL


// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************

/*** OSAL Configuration ***/
#define OSAL_USE_RTOS          1

#define APP_BT_TICK_TIMER_MS                            10
#define APP_PERIPHERAL_BUS_FREQUENCY_GET()               SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_2)

/* BT connection name */
#define APP_BT_SPP_CONNECTION_NAME                      "BTSK"

#define APP_BT_USART_BAUD_CLOCK                         SYS_CLK_BUS_PERIPHERAL_1
#define APP_BT_USART_WORKING_BAUD_RATE                  4000000

/* BT Reset PORT settings */
#define APP_BT_RESET_PORT                               PORT_CHANNEL_B
#define APP_BT_RESET_BIT                                PORTS_BIT_POS_5


/* PPS functions for RGB LED pins */
#define APP_RGB_LED_RED_OC_FUNCTION                     OTPUT_FUNC_OC1
#define APP_RGB_LED_RED_OC_PIN                          OUTPUT_PIN_RPA0
#define APP_RGB_LED_GREEN_OC_FUNCTION                   OTPUT_FUNC_OC2
#define APP_RGB_LED_GREEN_OC_PIN                        OUTPUT_PIN_RPA1
#define APP_RGB_LED_BLUE_OC_FUNCTION                    OTPUT_FUNC_OC3
#define APP_RGB_LED_BLUE_OC_PIN                         OUTPUT_PIN_RPC9

/* ID's for RGB LED */
#define APP_TMR_ID_FOR_OC                               TMR_ID_2
#define APP_RGB_LED_RED_OC_ID                           OC_ID_1
#define APP_RGB_LED_GREEN_OC_ID                         OC_ID_2
#define APP_RGB_LED_BLUE_OC_ID                          OC_ID_3



#endif // _SYSTEM_CONFIG_H
/*******************************************************************************
 End of File
*/

