/*******************************************************************************
 System Tasks File

  File Name:
    system_tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled state
    machines.

  Description:
    This file contains source code necessary to maintain system's polled state
    machines.  It implements the "SYS_Tasks" function that calls the individual
    "Tasks" functions for all the MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
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

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"
#include "app.h"


// *****************************************************************************
// *****************************************************************************
// Section: Local Prototypes
// *****************************************************************************
// *****************************************************************************
 
static void _SYS_Tasks ( void );
void _SYS_TMR_Tasks (void);
static void _Bluetooth_Tasks (void);
static void _LEDcontrol_Tasks ( void );
static void _ACCEL_Tasks (void);
static void _shell_Tasks (void);
// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/

void SYS_Tasks ( void )
{
    /* Create OS Thread for Sys Tasks. */
    xTaskCreate((TaskFunction_t) _SYS_Tasks,
                "Sys Tasks",
                configMINIMAL_STACK_SIZE, NULL, 2, NULL);

 
    /* Create task for System Timer state machine*/
    /* Create OS Thread for SYS_TMR Tasks. */
    xTaskCreate((TaskFunction_t) _SYS_TMR_Tasks,
                "SYS_TMR Tasks",
                configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    /* Create OS Thread for APP Tasks. */
    xTaskCreate((TaskFunction_t) _Bluetooth_Tasks,
                "Bluetooth Tasks",
                1024, NULL, 1, NULL);
    
    xTaskCreate((TaskFunction_t) _LEDcontrol_Tasks,
                "LED Tasks",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    
    xTaskCreate((TaskFunction_t) _ACCEL_Tasks,
                "Accelerometer Tasks",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    
    xTaskCreate((TaskFunction_t) _shell_Tasks,
                "Shell Tasks",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    /**************
     * Start RTOS * 
     **************/
    vTaskStartScheduler(); /* This function never returns. */
}


/*******************************************************************************
  Function:
    void _SYS_Tasks ( void )

  Summary:
    Maintains state machines of system modules.
*/

static void _SYS_Tasks ( void )
{
    while(1)
    {
        /* Maintain system services */
        SYS_DEVCON_Tasks(sysObj.sysDevcon);
        /* Maintain the DMA system state machine. */
        SYS_DMA_Tasks(sysObj.sysDma);

        /* Maintain Device Drivers */
        DRV_TMR_Tasks(sysObj.drvTmr0);

        /* Maintain Middleware */

        /* Task Delay */
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void _SYS_TMR_Tasks(void)
{
    while(1)
    {
        SYS_TMR_Tasks(sysObj.sysTmr);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
 }

/*******************************************************************************
  Function:
    void _APP_Tasks ( void )

  Summary:
    Maintains state machine of APP.
*/

static void _Bluetooth_Tasks(void)
{
    while(1)
    {
        //vTaskDelay(1000 / portTICK_PERIOD_MS);
        Bluetooth_Tasks();
        
    }
}


static void _ACCEL_Tasks(void)
{
    
    while( (xAccelQueue = xQueueCreate(1, sizeof(ACCEL_XYZf)) ) == 0);
    while( (xAccelRawQueue = xQueueCreate(1, sizeof(ACCEL_XYZ_RAW)) ) == 0);
    
    while(1) {
        vTaskDelay(50 / portTICK_PERIOD_MS);
        ACCEL_Task();
    }
}

static void _LEDcontrol_Tasks ( void )
{
    while (1) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        LEDcontrol_Tasks();
    }
    
}


void shellUSARTBufferEventHandler(DRV_USART_BUFFER_EVENT event, DRV_USART_BUFFER_HANDLE bufferHandle, uintptr_t contextHandle)
{
    
}

static void _shell_Tasks (void)
{
    APP_USART_CLIENT usartClient;
    const char outStr[6] = "Test\n";
    
    /* Open a USART client and setup the buffer event handler */
    usartClient.handle = DRV_USART_Open(DRV_USART_INDEX_1,
                DRV_IO_INTENT_READWRITE|DRV_IO_INTENT_NONBLOCKING);
    if(usartClient.handle != DRV_HANDLE_INVALID)
    {
        if(DRV_USART_CLIENT_STATUS_READY == DRV_USART_ClientStatus(usartClient.handle))
        {
            DRV_USART_BufferEventHandlerSet(usartClient.handle,
            (const DRV_USART_BUFFER_EVENT_HANDLER)shellUSARTBufferEventHandler,
                (const uintptr_t)&usartClient.context);
        }
        else
        {
            SYS_DEBUG(0, "USART Driver Not Ready");
        }
    }
    else
    {
        ;
    }
    
    
    while (1) {
        char getChar = '\0';
        DRV_USART_BufferAddRead(usartClient.handle, &usartClient.readBufHandle, &getChar, 1);
        if (getChar != '\0'){
            DRV_USART_BufferAddWrite(usartClient.handle, &usartClient.writeBufHandle, &getChar, 1);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
/*******************************************************************************
 End of File
 */

