/*******************************************************************************
    Accelerometer Library

  Company:
    Microchip Technology Inc.

  File Name:
    accelerometer_bma250_i2c.c

  Summary:
    Contains the functional implementation of Accelerometer library.

  Description:
    This library provides a low-level abstraction of the Accelerometer device.  It
    can be used to simplify low-level access to the device without the necessity of
    interacting directly with the communication module's registers, thus hiding
    differences from one serial device variant to another.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "app.h"

#define ACCEL_BMA250_I2C_MODULE             I2C_ID_1
#define ACCEL_BMA250_I2C_VECTOR_LOCATION    _I2C_1_VECTOR
#define ACCEL_BMA250_I2C_SOURCE             INT_SOURCE_I2C_1_MASTER
#define ACCEL_BMA250_I2C_VECTOR             INT_VECTOR_I2C1
/********************************************************************
 Section: Private Macros
********************************************************************/
#define     ACCEL_BMA250_WRITE_ADDR     (0x30)
#define     ACCEL_BMA250_READ_ADDR      (0x31)

#define     ACCEL_BMA250_CHIP_ID_ADDR   (0x00)
#define     ACCEL_BMA250_VERSION_ADDR   (0x01)
#define     ACCEL_BMA250_ACC_X_LSB_ADDR (0x02)
#define     ACCEL_BMA250_ACC_X_MSB_ADDR (0x03)
#define     ACCEL_BMA250_ACC_Y_LSB_ADDR (0x04)
#define     ACCEL_BMA250_ACC_Y_MSB_ADDR (0x05)
#define     ACCEL_BMA250_ACC_Z_LSB_ADDR (0x06)
#define     ACCEL_BMA250_ACC_Z_MSB_ADDR (0x07)
#define     ACCEL_BMA250_TEMP           (0x08)
#define     ACCEL_BMA250_FIFO_SET_ADDR  (0x3e)
#define     ACCEL_BMA250_FIFO_DATA_ADDR (0x3f)

#define     ACCEL_BMA250_ADDR15         (0x15)
#define     ACCEL_BMA250_ADDR14         (0x14)
#define     ACCEL_BMA250_ADDR13         (0x13)
#define     ACCEL_BMA250_ADDR12         (0x12)
#define     ACCEL_BMA250_ADDR11         (0x11)
#define     ACCEL_BMA250_ADDR10         (0x10)
#define     ACCEL_BMA250_ADDR0F         (0x0F)
#define     ACCEL_BMA250_ADDR0E         (0x0E)
#define     ACCEL_BMA250_ADDR0D         (0x0D)
#define     ACCEL_BMA250_ADDR0C         (0x0C)
#define     ACCEL_BMA250_ADDR0B         (0x0B)
#define     ACCEL_BMA250_ADDR0A         (0x0A)
#define     ACCEL_BMA250_ADDR09         (0x09)

#define     ACCEL_BMA250_CHIP_ID        (0xF9)
#define     ACCEL_BMA250_THES           (0x00)

#define     ACCEL_BMA250_RANGE_2G       (3)
#define     ACCEL_BMA250_RANGE_4G       (5)
#define     ACCEL_BMA250_RANGE_8G       (8)
#define     ACCEL_BMA250_RANGE_16G      (12)

#define     ACCEL_BMA250_BW_25          (0x00)
#define     ACCEL_BMA250_BW_50          (0x01)
#define     ACCEL_BMA250_BW_100         (0x02)
#define     ACCEL_BMA250_BW_190         (0x03)
#define     ACCEL_BMA250_BW_375         (0x04)
#define     ACCEL_BMA250_BW_750         (0x05)
#define     ACCEL_BMA250_BW_1500        (0x06)

#define    ACCEL_BMA250_FIFO_MODE_BYPASS (0x00)
#define    ACCEL_BMA250_FIFO_MODE_FIFO   (0x01)
#define    ACCEL_BMA250_FIFO_MODE_STREAM (0x02)

#define    ACCEL_BMA250_FIFO_DATA_SELECT_XYZ (0x00 << 6)
#define    ACCEL_BMA250_FIFO_DATA_SELECT_X   (0x01 << 6)
#define    ACCEL_BMA250_FIFO_DATA_SELECT_Y   (0x02 << 6)
#define    ACCEL_BMA250_FIFO_DATA_SELECT_Z   (0x03 << 6)


