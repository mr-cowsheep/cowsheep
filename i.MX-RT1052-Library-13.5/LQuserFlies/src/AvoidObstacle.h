# ifndef _AVOIDOBSTACLE_H_
# define _AVOIDOBSTACLE_H_

# define Ultrasound_Uart_Port_1 LPUART6
# define Ultrasound_Uart_Port_2 LPUART1

typedef enum
{
    Meassuring,
    Meassured,
    Error
}Enum_UltrasoundMeassure;

typedef struct
{
    unsigned char UltrasoundIndex;
    unsigned char DisReceiveIndex;
    float Distance_Meassured;
    unsigned char ReceiveBuff_Dis[2];
    unsigned int MeasureTime_Dis;
    Enum_UltrasoundMeassure UltrasoundMeassure;
}DistanceMeasureStruct;

extern DistanceMeasureStruct DistanceMeasure_1;
extern DistanceMeasureStruct DistanceMeasure_2;

# define MaxMeassureRangeSetCMD_50cm 0x05 // 3885us
# define MaxMeassureRangeSetCMD_100cm 0x0A // 6770us
# define MaxMeassureRangeSetCMD_150cm 0x0F // 9655us
# define MaxMeassureRangeSetCMD_200cm 0x14 // 12540us
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ṹ���ʼ��
//  @param      DistanceMeasure  ���ṹ��
//  @param      Index  ����������
//  Example:    DistanceMeasureStructInit(&DistanceMeasure_1, 1);
//-------------------------------------------------------------------------------------------------------------------
void DistanceMeasureStructInit(DistanceMeasureStruct * DistanceMeasure, unsigned char Index);
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������������ָ��
//  @param      DistanceMeasure  ���ṹ��
//  Example:    SendCMDToUltrasound(&DistanceMeasure_1);
//-------------------------------------------------------------------------------------------------------------------
void SendCMDToUltrasound(DistanceMeasureStruct * DistanceMeasure);
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������ģ���ʼ��
//  @param      DistanceMeasure  ���ṹ��
//  Example:    UltrasoundInit(&DistanceMeasure_1);
//-------------------------------------------------------------------------------------------------------------------
void UltrasoundInit(DistanceMeasureStruct * DistanceMeasure);
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ڽ��ܴ������Ӧ�ŵ���Ӧ�Ĵ����ж�
//  Example:    ReceiveDistance_2();
//-------------------------------------------------------------------------------------------------------------------
void ReceiveDistance_2();
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ڽ��ܴ������Ӧ�ŵ���Ӧ�Ĵ����ж�
//  Example:    ReceiveDistance_1();
//-------------------------------------------------------------------------------------------------------------------
void ReceiveDistance_1();

# endif