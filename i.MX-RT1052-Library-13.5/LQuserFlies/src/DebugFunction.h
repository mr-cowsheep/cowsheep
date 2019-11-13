/*!
* @file       DebugFuction.h
* @brief      ���ڵ����õĺ�����ͷ�ļ�
* @details
* @author     pig's grief
* @version    v1.0
* @date       2019-2-26
* @to do
*
*/
# ifndef _DEBUGFUNCTION_H_
# define _DEBUGFUNCTION_H_
# define Remote_UseDigitalReceive
# define CRC_Uart_Port LPUART1
# define Series_Uart_Port LPUART2
# define GY53_Uart_Port LPUART6
# define Remote_Uart_Port LPUART3
typedef enum
{
    SendFinish,
    Sending
    
}DebugSeriesDMAStatus;

extern DebugSeriesDMAStatus DMASendStatus;
/// <summary>
///������ʾ��������a,b,c,dȡ��֮���ֵ
///<para>example:  SEND(a,b,c,d);</para>
///</summary>
/// <param name="a">�����͵ı���1</param>
/// <param name="b">�����͵ı���2</param>
/// <param name="c">�����͵ı���3</param>
/// <param name="d">�����͵ı���4</param>
void SEND(float a,float b,float c,float d);
void CRC_RX(void);
/// <summary>
///��OLED�ϻ�������ͼ(�൱����OLED��ʹ������ʾ����)
///����û����һ��ʱ�̵���1ֱ������t_refresh֮������´�0ʱ�̿�ʼ
///<para>example:  LCD_ShowGraphs(data, 126, 0, 100, 0);</para>
///</summary>
/// <param name="data">����ʱ�̶�Ӧ������ֵ(y���ֵ)</param>
/// <param name="t_refresh">ˢ��ʱ�̵����ֵ����󲻳���126</param>
/// <param name="data_low">Ҫ��ʾ�����ݵķ�Χ������</param>
/// <param name="data_high">Ҫ��ʾ�����ݵķ�Χ������</param>
/// <param name="WhetherClear">ˢ��ʱ�̵�֮���Ƿ�������1��ʾ������0��ʾ��</param>
void LCD_ShowGraphs(float data, float t_refresh, float data_low, float data_high, int WhetherClear);
/*===================================================================
���ܣ����ڷ�����������Matlab���ݴ���(����)
===================================================================*/
void DATA_SEND(long num);

# define uint8 unsigned char

typedef enum
{
    Left_Return0,
    Left_Left,
    Left_Right,
    Left_Up,
    Left_Down,
    Right_Return0,
    Right_Left,
    Right_Right,
    Right_Up,
    Right_Down,
    Start
}RemoteCMDMode;
typedef enum
{
    Sleep,
    SendLeftCMD,
    ReceivingLeftCMD,
    ReceivedLeftCMD,
    SendRightCMD,
    ReceivingRightCMD,
    ReceivedRightCMD
}Remote_State;
typedef struct
{
    uint8 Left_X;
    uint8 Left_Y;
    uint8 Right_X;
    uint8 Right_Y;
}ReceiveCMDData;
/// <summary>
///��ʼ��ң�����Ĵ���
///</summary>
void RemoteInit();
void RemoteData_init(void);
/// <summary>
///����ң��ָ�����Ӧ�����Ӧ�Ĵ����ж���
///</summary>
#ifdef Remote_UseDigitalReceive
void ReceiveCMD_Remote(void);
extern RemoteCMDMode RunMode;
#else
void SendRemoteCMDData(void);
void ReceiveCMD_Remote(void);
#endif

void Series_RX(void);
void Series_Receive_Data_Analysis(void);
void Series_Receive_init(void);
typedef enum
{
    Press,
    NotPress
}ButtonStatus;//������״̬��PressΪ����,NotPressΪ����
extern ButtonStatus Button[3];
extern int ButtonOnceFlag[3];
extern int ButtonOnceBuffFlag[3];
extern ButtonStatus Button1;//PTE12
extern ButtonStatus Button2;//PTE11
extern ButtonStatus Button3;//PTE10
extern uint8 Series_deviation_received_front;
extern uint8 Series_distance_received_front;
extern uint8 Series_deviation_received_back;
extern uint8 Series_distance_received_back;
///<summary>������ʼ��</summary>
void ButtonInit();
///<summary>����ɨ���ж�</summary>
void ButtonScan_interrupt();
/// <summary>
///�����˵��������ڲ����趨�ȹ��ܣ���������������Ҫ����ǰ
///<para>ע��һ��Ҫ����TFT��ʼ���������������жϵ�ģ���ʼ����������������������</para>
///</summary>
void ButtonMenu();
void ButtonMenu_SetPara();

# define BeepIOPortType PTB
# define BeepIOPortIndex 19
# define BeepTimeSet 100

typedef enum
{
    Off,
    Alarming,
    AlarmFinish
}BeepAlarmStatus;
extern BeepAlarmStatus AlarmStatus;
void BeepTimerInter();
void BeepAlarm();

# define BatteryCollectADCChn ADC1_CH3
# define BatteryCollectADC ADC1

void BatteryVoltageCollect_Init(int IfUseBeep);
float GetBatteryVoltage(float HintVoltage);
void LCD_ShowBatteryVoltage(unsigned char x, unsigned char y, float num);
void FIFO(uint8 *head, uint8 depth, uint8 num);
void FIFO_Clean(uint8 *head, uint8 depth);
uint8 One_loop_bubblesort(uint8 *lis, uint8 depth, int flag);
uint8 One_loop_bubblesort_new(uint8 *lis, uint8 depth, int flag);
void GY53_RX(void);
void GY53_Receive_init(void);

typedef enum
{
    Race_Start,
    Lamp_Found,
    Find_Next
}Lamp_Status;
extern Lamp_Status Status_Present;
typedef enum
{
    Stationary,
    Speed_up,
    Speed_down,
    Rotate,
    Turn_Around
}Motor_Pattern;
extern Motor_Pattern Function_Present;

extern int graph_offset_front;
extern int trail_offset_front_left;
extern int trail_offset_front_right;
extern int speed_offset_front;
extern int graph_offset_back;
extern int trail_offset_back_left;
extern int trail_offset_back_right;
extern int speed_offset_back;
#endif