#define     ACCEL_CMD_START             0x00
#define     ACCEL_CMD_RESTART           0x01
#define     ACCEL_CMD_STOP              0x02
#define     ACCEL_CMD_TX_BYTE           0x03
#define     ACCEL_CMD_RX_BYTE           0x04
#define     ACCEL_CMD_ACK               0x05
#define     ACCEL_CMD_EN_RX             0x06
#define     ACCEL_CMD_ACK_POLL          0x07
#define     ACCEL_DONE                  0xFF
#define     ACCEL_INVALID_IDX           0xFFFFFFFF

#define     ACCEL_BMA250_AXIS_BIT       (10)
/********************************************************************
 Section: Private structures
********************************************************************/
typedef struct
{
    uint8_t data;
    uint8_t cmd;
}ACCEL_CMD_DATA;

typedef union
{
    //Address 0x00
    struct
    {
        uint8_t chip_id :8;
    } ;

    //Address 0x01
    struct
    {
        uint8_t ml_version :4;
        uint8_t al_version :4;
    };

    //Address 0x02
    struct
    {
        uint8_t new_data_x :1;
        uint8_t :5;
        uint8_t acc_x_2 :2;
    } ;

    //Address 0x03
    struct
    {
        uint8_t acc_x :8;
    };

    //Address 0x04
    struct
    {
        uint8_t new_data_y :1;
        uint8_t :5;
        uint8_t acc_y_2 :2;
    } __attribute__((packed));

    //Address 0x05
    struct
    {
        uint8_t acc_y :8;
    };

    //Address 0x06
    struct
    {
        uint8_t new_data_z :1;
        uint8_t :5;
        uint8_t acc_z_2 :2;
    };

    //Address 0x07
    struct
    {
        uint8_t acc_z :8;
    };

    //Address 0x08
    struct
    {
        int acc_Temp :8;
    };

    //Address 0x14
    struct
    {
        uint8_t bandwidth :3;
        uint8_t range :2;
		uint8_t  :3;
    } ;

    uint8_t val;
} ACCEL_BMA250_REG;

typedef struct
{
    ACCEL_BMA250_REG    acc_x_lsb;
    ACCEL_BMA250_REG    acc_x_msb;
    ACCEL_BMA250_REG    acc_y_lsb;
    ACCEL_BMA250_REG    acc_y_msb;
    ACCEL_BMA250_REG    acc_z_lsb;
    ACCEL_BMA250_REG    acc_z_msb;
}ACCEL_X_Y_Z_DATA;

typedef struct
{
    uint8_t x_lsb;
    uint8_t x_msb;
    uint8_t y_lsb;
    uint8_t y_msb;
    uint8_t z_lsb;
    uint8_t z_msb;
}ACCEL_X_Y_Z_BUF;
typedef enum
{
    ACCEL_FALSE = 0,
    ACCEL_TRUE
}ACCEL_BOOL;
/********************************************************************
 Section: Gobal Structures
********************************************************************/
static ACCEL_CMD_DATA       accelData[2000];
static uint32_t             accelIdx = ACCEL_INVALID_IDX;
static uint8_t              *AccelRxBuffer;
static int32_t              SignedBuffer;
static ACCEL_X_Y_Z_DATA     accelXYZData;
static ACCEL_X_Y_Z_BUF      accelXYZBurstData[32];
volatile uint8_t accl_range;
volatile uint8_t I2CPriority = FREE_PRIORITY;
/********************************************************************
 Section: Code
********************************************************************/
/********************************************************************
 Funciton: __ISR(EEPROM_I2C_VECTOR_LOCATION, ipl2) ACCEL_BMA250InterruptHandler(void)
********************************************************************/
#if defined(ACCEL_USE_EXTERNAL_INTERUPT_HANDLER) || defined(ACCEL_USE_POLLING)
void ACCEL_BMA250InterruptHandler(void)
#else

	#if defined (__PIC32MX__)
		__ISR(ACCEL_BMA250_I2C_VECTOR_LOCATION, ipl2) ACCEL_BMA250InterruptHandler(void)
	#elif defined (__dsPIC33E__)|| defined (__PIC24E__)
		void __attribute__((interrupt, no_auto_psv)) _MI2C2Interrupt(void)
	#endif
