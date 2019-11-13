# include "include.h"

DistanceMeasureStruct DistanceMeasure_1;
DistanceMeasureStruct DistanceMeasure_2;

void DistanceMeasureStructInit(DistanceMeasureStruct * DistanceMeasure, unsigned char Index)
{
    DistanceMeasure->UltrasoundIndex = Index;
    DistanceMeasure->DisReceiveIndex = 0;
    DistanceMeasure->Distance_Meassured = 0;
    DistanceMeasure->MeasureTime_Dis = 0;
    DistanceMeasure->UltrasoundMeassure = Meassuring;
}
/// <summary>
///初始化超声波模块
///</summary>
void UltrasoundInit(DistanceMeasureStruct * DistanceMeasure)
{
    if (DistanceMeasure->UltrasoundIndex == 1)
    {
        UART_Init(Ultrasound_Uart_Port_1, 9600);
        LQ_UART_PutChar(Ultrasound_Uart_Port_1, 0xe8);//模块地址
        LQ_UART_PutChar(Ultrasound_Uart_Port_1, 0x02);//寄存器2
        LQ_UART_PutChar(Ultrasound_Uart_Port_1, 0x75);//6级电源降噪
    }
    else if (DistanceMeasure->UltrasoundIndex == 2)
    {
        UART_Init(Ultrasound_Uart_Port_2, 9600);
        LQ_UART_PutChar(Ultrasound_Uart_Port_2, 0xe8);//模块地址
        LQ_UART_PutChar(Ultrasound_Uart_Port_2, 0x02);//寄存器2
        LQ_UART_PutChar(Ultrasound_Uart_Port_2, 0x75);//6级电源降噪
    }

}
/// <summary>
///给超声波模块发送查询指令
///</summary>
void SendCMDToUltrasound(DistanceMeasureStruct * DistanceMeasure)
{
    if (DistanceMeasure->UltrasoundIndex == 1)
    {
        if (DistanceMeasure->UltrasoundMeassure == Meassured)
        {
            DistanceMeasure->UltrasoundMeassure = Meassuring;
            LQ_UART_PutChar(Ultrasound_Uart_Port_1, 0xe8);//模块地址
            LQ_UART_PutChar(Ultrasound_Uart_Port_1, 0x02);//寄存器2
            LQ_UART_PutChar(Ultrasound_Uart_Port_1, MaxMeassureRangeSetCMD_100cm);//写入的指令
        }
    }
    else if (DistanceMeasure->UltrasoundIndex == 2)
    {
        if (DistanceMeasure->UltrasoundMeassure == Meassured)
        {
            DistanceMeasure->UltrasoundMeassure = Meassuring;
            LQ_UART_PutChar(Ultrasound_Uart_Port_2, 0xe8);//模块地址
            LQ_UART_PutChar(Ultrasound_Uart_Port_2, 0x02);//寄存器2
            LQ_UART_PutChar(Ultrasound_Uart_Port_2, MaxMeassureRangeSetCMD_100cm);//写入的指令
        }
    }

}

/// <summary>
///接受距离数据反馈，要放到相应串口中断里面
///</summary>
void ReceiveDistance_2()
{
    uint8_t buff = 0;

    if (DistanceMeasure_2.UltrasoundMeassure == Meassuring)
    {
        DistanceMeasure_2.ReceiveBuff_Dis[DistanceMeasure_2.DisReceiveIndex] = UART_Get_Char(Ultrasound_Uart_Port_2);
        DistanceMeasure_2.DisReceiveIndex++;

        if (DistanceMeasure_2.DisReceiveIndex == 2)
        {
            DistanceMeasure_2.DisReceiveIndex = 0;
            DistanceMeasure_2.UltrasoundMeassure = Meassured;
            DistanceMeasure_2.MeasureTime_Dis = DistanceMeasure_2.ReceiveBuff_Dis[0] * 256 + DistanceMeasure_2.ReceiveBuff_Dis[0];
            DistanceMeasure_2.Distance_Meassured = DistanceMeasure_2.MeasureTime_Dis * 0.0173;
        }
    }
    else
    {
        buff = UART_Get_Char(Ultrasound_Uart_Port_2);
    }

}
void ReceiveDistance_1()
{
    uint8_t buff = 0;

    if (DistanceMeasure_1.UltrasoundMeassure == Meassuring)
    {
        DistanceMeasure_1.ReceiveBuff_Dis[DistanceMeasure_1.DisReceiveIndex] = UART_Get_Char(Ultrasound_Uart_Port_1);
        DistanceMeasure_1.DisReceiveIndex++;

        if (DistanceMeasure_1.DisReceiveIndex == 2)
        {
            DistanceMeasure_1.DisReceiveIndex = 0;
            DistanceMeasure_1.UltrasoundMeassure = Meassured;
            DistanceMeasure_1.MeasureTime_Dis = DistanceMeasure_1.ReceiveBuff_Dis[0] * 256 + DistanceMeasure_1.ReceiveBuff_Dis[0];
            DistanceMeasure_1.Distance_Meassured = DistanceMeasure_1.MeasureTime_Dis * 0.0173;
        }
    }
    else
    {
        buff = UART_Get_Char(Ultrasound_Uart_Port_1);
    }

}

