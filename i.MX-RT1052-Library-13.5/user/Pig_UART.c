/*!
* @file       Pig_UART.c
* @brief      ���ݹٷ���fsl_lpuart���һЩ����Ľӿ�
* @details    ����UART�����š�ʱ����Ҫ�ùٷ����ù���������
* @author     pig's grief
* @version    v1.0
* @date       2019-4-10
* @to do      ����UART��eDMAģʽ
*/
# include "include.h"
# include "Pig_UART.h"

#define LPUART_Default_CLOCK_SOURCE 80000000UL;
//UART��Ĭ������
const lpuart_config_t LPUART_Default_config = {
    .baudRate_Bps = 256000,                     //�����ʣ���������
    .parityMode = kLPUART_ParityDisabled,       //�Ƿ�����żУ�飬һ�㲻��żУ��
    .dataBitsCount = kLPUART_EightDataBits,     //���ݴ���λ����һ��Ϊ8λ
    .isMsb = false,                             //�Ƿ��ȴ�MSB��һ��Ϊ��
    .stopBitCount = kLPUART_OneStopBit,         //�趨ֹͣλ�м�λ��һ��Ϊ1λ
    .txFifoWatermark = 0,                       //txˮӡֵ
    .rxFifoWatermark = 1,                       //rxˮӡֵ
    .enableRxRTS = false,                       //�Ƿ�ʹ��RTS��һ��Ϊ��
    .enableTxCTS = false,                       //�Ƿ�ʹ��CTS��һ��Ϊ��
    .txCtsSource = kLPUART_CtsSourcePin,        //һ�㱣��Ĭ��
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //һ�㱣��Ĭ��
    .rxIdleType = kLPUART_IdleTypeStartBit,     //һ�㱣��Ĭ��
    .rxIdleConfig = kLPUART_IdleCharacter1,     //һ�㱣��Ĭ��
    .enableTx = true,                           //�Ƿ�ʹ��Tx,�������趨
    .enableRx = true                            //�Ƿ�ʹ��Rx,�������趨
};

uint32_t LPUART1_Clock = LPUART_Default_CLOCK_SOURCE;//UART1��ʱ��Ƶ��
lpuart_config_t LPUART1_config = {
    .baudRate_Bps = 256000,                     //�����ʣ���������
    .parityMode = kLPUART_ParityDisabled,       //�Ƿ�����żУ�飬һ�㲻��żУ��
    .dataBitsCount = kLPUART_EightDataBits,     //���ݴ���λ����һ��Ϊ8λ
    .isMsb = false,                             //�Ƿ��ȴ�MSB��һ��Ϊ��
    .stopBitCount = kLPUART_OneStopBit,         //�趨ֹͣλ�м�λ��һ��Ϊ1λ
    .txFifoWatermark = 0,                       //txˮӡֵ
    .rxFifoWatermark = 1,                       //rxˮӡֵ
    .enableRxRTS = false,                       //�Ƿ�ʹ��RTS��һ��Ϊ��
    .enableTxCTS = false,                       //�Ƿ�ʹ��CTS��һ��Ϊ��
    .txCtsSource = kLPUART_CtsSourcePin,        //һ�㱣��Ĭ��
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //һ�㱣��Ĭ��
    .rxIdleType = kLPUART_IdleTypeStartBit,     //һ�㱣��Ĭ��
    .rxIdleConfig = kLPUART_IdleCharacter1,     //һ�㱣��Ĭ��
    .enableTx = true,                           //�Ƿ�ʹ��Tx,�������趨
    .enableRx = true                            //�Ƿ�ʹ��Rx,�������趨
};//UART1������

uint32_t LPUART2_Clock = LPUART_Default_CLOCK_SOURCE;//UART2��ʱ��Ƶ��
lpuart_config_t LPUART2_config = {
    .baudRate_Bps = 256000,                     //�����ʣ���������
    .parityMode = kLPUART_ParityDisabled,       //�Ƿ�����żУ�飬һ�㲻��żУ��
    .dataBitsCount = kLPUART_EightDataBits,     //���ݴ���λ����һ��Ϊ8λ
    .isMsb = false,                             //�Ƿ��ȴ�MSB��һ��Ϊ��
    .stopBitCount = kLPUART_OneStopBit,         //�趨ֹͣλ�м�λ��һ��Ϊ1λ
    .txFifoWatermark = 0,                       //txˮӡֵ
    .rxFifoWatermark = 1,                       //rxˮӡֵ
    .enableRxRTS = false,                       //�Ƿ�ʹ��RTS��һ��Ϊ��
    .enableTxCTS = false,                       //�Ƿ�ʹ��CTS��һ��Ϊ��
    .txCtsSource = kLPUART_CtsSourcePin,        //һ�㱣��Ĭ��
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //һ�㱣��Ĭ��
    .rxIdleType = kLPUART_IdleTypeStartBit,     //һ�㱣��Ĭ��
    .rxIdleConfig = kLPUART_IdleCharacter1,     //һ�㱣��Ĭ��
    .enableTx = true,                           //�Ƿ�ʹ��Tx,�������趨
    .enableRx = true                            //�Ƿ�ʹ��Rx,�������趨
};//UART2������

uint32_t LPUART3_Clock = LPUART_Default_CLOCK_SOURCE;//UART3��ʱ��Ƶ��
lpuart_config_t LPUART3_config = {
    .baudRate_Bps = 256000,                     //�����ʣ���������
    .parityMode = kLPUART_ParityDisabled,       //�Ƿ�����żУ�飬һ�㲻��żУ��
    .dataBitsCount = kLPUART_EightDataBits,     //���ݴ���λ����һ��Ϊ8λ
    .isMsb = false,                             //�Ƿ��ȴ�MSB��һ��Ϊ��
    .stopBitCount = kLPUART_OneStopBit,         //�趨ֹͣλ�м�λ��һ��Ϊ1λ
    .txFifoWatermark = 0,                       //txˮӡֵ
    .rxFifoWatermark = 1,                       //rxˮӡֵ
    .enableRxRTS = false,                       //�Ƿ�ʹ��RTS��һ��Ϊ��
    .enableTxCTS = false,                       //�Ƿ�ʹ��CTS��һ��Ϊ��
    .txCtsSource = kLPUART_CtsSourcePin,        //һ�㱣��Ĭ��
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //һ�㱣��Ĭ��
    .rxIdleType = kLPUART_IdleTypeStartBit,     //һ�㱣��Ĭ��
    .rxIdleConfig = kLPUART_IdleCharacter1,     //һ�㱣��Ĭ��
    .enableTx = true,                           //�Ƿ�ʹ��Tx,�������趨
    .enableRx = true                            //�Ƿ�ʹ��Rx,�������趨
};//UART3������

uint32_t LPUART4_Clock = LPUART_Default_CLOCK_SOURCE;//UART4��ʱ��Ƶ��
lpuart_config_t LPUART4_config = {
    .baudRate_Bps = 256000,                     //�����ʣ���������
    .parityMode = kLPUART_ParityDisabled,       //�Ƿ�����żУ�飬һ�㲻��żУ��
    .dataBitsCount = kLPUART_EightDataBits,     //���ݴ���λ����һ��Ϊ8λ
    .isMsb = false,                             //�Ƿ��ȴ�MSB��һ��Ϊ��
    .stopBitCount = kLPUART_OneStopBit,         //�趨ֹͣλ�м�λ��һ��Ϊ1λ
    .txFifoWatermark = 0,                       //txˮӡֵ
    .rxFifoWatermark = 1,                       //rxˮӡֵ
    .enableRxRTS = false,                       //�Ƿ�ʹ��RTS��һ��Ϊ��
    .enableTxCTS = false,                       //�Ƿ�ʹ��CTS��һ��Ϊ��
    .txCtsSource = kLPUART_CtsSourcePin,        //һ�㱣��Ĭ��
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //һ�㱣��Ĭ��
    .rxIdleType = kLPUART_IdleTypeStartBit,     //һ�㱣��Ĭ��
    .rxIdleConfig = kLPUART_IdleCharacter1,     //һ�㱣��Ĭ��
    .enableTx = true,                           //�Ƿ�ʹ��Tx,�������趨
    .enableRx = true                            //�Ƿ�ʹ��Rx,�������趨
};//UART4������

uint32_t LPUART5_Clock = LPUART_Default_CLOCK_SOURCE;//UART5��ʱ��Ƶ��
lpuart_config_t LPUART5_config = {
    .baudRate_Bps = 256000,                     //�����ʣ���������
    .parityMode = kLPUART_ParityDisabled,       //�Ƿ�����żУ�飬һ�㲻��żУ��
    .dataBitsCount = kLPUART_EightDataBits,     //���ݴ���λ����һ��Ϊ8λ
    .isMsb = false,                             //�Ƿ��ȴ�MSB��һ��Ϊ��
    .stopBitCount = kLPUART_OneStopBit,         //�趨ֹͣλ�м�λ��һ��Ϊ1λ
    .txFifoWatermark = 0,                       //txˮӡֵ
    .rxFifoWatermark = 1,                       //rxˮӡֵ
    .enableRxRTS = false,                       //�Ƿ�ʹ��RTS��һ��Ϊ��
    .enableTxCTS = false,                       //�Ƿ�ʹ��CTS��һ��Ϊ��
    .txCtsSource = kLPUART_CtsSourcePin,        //һ�㱣��Ĭ��
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //һ�㱣��Ĭ��
    .rxIdleType = kLPUART_IdleTypeStartBit,     //һ�㱣��Ĭ��
    .rxIdleConfig = kLPUART_IdleCharacter1,     //һ�㱣��Ĭ��
    .enableTx = true,                           //�Ƿ�ʹ��Tx,�������趨
    .enableRx = true                            //�Ƿ�ʹ��Rx,�������趨
};//UART5������

uint32_t LPUART6_Clock = LPUART_Default_CLOCK_SOURCE;//UART6��ʱ��Ƶ��
lpuart_config_t LPUART6_config = {
    .baudRate_Bps = 256000,                     //�����ʣ���������
    .parityMode = kLPUART_ParityDisabled,       //�Ƿ�����żУ�飬һ�㲻��żУ��
    .dataBitsCount = kLPUART_EightDataBits,     //���ݴ���λ����һ��Ϊ8λ
    .isMsb = false,                             //�Ƿ��ȴ�MSB��һ��Ϊ��
    .stopBitCount = kLPUART_OneStopBit,         //�趨ֹͣλ�м�λ��һ��Ϊ1λ
    .txFifoWatermark = 0,                       //txˮӡֵ
    .rxFifoWatermark = 1,                       //rxˮӡֵ
    .enableRxRTS = false,                       //�Ƿ�ʹ��RTS��һ��Ϊ��
    .enableTxCTS = false,                       //�Ƿ�ʹ��CTS��һ��Ϊ��
    .txCtsSource = kLPUART_CtsSourcePin,        //һ�㱣��Ĭ��
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //һ�㱣��Ĭ��
    .rxIdleType = kLPUART_IdleTypeStartBit,     //һ�㱣��Ĭ��
    .rxIdleConfig = kLPUART_IdleCharacter1,     //һ�㱣��Ĭ��
    .enableTx = true,                           //�Ƿ�ʹ��Tx,�������趨
    .enableRx = true                            //�Ƿ�ʹ��Rx,�������趨
};//UART6������

uint32_t LPUART7_Clock = LPUART_Default_CLOCK_SOURCE;//UART7��ʱ��Ƶ��
lpuart_config_t LPUART7_config = {
    .baudRate_Bps = 256000,                     //�����ʣ���������
    .parityMode = kLPUART_ParityDisabled,       //�Ƿ�����żУ�飬һ�㲻��żУ��
    .dataBitsCount = kLPUART_EightDataBits,     //���ݴ���λ����һ��Ϊ8λ
    .isMsb = false,                             //�Ƿ��ȴ�MSB��һ��Ϊ��
    .stopBitCount = kLPUART_OneStopBit,         //�趨ֹͣλ�м�λ��һ��Ϊ1λ
    .txFifoWatermark = 0,                       //txˮӡֵ
    .rxFifoWatermark = 1,                       //rxˮӡֵ
    .enableRxRTS = false,                       //�Ƿ�ʹ��RTS��һ��Ϊ��
    .enableTxCTS = false,                       //�Ƿ�ʹ��CTS��һ��Ϊ��
    .txCtsSource = kLPUART_CtsSourcePin,        //һ�㱣��Ĭ��
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //һ�㱣��Ĭ��
    .rxIdleType = kLPUART_IdleTypeStartBit,     //һ�㱣��Ĭ��
    .rxIdleConfig = kLPUART_IdleCharacter1,     //һ�㱣��Ĭ��
    .enableTx = true,                           //�Ƿ�ʹ��Tx,�������趨
    .enableRx = true                            //�Ƿ�ʹ��Rx,�������趨
};//UART7������

uint32_t LPUART8_Clock = LPUART_Default_CLOCK_SOURCE;//UART8��ʱ��Ƶ��
lpuart_config_t LPUART8_config = {
    .baudRate_Bps = 256000,                     //�����ʣ���������
    .parityMode = kLPUART_ParityDisabled,       //�Ƿ�����żУ�飬һ�㲻��żУ��
    .dataBitsCount = kLPUART_EightDataBits,     //���ݴ���λ����һ��Ϊ8λ
    .isMsb = false,                             //�Ƿ��ȴ�MSB��һ��Ϊ��
    .stopBitCount = kLPUART_OneStopBit,         //�趨ֹͣλ�м�λ��һ��Ϊ1λ
    .txFifoWatermark = 0,                       //txˮӡֵ
    .rxFifoWatermark = 1,                       //rxˮӡֵ
    .enableRxRTS = false,                       //�Ƿ�ʹ��RTS��һ��Ϊ��
    .enableTxCTS = false,                       //�Ƿ�ʹ��CTS��һ��Ϊ��
    .txCtsSource = kLPUART_CtsSourcePin,        //һ�㱣��Ĭ��
    .txCtsConfig = kLPUART_CtsSampleAtStart,    //һ�㱣��Ĭ��
    .rxIdleType = kLPUART_IdleTypeStartBit,     //һ�㱣��Ĭ��
    .rxIdleConfig = kLPUART_IdleCharacter1,     //һ�㱣��Ĭ��
    .enableTx = true,                           //�Ƿ�ʹ��Tx,�������趨
    .enableRx = true                            //�Ƿ�ʹ��Rx,�������趨
};//UART8������


void UART_Init(LPUART_Type *UARTn, uint32_t bound)
{
    lpuart_config_t UartConfig = LPUART_Default_config;
    uint32_t UartClock = LPUART_Default_CLOCK_SOURCE;

    if (UARTn == LPUART1)
    {
        UartConfig = LPUART1_config;
        UartClock = LPUART1_Clock;
    }
    else if (UARTn == LPUART2)
    {
        UartConfig = LPUART2_config;
        UartClock = LPUART2_Clock;
    }
    else if (UARTn == LPUART3)
    {
        UartConfig = LPUART3_config;
        UartClock = LPUART3_Clock;
    }
    else if (UARTn == LPUART4)
    {
        UartConfig = LPUART4_config;
        UartClock = LPUART4_Clock;
    }
    else if (UARTn == LPUART5)
    {
        UartConfig = LPUART5_config;
        UartClock = LPUART5_Clock;
    }
    else if (UARTn == LPUART6)
    {
        UartConfig = LPUART6_config;
        UartClock = LPUART6_Clock;
    }
    else if (UARTn == LPUART7)
    {
        UartConfig = LPUART7_config;
        UartClock = LPUART7_Clock;
    }
    else if (UARTn == LPUART8)
    {
        UartConfig = LPUART8_config;
        UartClock = LPUART8_Clock;
    }
    UartConfig.baudRate_Bps = bound;

    LPUART_Init(UARTn, &UartConfig, UartClock);
}

uint8 UART_GetChar(LPUART_Type *UARTn)
{
    return LPUART_ReadByte(UARTn);
}

void UART_Send(LPUART_Type *UARTn, uint8 * databuff, uint8 datasize)
{
    LPUART_WriteBlocking(UARTn, databuff, datasize);
}