#endif
{
    ACCEL_BOOL polling;
    ACCEL_CMD_DATA *pCmdData;
    uint32_t *pIndex;
//    uint8_t **pRxBuffer;
    uint8_t *pRxBuffer;
    
//    if(I2CPriority == ACCEL_PRIORIY)
//    {
//        pCmdData = accelData;
//        pIndex = &accelIdx;
////        pRxBuffer = &AccelRxBuffer;
//        pRxBuffer = AccelRxBuffer;
//    }
//    else
//    {
////        pCmdData = (ACCEL_CMD_DATA *)EepromI2CData;
////        pIndex = &EepromI2CIdx;
////        pRxBuffer = &EepromRxBuffer;
//    }
    
    pCmdData = accelData;
    pIndex = &accelIdx;
    pRxBuffer = AccelRxBuffer;
//    AccelRxBuffer += 4;
    
    if(SYS_INT_SourceStatusGet(ACCEL_BMA250_I2C_SOURCE))
    {
        polling = ACCEL_FALSE;

        switch(pCmdData[*pIndex].cmd)
        {
        case ACCEL_CMD_START:
            PLIB_I2C_MasterStart(ACCEL_BMA250_I2C_MODULE);
            break;

        case ACCEL_CMD_RESTART:
            PLIB_I2C_MasterStartRepeat(ACCEL_BMA250_I2C_MODULE);
            break;

        case ACCEL_CMD_STOP:
            PLIB_I2C_MasterStop(ACCEL_BMA250_I2C_MODULE);
            break;

        case ACCEL_CMD_TX_BYTE:
            PLIB_I2C_TransmitterByteSend(ACCEL_BMA250_I2C_MODULE, pCmdData[*pIndex].data);
            break;

        case ACCEL_CMD_EN_RX:
            PLIB_I2C_MasterReceiverClock1Byte(ACCEL_BMA250_I2C_MODULE);
            break;

        case ACCEL_CMD_RX_BYTE:
//            **pRxBuffer = PLIB_I2C_ReceivedByteGet(ACCEL_BMA250_I2C_MODULE);
//            (*pRxBuffer)++;
            *pRxBuffer = PLIB_I2C_ReceivedByteGet(ACCEL_BMA250_I2C_MODULE);
            pRxBuffer++;
            break;

        case ACCEL_CMD_ACK:
            PLIB_I2C_ReceivedByteAcknowledge(ACCEL_BMA250_I2C_MODULE, pCmdData[*pIndex].data);
            break;

        case ACCEL_CMD_ACK_POLL:
            polling = ACCEL_TRUE;
            if(!PLIB_I2C_TransmitterByteWasAcknowledged(ACCEL_BMA250_I2C_MODULE))

            {
                (*pIndex) -= 4;
            }
           break;

        default:
            accelData[*pIndex].cmd = ACCEL_DONE;
        case ACCEL_DONE:
#ifndef ACCEL_USE_EXTERNAL_INTERUPT_HANDLER
            SYS_INT_SourceDisable(ACCEL_BMA250_I2C_SOURCE);
            accelData[0].cmd = ACCEL_DONE;
            break;
#else
            *pIndex = ACCEL_INVALID_IDX;
            return;
#endif
        }
        SYS_INT_SourceStatusClear(ACCEL_BMA250_I2C_SOURCE);

        if(pCmdData[*pIndex].cmd != ACCEL_DONE)
        {
            if((pCmdData[*pIndex].cmd == ACCEL_CMD_RX_BYTE) || (polling))
            {
                SYS_INT_SourceStatusSet(ACCEL_BMA250_I2C_SOURCE);
            }

            (*pIndex)++;
        }else
        {
            *pIndex = ACCEL_INVALID_IDX;
            I2CPriority = FREE_PRIORITY;
        }
    }

}
/*******************************************************************************
  Function:  inline ACCEL_RESULT IsBusy(void)
  *****************************************************************************/
static bool  intStatus;
inline int IsBusy(void)
{
    uint32_t  tempAccelIdx;

    intStatus = SYS_INT_Disable();
    tempAccelIdx = accelIdx;
    if(intStatus)
    {
        SYS_INT_Enable();
    }
    return (tempAccelIdx != ACCEL_INVALID_IDX);
}
/*******************************************************************************
  Function:  inline uint8_t ReadByte(uint8_t start_idx, uint8_t addr)
  *****************************************************************************/
inline uint16_t ReadByteSequence(uint16_t start_idx, uint8_t addr)
{

    accelData[start_idx++].cmd = ACCEL_CMD_START;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = ACCEL_BMA250_WRITE_ADDR;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = addr;

    accelData[start_idx++].cmd = ACCEL_CMD_STOP;

    accelData[start_idx++].cmd = ACCEL_CMD_START;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = ACCEL_BMA250_READ_ADDR;

    accelData[start_idx++].cmd = ACCEL_CMD_EN_RX;

    accelData[start_idx++].cmd = ACCEL_CMD_RX_BYTE;

    accelData[start_idx++].cmd = ACCEL_CMD_STOP;

    return start_idx;
}
/*******************************************************************************
  Function:  inline uint8_t WriteByteSequence(uint8_t start_idx, uint8_t addr, uint8_t data)
  *****************************************************************************/
inline uint16_t WriteByteSequence(uint16_t start_idx, uint8_t addr, uint8_t data)
{
   accelData[start_idx++].cmd = ACCEL_CMD_START;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = ACCEL_BMA250_WRITE_ADDR;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = addr;

    accelData[start_idx].cmd = ACCEL_CMD_TX_BYTE;
    accelData[start_idx++].data = data;

    accelData[start_idx++].cmd = ACCEL_CMD_STOP;

    return start_idx;
}
/********************************************************************
 Funciton: UINT8 EEPROM_24LC01HWReadByte(size_t addr)
********************************************************************/
inline void ACCELStartCommandSequence(void)
{
    bool  intStatus;

    intStatus = SYS_INT_Disable();
    accelIdx = 0;
    I2CPriority = ACCEL_PRIORIY;
    if(!SYS_INT_SourceIsEnabled(ACCEL_BMA250_I2C_SOURCE))
    {
        SYS_INT_SourceStatusSet(ACCEL_BMA250_I2C_SOURCE);
    }
#ifndef ACCEL_USE_POLLING
#endif
    if(intStatus)
    {
        SYS_INT_Enable();
    }

}
/*******************************************************************************
  Function:  inline uint8_t WriteByteSequence(uint8_t start_idx, uint8_t addr, uint8_t data)
  *****************************************************************************/
inline ACCEL_RESULT ReadChipID(void)
{
    ACCEL_BMA250_REG        reg;
    uint16_t                idx;

    idx = ReadByteSequence(0, ACCEL_BMA250_CHIP_ID_ADDR);
    AccelRxBuffer = (uint8_t *)&reg;
    accelData[idx].cmd = ACCEL_DONE;
    ACCELStartCommandSequence();
#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif
    if(reg.chip_id != ACCEL_BMA250_CHIP_ID)
    {
        return ACCEL_INVALID;
    }
    return ACCEL_VALID;
}
/*******************************************************************************
  Function:  inline void ReadChipVersion(void)
  *****************************************************************************/
inline void ReadChipVersion(void)
{
    ACCEL_BMA250_REG        reg;
    uint16_t                idx;

    idx = ReadByteSequence(0, ACCEL_BMA250_VERSION_ADDR);
    AccelRxBuffer = (uint8_t *)&reg;
    accelData[idx].cmd = ACCEL_DONE;
    ACCELStartCommandSequence();
#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif

}

/*******************************************************************************
  Function:  inline void SetRangeAndBandwidth(void)
  *****************************************************************************/
inline void SetRangeAndBandwidth(uint8_t accl_range)
{
    ACCEL_BMA250_REG        reg;
    uint16_t                idx;

    idx = ReadByteSequence(0, ACCEL_BMA250_ADDR14);
    AccelRxBuffer = (uint8_t *)&reg;
    accelData[idx].cmd = ACCEL_DONE;
    ACCELStartCommandSequence();
#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif

//    reg.range       = accl_range;
//    reg.bandwidth	= ACCEL_BMA250_BW_50;
//    idx = WriteByteSequence(0, ACCEL_BMA250_ADDR14, reg.val);
    idx = WriteByteSequence(0, ACCEL_BMA250_ADDR0F, accl_range);
//    idx = WriteByteSequence(idx, ACCEL_BMA250_ADDR10, 0x08);
    idx = WriteByteSequence(idx, ACCEL_BMA250_ADDR10, 0x0f); //0x0f, bandwidth 1000Hz, updata time 0.5ms
    accelData[idx].cmd = ACCEL_DONE;
    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif
}
/*******************************************************************************
  Function:  inline void SetThershold(void)
  *****************************************************************************/
inline void SetThershold(void)
{
    ACCEL_BMA250_REG    reg;
    uint16_t                idx;

    idx = ReadByteSequence(0, ACCEL_BMA250_ADDR11);

    AccelRxBuffer = (uint8_t *)&reg;

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif

    idx = WriteByteSequence(0, ACCEL_BMA250_ADDR14, ACCEL_BMA250_THES);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif

    idx = ReadByteSequence(0, ACCEL_BMA250_ADDR11);

    AccelRxBuffer = (uint8_t *)&reg;

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif
}
/*******************************************************************************
  Function:  inline void SetFIFO(void)
  *****************************************************************************/
inline void SetFIFO(void)
{
    ACCEL_BMA250_REG    reg;
    uint16_t                idx;

    idx = WriteByteSequence(0, ACCEL_BMA250_FIFO_SET_ADDR, ACCEL_BMA250_FIFO_MODE_STREAM
                                                            | ACCEL_BMA250_FIFO_DATA_SELECT_XYZ);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID)
        ;
#endif
}
/*******************************************************************************
  Function:  inline void StartXYZAccelConversion(void)
  *****************************************************************************/
inline void StartXYZAccelConversion(void)
{
    uint16_t idx;

    AccelRxBuffer = (uint8_t *)&accelXYZData;

    idx = ReadByteSequence(0, ACCEL_BMA250_ACC_X_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA250_ACC_X_MSB_ADDR);

    idx = ReadByteSequence(idx, ACCEL_BMA250_ACC_Y_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA250_ACC_Y_MSB_ADDR);

    idx = ReadByteSequence(idx, ACCEL_BMA250_ACC_Z_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA250_ACC_Z_MSB_ADDR);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();
}
/*******************************************************************************
  Function:  inline void StartXYAccelConversion(void)
  *****************************************************************************/
inline void StartXYAccelConversion(void)
{
    uint16_t idx;

    AccelRxBuffer = (uint8_t *)&accelXYZData;

    idx = ReadByteSequence(0, ACCEL_BMA250_ACC_X_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA250_ACC_X_MSB_ADDR);

    idx = ReadByteSequence(idx, ACCEL_BMA250_ACC_Y_LSB_ADDR);
    idx = ReadByteSequence(idx, ACCEL_BMA250_ACC_Y_MSB_ADDR);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();
}
/*******************************************************************************
  Function:  inline void StartXAccelConversion(void)
  *****************************************************************************/
inline void StartXAccelConversion(void)
{
    uint16_t idx;

//    AccelRxBuffer = (uint8_t *)&accelXYZData;
    AccelRxBuffer = (uint8_t *)&accelXYZData.acc_x_msb;

//    idx = ReadByteSequence(0, ACCEL_BMA250_ACC_X_LSB_ADDR);
//    idx = ReadByteSequence(idx, ACCEL_BMA250_ACC_X_MSB_ADDR);
    
    idx = ReadByteSequence(0, ACCEL_BMA250_ACC_X_MSB_ADDR);
    
    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();
}
/*******************************************************************************
  Function:  inline void StartYAccelConversion(void)
  *****************************************************************************/
inline void StartYAccelConversion(void)
{
    uint16_t idx;

//    AccelRxBuffer = (uint8_t *)&accelXYZData.acc_y_lsb.val;
    AccelRxBuffer = (uint8_t *)&accelXYZData.acc_y_msb.val;

//    idx = ReadByteSequence(0, ACCEL_BMA250_ACC_Y_LSB_ADDR);
//    idx = ReadByteSequence(idx, ACCEL_BMA250_ACC_Y_MSB_ADDR);
    idx = ReadByteSequence(0, ACCEL_BMA250_ACC_Y_MSB_ADDR);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();
}
/*******************************************************************************
  Function:  inline void StartZAccelConversion(void)
  *****************************************************************************/
inline void StartZAccelConversion(void)
{
    uint16_t idx;
    
    AccelRxBuffer = (uint8_t *)&accelXYZData.acc_z_msb.val;

    idx = ReadByteSequence(0, ACCEL_BMA250_ACC_Z_MSB_ADDR);

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();
    
}
/*******************************************************************************
  Function:  inline void StartBurstXYZAccelConversion(uint8_t u8_BurstNumber)
  *****************************************************************************/
inline void StartBurstXYZAccelConversion(uint8_t u8_BurstNumber)
{
    uint16_t idx = 0;
    uint8_t i;
    AccelRxBuffer = (uint8_t *)accelXYZBurstData;
    for(i = 0;i < u8_BurstNumber*6;i ++)
    {
        idx = ReadByteSequence(idx, ACCEL_BMA250_FIFO_DATA_ADDR);
    }
    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELInitialize(ACCEL_INIT *initialization)
  *****************************************************************************/
ACCEL_RESULT ACCELInitialize(ACCEL_INIT *initialization, uint8_t accl_range)
{
    PLIB_I2C_BaudRateSet(ACCEL_BMA250_I2C_MODULE, initialization->sourceClock, initialization->dataRate);
    PLIB_I2C_Enable(ACCEL_BMA250_I2C_MODULE);

#ifndef ACCEL_USE_EXTERNAL_INTERUPT_HANDLER
#ifndef ACCEL_USE_POLLING
    SYS_INT_VectorPrioritySet(ACCEL_BMA250_I2C_VECTOR,INT_PRIORITY_LEVEL_2);
#endif
#endif

    if(ReadChipID() == ACCEL_INVALID)
        return ACCEL_INVALID;

    SetRangeAndBandwidth(accl_range);

    ReadChipVersion();

    SetThershold();

    SetFIFO();

    accelXYZData.acc_x_lsb.new_data_x = 0;
    accelXYZData.acc_y_lsb.new_data_y = 0;
    accelXYZData.acc_z_lsb.new_data_z = 0;

    return ACCEL_VALID;
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELTask(void)
  *****************************************************************************/
ACCEL_RESULT ACCELTask(void)
{
    if(IsBusy())
    {
        ACCEL_BMA250InterruptHandler();

        // one last check to see if we are busy
        if(IsBusy())
            return ACCEL_VALID;
    }

    return ACCEL_INVALID;
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELGetXYZAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y, ACCEL_DATA *acc_z)
  *****************************************************************************/
//ACCEL_RESULT ACCELGetXYZAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y, ACCEL_DATA *acc_z)
//{
//    if(IsBusy())
//        return ACCEL_INVALID;
//
//    if((accelXYZData.acc_x_lsb.new_data_x) && (accelXYZData.acc_y_lsb.new_data_y) && (accelXYZData.acc_z_lsb.new_data_z))
//    {
//        uint32_t data;
//        uint8_t size = 0;
//
//        if(ACCEL_BMA250_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
//            size = (uint8_t)(ACCEL_BMA250_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));
//
//        data = (uint32_t)accelXYZData.acc_x_msb.val << 8;
//        data |= (uint32_t)accelXYZData.acc_x_lsb.val;
//        data >>= 6;
//
//        *acc_x = (ACCEL_DATA)(data >> size) ;
//
//        data = (uint32_t)accelXYZData.acc_y_msb.val << 8;
//        data |= (uint32_t)accelXYZData.acc_y_lsb.val;
//        data >>= 6;
//
//        *acc_y = (ACCEL_DATA)(data >> size);
//
//        data = (uint32_t)accelXYZData.acc_z_msb.val << 8;
//        data |= (uint32_t)accelXYZData.acc_z_lsb.val;
//        data >>= 6;
//
//        *acc_z = (ACCEL_DATA)(data >> size);
//
//        accelXYZData.acc_x_lsb.new_data_x = 0;
//        accelXYZData.acc_y_lsb.new_data_y = 0;
//        accelXYZData.acc_z_lsb.new_data_z = 0;
//
//        return ACCEL_VALID;
//    }
//
//    StartXYZAccelConversion();
//    
//    return ACCEL_INVALID;
//}

ACCEL_RESULT ACCELGetXYZAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y, ACCEL_DATA *acc_z)
{  
        uint32_t data;
        uint8_t size = 0;

        if(ACCEL_BMA250_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
            size = (uint8_t)(ACCEL_BMA250_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));

        StartXYZAccelConversion();
        while(ACCELTask() == ACCEL_VALID);
        
        data = (uint32_t)accelXYZData.acc_x_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_x_lsb.val;
        data >>= 6;

        *acc_x = (ACCEL_DATA)(data >> size) ;

        data = (uint32_t)accelXYZData.acc_y_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_y_lsb.val;
        data >>= 6;

        *acc_y = (ACCEL_DATA)(data >> size);

        data = (uint32_t)accelXYZData.acc_z_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_z_lsb.val;
        data >>= 6;

        *acc_z = (ACCEL_DATA)(data >> size);

        accelXYZData.acc_x_lsb.new_data_x = 0;
        accelXYZData.acc_y_lsb.new_data_y = 0;
        accelXYZData.acc_z_lsb.new_data_z = 0;
 
        return ACCEL_VALID;
}

/*******************************************************************************
  Function:  ACCEL_RESULT ACCELGetXYAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y)
  *****************************************************************************/
ACCEL_RESULT ACCELGetXYAxis(ACCEL_DATA *acc_x, ACCEL_DATA *acc_y)
{
    if(IsBusy())
        return ACCEL_INVALID;

    if((accelXYZData.acc_x_lsb.new_data_x) && (accelXYZData.acc_y_lsb.new_data_y))
    {
        uint32_t data;
        uint8_t size = 0;

        if(ACCEL_BMA250_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
            size = (uint8_t)(ACCEL_BMA250_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));

        data = (uint32_t)accelXYZData.acc_x_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_x_lsb.val;
        data >>= 6;

        *acc_x = (ACCEL_DATA)(data >> size) ;

        data = (uint32_t)accelXYZData.acc_y_msb.val << 8;
        data |= (uint32_t)accelXYZData.acc_y_lsb.val;
        data >>= 6;

        *acc_y = (ACCEL_DATA)(data >> size);


        accelXYZData.acc_x_lsb.new_data_x = 0;
        accelXYZData.acc_y_lsb.new_data_y = 0;
        accelXYZData.acc_z_lsb.new_data_z = 0;

        return ACCEL_VALID;
    }

    StartXYAccelConversion();

    return ACCEL_INVALID;
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELGetXAxis(ACCEL_DATA *acc_x)
  *****************************************************************************/
//ACCEL_RESULT ACCELGetXAxis(ACCEL_DATA *acc_x)
//{
//    if(IsBusy())
//        return ACCEL_INVALID;
//
//    if(accelXYZData.acc_x_lsb.new_data_x)
//    {
//        uint32_t data;
//        uint8_t size = 0;
//
//        if(ACCEL_BMA250_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
//            size = (uint8_t)(ACCEL_BMA250_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));
//
//        data = (uint32_t)accelXYZData.acc_x_msb.val << 8;
//        data |= (uint32_t)accelXYZData.acc_x_lsb.val;
//        data >>= 6;
//
//        *acc_x = (ACCEL_DATA)(data >> size) ;
//
//        accelXYZData.acc_x_lsb.new_data_x = 0;
//        accelXYZData.acc_y_lsb.new_data_y = 0;
//        accelXYZData.acc_z_lsb.new_data_z = 0;
//
//        return ACCEL_VALID;
//    }
//
//    StartXAccelConversion();
//
//    return ACCEL_INVALID;
//}

//Ignore LSB and using polling Version
ACCEL_RESULT ACCELGetXAxis(ACCEL_DATA *acc_x)
{
    StartXAccelConversion();
    
    while(ACCELTask() == ACCEL_VALID);

    *acc_x = (ACCEL_DATA) accelXYZData.acc_x_msb.val ;

    return ACCEL_VALID;
}
/*******************************************************************************
  Function:  ACCEL_RESULT ACCELGetYAxis(ACCEL_DATA *acc_y)
  *****************************************************************************/
//ACCEL_RESULT ACCELGetYAxis(ACCEL_DATA *acc_y)
//{
//    if(IsBusy())
//        return ACCEL_INVALID;
//
//    if(accelXYZData.acc_y_lsb.new_data_y)
//    {
//        uint32_t data;
//        uint8_t size = 0;
//
//        if(ACCEL_BMA250_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
//            size = (uint8_t)(ACCEL_BMA250_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));
//
//        data = (uint32_t)accelXYZData.acc_y_msb.val << 8;
//        data |= (uint32_t)accelXYZData.acc_y_lsb.val;
//        data >>= 6;
//
//        *acc_y = (ACCEL_DATA)(data >> size) ;
//
//        accelXYZData.acc_x_lsb.new_data_x = 0;
//        accelXYZData.acc_y_lsb.new_data_y = 0;
//        accelXYZData.acc_z_lsb.new_data_z = 0;
//
//        return ACCEL_VALID;
//    }
//
//    StartYAccelConversion();
//
//    return ACCEL_INVALID;
//}

//Ignore LSB and using polling Version
ACCEL_RESULT ACCELGetYAxis(ACCEL_DATA *acc_y)
{

    StartYAccelConversion(); 
    while(ACCELTask() == ACCEL_VALID);

    *acc_y = (ACCEL_DATA) accelXYZData.acc_y_msb.val;

    return ACCEL_VALID;
}

/*******************************************************************************
  Function:  ACCEL_RESULT ACCELGetZAxis(ACCEL_DATA *acc_z)
  *****************************************************************************/
//ACCEL_RESULT ACCELGetZAxis(ACCEL_DATA *acc_z)
//{
//    if(IsBusy())
//        return ACCEL_INVALID;
//
//    if(accelXYZData.acc_z_lsb.new_data_z)
//    {
//        uint32_t data;
//        uint8_t size = 0;
//
//        if(ACCEL_BMA250_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
//            size = (uint8_t)(ACCEL_BMA250_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));
//
//        data = (uint32_t)accelXYZData.acc_z_msb.val << 8;
//        data |= (uint32_t)accelXYZData.acc_z_lsb.val;
//        data >>= 6;
//
//        *acc_z = (ACCEL_DATA)(data >> size) ;
//
//        accelXYZData.acc_x_lsb.new_data_x = 0;
//        accelXYZData.acc_y_lsb.new_data_y = 0;
//        accelXYZData.acc_z_lsb.new_data_z = 0;
//
//        return ACCEL_VALID;
//    }
//
//    StartZAccelConversion();
//
//    return ACCEL_INVALID;
//}

//Ignore LSB and using polling Version
ACCEL_RESULT ACCELGetZAxis(ACCEL_DATA *acc_z)
{
    
    StartZAccelConversion();
    
    while(ACCELTask() == ACCEL_VALID);
    
    *acc_z = (ACCEL_DATA) accelXYZData.acc_z_msb.val;
    
    return ACCEL_VALID;
}

ACCEL_RESULT BMA250EGetBurstXYZ( ACCEL_DATA *u16_XDataArray, ACCEL_DATA *u16_YDataArray, ACCEL_DATA *u16_ZDataArray, uint8_t u8_BurstNumber)
{
    uint8_t i;
    StartBurstXYZAccelConversion(u8_BurstNumber);
    while(IsBusy());
    
    uint8_t size = 0;

    if(ACCEL_BMA250_AXIS_BIT > (sizeof(ACCEL_DATA) * 8))
        size = (uint8_t)(ACCEL_BMA250_AXIS_BIT - (sizeof(ACCEL_DATA) * 8));

    for(i = 0;i < u8_BurstNumber;i ++)
    {
        uint32_t data;


        data = (uint32_t)accelXYZBurstData[i].x_msb << 8;
        data |= (uint32_t)accelXYZBurstData[i].x_lsb;
        data >>= 6;

        //u16_XDataArray[i] = (ACCEL_DATA)(data >> size) ;
        u16_XDataArray[i] = (ACCEL_DATA)(data) ;

        data = (uint32_t)accelXYZBurstData[i].y_msb << 8;
        data |= (uint32_t)accelXYZBurstData[i].y_lsb;
        data >>= 6;

        u16_YDataArray[i] = (ACCEL_DATA)(data) ;
        //u16_YDataArray[i] = (ACCEL_DATA)(data >> size);

        data = (uint32_t)accelXYZBurstData[i].z_msb << 8;
        data |= (uint32_t)accelXYZBurstData[i].z_lsb;
        data >>= 6;

        u16_ZDataArray[i] = (ACCEL_DATA)(data) ;
        //u16_ZDataArray[i] = (ACCEL_DATA)(data >> size);
    }

}

ACCEL_RESULT  ReadTemp(RETURN_TEMP ReturnType)
{
    ACCEL_BMA250_REG    reg;
    uint16_t            idx;
    int                 Output;

    idx = ReadByteSequence(0, ACCEL_BMA250_TEMP);

    AccelRxBuffer = (uint8_t *)&reg;

    accelData[idx].cmd = ACCEL_DONE;

    ACCELStartCommandSequence();

#ifndef ACCEL_USE_POLLING
    while(IsBusy())
        ;
#else
    while(ACCELTask() == ACCEL_VALID);
#endif

    if (ReturnType == FAHRENHEIT)
    {
        Output  = 75.2 + ((reg.acc_Temp) *0.9);
    }
    if (ReturnType == CELSIUS)
    {
        Output = 24.0 + ((reg.acc_Temp)*0.5);
    }
    if (ReturnType == RAW_DATA)
    {
        Output = (reg.acc_Temp);
    }
    if (ReturnType == KELVIN)
    {
        Output = 297.15 + ((reg.acc_Temp) * 0.5);
    }

        return Output;
}


void AccelInit(void)
{
    ACCEL_INIT  accel_init;
    accel_init.sourceClock  = APP_PERIPHERAL_BUS_FREQUENCY_GET();
    accel_init.dataRate     = 200000;
    ACCELInitialize(&accel_init, ACCEL_BMA250_RANGE_2G);
}
/*******************************************************************************
 End of File
 */