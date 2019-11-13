/*!
* @file       DebugFuction.c
* @brief      ÓÃÓÚµ÷ÊÔÓÃµÄº¯Êý¿â
* @details
* @author     pig's grief
* @version    v1.0
* @date       2019-2-26
* @to do
*             
*/
# include "include.h"
DebugSeriesDMAStatus DMASendStatus;
extern lpuart_transfer_t sendXfer;
extern lpuart_edma_handle_t g_lpuartEdmaHandle;
void UART_Put_Buff_DMA(uint8_t *dataToSend, uint8_t length)
{
    /* If TX is idle and g_txBuffer is full, start to send data. */
    /*Ê¹ÓÃDMA + ´®¿Ú£¬ÎÞÐèÕ¼ÓÃCPUÊ±¼ä */
    sendXfer.data = dataToSend;
    sendXfer.dataSize = length;
    if (DMASendStatus == SendFinish)
    {
        DMASendStatus = Sending;
        LPUART_SendEDMA(LPUART1, &g_lpuartEdmaHandle, &sendXfer);
    }

}
/*************************************************************/
/*****************ÐéÄâÊ¾²¨Æ÷²¿·Ö************/
/*************************************************************/
int CRC_Index = 0;
char CRC_Buff[3] = {0};
unsigned char CRC_Receive = 0;
float OutData[4] = { 0 };//³ÌÐò»á½«Êý×éÖÐµÄËÄ¸öÊý·¢ËÍ¸øÐéÄâÊ¾²¨Æ÷
//float temp = 0.0;
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

void OutPut_Data(void)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
  {
    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];
    
}

for(i=0;i<4;i++) 
{
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
}

CRC16 = CRC_CHECK(databuf,8);
databuf[8] = CRC16%256;
databuf[9] = CRC16/256;

UART_Put_Buff_DMA(databuf, 10);
}
/// <summary>
///¸øÐéÄâÊ¾²¨Æ÷·¢ËÍa,b,c,dÈ¡ÕûÖ®ºóµÄÖµ
///<para>example:  SEND(a,b,c,d);</para>
///</summary>
/// <param name="a">´ý·¢ËÍµÄ±äÁ¿1</param>
/// <param name="b">´ý·¢ËÍµÄ±äÁ¿2</param>
/// <param name="c">´ý·¢ËÍµÄ±äÁ¿3</param>
/// <param name="d">´ý·¢ËÍµÄ±äÁ¿4</param>
void SEND(float a,float b,float c,float d)
{       
   OutData[0] = a;                   
   OutData[1] = b;                    
   OutData[2] = c;                
   OutData[3] = d;
   OutPut_Data();
}


void CRC_RX(void)
{
    uint8 buff = 0;
    buff = UART_Get_Char(CRC_Uart_Port);
    if (buff == 0xFF && CRC_Receive == 0)
    {
        CRC_Receive = 1;
        return;
    }
    else
    {
      if(CRC_Receive == 0)
         CRC_Index = 0;
 }
 if (CRC_Receive == 1)
 {
    if (CRC_Index < 1)
    {
      CRC_Buff[CRC_Index] = buff;
      CRC_Index++;
  }
  else
  {
    if (buff == 0xFF)
    {          
      CRC_Receive = 0;
      CRC_Index = 0;
  }
}
}
return;
}
/*************************************************************/
/*****************Êý¾Ý²É¼¯²¿·Ö************/
/*************************************************************/
/*===================================================================
¹¦ÄÜ£º´®¿Ú·¢ËÍÊý¾ÝÓÃÓÚMatlabÊý¾Ý´¦Àí
===================================================================*/
void DATA_SEND(long num)
{
    int weishu = 0;
    long num_buff = num;
    long buff = 1;
    int index = 0;

    if (num < 0)
    {
        UART_Put_Buff(CRC_Uart_Port, "-", 1);
        num = -num;
    }

    if (num_buff == 0)
    {
        weishu = 1;
        buff = 10;
    }

    while (num_buff != 0)
    {
        num_buff /= 10;
        weishu++;
        buff *= 10;
    }
    buff /= 10;

    for (index = 0; index < weishu; index++)
    {
        //UART_Put_Buff(CRC_Uart_Port, ((num / buff) % 10) + '0', 1);

        buff /= 10;
    }
    UART_Put_Buff(CRC_Uart_Port, " ", 1);
}

/*************************************************************/
/*****************Ò£¿Ø²¿·Ö************/
/*************************************************************/
/// <summary>
///³õÊ¼»¯Ò£¿ØÆ÷µÄ´®¿Ú
///</summary>
void RemoteInit()
{
    UART_Init(Remote_Uart_Port, 9600);
}
/// <summary>
///³õÊ¼»¯Ä£ÄâÒ£¿ØÆ÷µÄ½ÓÊÕÊý¾Ý
///</summary>
ReceiveCMDData RemoteData;
void RemoteData_init(void)
{
    RemoteData.Left_Y = 126;
    RemoteData.Left_X = 126;
    RemoteData.Right_Y = 126;
    RemoteData.Right_X = 126;
    
}
int Given_Speed = 0;
int ReceiveIndex = 0;
char ReceiveBuff[3] = {0};
unsigned char StartReceive = 0;
#ifdef Remote_UseDigitalReceive
RemoteCMDMode RunMode;//Ò£¿ØÄ£Ê½
/// <summary>
///½ÓÊÜÒ£¿ØÖ¸Áî³ÌÐò£¬Ó¦·ÅÈë¶ÔÓ¦µÄ´®¿ÚÖÐ¶ÏÄÚ
///</summary>
void ReceiveCMD_Remote(void)
{
    char buff = 0;
    buff = UART_Get_Char(Remote_Uart_Port);
    if (buff == 0xFF && StartReceive == 0)
    {
        StartReceive = 1;
        return;
    }
    else
    {
      if(StartReceive == 0)
         ReceiveIndex = 0;
 }
 if (StartReceive == 1)
 {
    if (ReceiveIndex < 3)
    {
        ReceiveBuff[ReceiveIndex] = buff;
        ReceiveIndex++;
    }
    else
    {
        if (buff == 0xFF)
        {
            StartReceive = 0;
            ReceiveIndex = 0;
                    if (ReceiveBuff[0] == 0x00)//×óÒ¡¸Ë
                    {
                        switch (ReceiveBuff[1])
                        {
                            case 0x00:
                            RunMode = Left_Return0;
                            break;
                            case 0x01:
                            RunMode = Left_Up;
                            break;
                            case 0x02:
                            RunMode = Left_Down;
                            break;
                            case 0x03:
                            RunMode = Left_Left;
                            break;
                            case 0x04:
                            RunMode = Left_Right;
                            break;
                            default:
                            break;
                        }
                    }
                    else if (ReceiveBuff[0] == 0x35)//×óÒ¡¸Ë
                    {
                        switch (ReceiveBuff[1])
                        {
                            case 0x00:
                            RunMode = Right_Return0;                            
                            Given_Speed = 50;
                            break;
                            case 0x01:
                            RunMode = Right_Up;                  
                            Given_Speed = 100;
                            break;
                            case 0x02:
                            RunMode = Right_Down;
                            Given_Speed = 150;
                            break;
                            case 0x03:
                            RunMode = Right_Left;
                            Given_Speed = 200;
                            break;
                            case 0x04:
                            RunMode = Right_Right;
                            Given_Speed = 250;
                            break;
                            default:
                            break;
                        }
                    }
                    else if (ReceiveBuff[0] == 0xAA)
                    {
                        RunMode = Start;
                    }
                }
            }
        }

    }
#else
/// <summary>
///ÇëÇóÒ£¿ØÖ¸Áî³ÌÐò£¬Ä£ÄâÓÃ
///</summary>
    Remote_State Remote_CMD_ReceiveStatus = Sleep;
    long count_error_left = 0;
    long count_error_right = 0;

    void SendRemoteCMDData(void)
    {
        if (Remote_CMD_ReceiveStatus == Sleep)
        {
            Remote_CMD_ReceiveStatus = SendLeftCMD;
            Remote_CMD_ReceiveStatus = ReceivingLeftCMD;
            UART_Put_Char(Remote_Uart_Port, 0xBB);
        }
        if(Remote_CMD_ReceiveStatus == ReceivingLeftCMD)
        {
          count_error_left++;
          count_error_right = 0;
      }
      else if(Remote_CMD_ReceiveStatus == ReceivingRightCMD)
      {
          count_error_right++; 
          count_error_left = 0;
      }
      else
      {
          count_error_left = 0;
          count_error_right = 0;
      }
      
      if(count_error_left >= 500 || count_error_right >= 500)
      {
          RemoteData.Left_Y = 127;
          RemoteData.Left_X = 127;
          RemoteData.Right_Y = 127;
          RemoteData.Right_X = 127;
          Remote_CMD_ReceiveStatus = Sleep;
          count_error_left = 0;
          count_error_right = 0;
      }
  }
/// <summary>
///½ÓÊÜÒ£¿ØÖ¸Áî³ÌÐò£¬Ó¦·ÅÈë¶ÔÓ¦µÄ´®¿ÚÖÐ¶ÏÄÚ
///</summary>
  void ReceiveCMD_Remote(void)
  {
    char buff = 0;
    buff = UART_Get_Char(Remote_Uart_Port);
    if (buff == 0xFF && StartReceive == 0)
    {
        StartReceive = 1;
        return;
    }
    else
    {
        if (StartReceive == 0)
            ReceiveIndex = 0;
    }
    if (StartReceive == 1)
    {
        if (ReceiveIndex < 3)
        {
            ReceiveBuff[ReceiveIndex] = buff;
            ReceiveIndex++;
        }
        else
        {
            if (buff == 0xFF)
            {
                StartReceive = 0;
                ReceiveIndex = 0;
                if (ReceiveBuff[0] == 0xBB)//×óÒ¡¸Ë
                {
                    if (Remote_CMD_ReceiveStatus == ReceivingLeftCMD)
                    {
                        RemoteData.Left_Y = ReceiveBuff[1];
                        RemoteData.Left_X = ReceiveBuff[2];
                        Remote_CMD_ReceiveStatus = ReceivedLeftCMD;
                        Remote_CMD_ReceiveStatus = ReceivingRightCMD;
                        UART_Put_Char(Remote_Uart_Port, 0xCC);          
                    }
                    
                }
                else if (ReceiveBuff[0] == 0xCC)//ÓÒÒ¡¸Ë
                {
                    if (Remote_CMD_ReceiveStatus == ReceivingRightCMD)
                    {
                        RemoteData.Right_Y = ReceiveBuff[1];
                        RemoteData.Right_X = ReceiveBuff[2];
                        Remote_CMD_ReceiveStatus = ReceivedRightCMD;
                        Remote_CMD_ReceiveStatus = Sleep;
                    }
                }
                else
                {
                    RemoteData.Left_Y = 127;
                    RemoteData.Left_X = 127;
                    RemoteData.Right_Y = 127;
                    RemoteData.Right_X = 127;
                    Remote_CMD_ReceiveStatus = Sleep;
                }
            }
        }
    }

}
#endif

///<summary>°´¼ü³õÊ¼»¯</summary>
void ButtonInit()
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */

    /* GPIO configuration of k3 on GPIO_AD_B1_00 (pin J11) */
    gpio_pin_config_t k3_config = {
        .direction = kGPIO_DigitalInput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_IntRisingOrFallingEdge
    };
    /* Initialize GPIO functionality on GPIO_AD_B1_00 (pin J11) */
    GPIO_PinInit(GPIO1, 16U, &k3_config);
    /* Enable GPIO pin interrupt on GPIO_AD_B1_00 (pin J11) */
    GPIO_PortEnableInterrupts(GPIO1, 1U << 16U);

    /* GPIO configuration of k1 on GPIO_B1_08 (pin A12) */
    gpio_pin_config_t k1_config = {
        .direction = kGPIO_DigitalInput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_IntRisingOrFallingEdge
    };
    /* Initialize GPIO functionality on GPIO_B1_08 (pin A12) */
    GPIO_PinInit(GPIO2, 24U, &k1_config);
    /* Enable GPIO pin interrupt on GPIO_B1_08 (pin A12) */
    GPIO_PortEnableInterrupts(GPIO2, 1U << 24U);

    /* GPIO configuration of k2 on GPIO_B1_09 (pin A13) */
    gpio_pin_config_t k2_config = {
        .direction = kGPIO_DigitalInput,
        .outputLogic = 0U,
        .interruptMode = kGPIO_IntRisingOrFallingEdge
    };
    /* Initialize GPIO functionality on GPIO_B1_09 (pin A13) */
    GPIO_PinInit(GPIO2, 25U, &k2_config);
    /* Enable GPIO pin interrupt on GPIO_B1_09 (pin A13) */
    GPIO_PortEnableInterrupts(GPIO2, 1U << 25U);

    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_00_GPIO1_IO16,        /* GPIO_AD_B1_00 is configured as GPIO1_IO16 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
        IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_08_GPIO2_IO24,           /* GPIO_B1_08 is configured as GPIO2_IO24 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
            IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_09_GPIO2_IO25,           /* GPIO_B1_09 is configured as GPIO2_IO25 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
                IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B1_00_GPIO1_IO16,        /* GPIO_AD_B1_00 PAD functional properties : */
        0x0110B0U);                             /* Slew Rate Field: Slow Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: medium(100MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Enabled */
                    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_08_GPIO2_IO24,           /* GPIO_B1_08 PAD functional properties : */
        0x0110B0U);                             /* Slew Rate Field: Slow Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: medium(100MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Enabled */
                        IOMUXC_SetPinConfig(
        IOMUXC_GPIO_B1_09_GPIO2_IO25,           /* GPIO_B1_09 PAD functional properties : */
        0x0110B0U);                             /* Slew Rate Field: Slow Slew Rate
                                                Drive Strength Field: R0/6*/

                            EnableIRQ(GPIO2_Combined_16_31_IRQn);

                            EnableIRQ(GPIO1_Combined_16_31_IRQn);

                            TFT_showstr(0, 0, "Please Press!", BLACK, WHITE);
                        }
ButtonStatus Button[3] = { NotPress, NotPress, NotPress };//PTE12,PTE11,PTE10


///<summary>°å¼äÍ¨ÐÅ</summary>
int direction_flag=1;
uint8 SeriesReceive = 0;
uint8 SeriesIndex = 0;
uint8 Series_ReceiveBuff[4] = { 0 };
uint8 Front_Distance_ReceiveBuff[5] = { 0 };
uint8 Back_Distance_ReceiveBuff[5] = { 0 };
uint8 Front_Deviation_ReceiveBuff[11] = { 0 };
uint8 Back_Deviation_ReceiveBuff[11] = { 0 };
uint8 Series_deviation_received_front=0;
uint8 Series_distance_received_front=0;
uint8 Series_deviation_received_back=0;
uint8 Series_distance_received_back=0;
uint8 Series_deviation_received=0;
uint8 Series_distance_received=0;
//uint8 Series_distance_received_last=0;
//uint8 Series_deviation_received_last=0;
uint64_t StartTime_us_RX = 0;
uint64_t EndTime_us_RX = 0;
uint64_t Time_Speed = 0;
uint8 distance_deviation_relevance_front_left[35]={98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,102,107,111,115,118,122,125,128,131,134,134,134,134,134,134,134,134,134};
uint8 distance_deviation_relevance_back_left[35]={96,96,96,96,96,96,96,96,96,96,96,96,99,103,107,111,114,118,121,124,127,130,133,133,133,133,133,133,133,133,133,133,133,133,133};
uint8 distance_deviation_relevance_front_right[35]={89,89,89,89,89,89,89,89,89,89,89,89,89,89,89,89,89,86,82,76,71,68,64,61,58,55,55,55,55,55,55,55,55,55,55};
uint8 distance_deviation_relevance_back_right[35]={89,89,89,89,89,89,89,89,89,89,89,89,86,83,79,74,69,67,63,59,56,53,53,53,53,53,53,53,53,53,53,53,53,53,53};
Lamp_Status Status_Present = Race_Start;
Motor_Pattern Function_Present = Stationary;
extern struct PIDControl Car_Speed_Rotate;//追灯转向闭环
extern uint8 Gear_Flag;
extern long YSpeed_Solved;
extern float V_Ytarget;
int trail_flag = 0;

int graph_offset_front = 8;
int trail_offset_front_left = 0;
int trail_offset_front_right = 0;
int speed_offset_front = 9;
int graph_offset_back = 8;
int trail_offset_back_left = 0;
int trail_offset_back_right = 0;
int speed_offset_back = 4;

void Series_RX(void)
{
    uint8 buff = 0;
    buff = UART_Get_Char(Series_Uart_Port);
    if (buff == 0xFF && SeriesReceive == 0)
    {
        SeriesReceive = 1;
        return;
    }
    else
    {
      if(SeriesReceive == 0)
         SeriesIndex = 0;
 }
 if (SeriesReceive == 1)
 {
    if (SeriesIndex < 4)
    {
      Series_ReceiveBuff[SeriesIndex] = buff;
      SeriesIndex++;
  }
  else
  {
    if (buff == 0xFF)
    {
      FIFO(Front_Distance_ReceiveBuff, 5, Series_ReceiveBuff[1]);
      FIFO(Back_Distance_ReceiveBuff, 5, Series_ReceiveBuff[3]);
              //EndTime_us_RX = _systime.get_time_us();
              //Time_Speed = EndTime_us_RX - StartTime_us_RX;
              //StartTime_us_RX = _systime.get_time_us();            
      SeriesReceive = 0;
      SeriesIndex = 0;
  }
}
}
}
///<summary>´®¼¶Í¨ÐÅ³õÊ¼»¯</summary>
void Series_Receive_init(void)
{
    UART_Init(Series_Uart_Port, 115200);
}
///<summary>´®¼¶Í¨ÐÅÊý¾Ý´¦Àí</summary>
void Series_Receive_Data_Analysis(void)
{
    Series_distance_received_front = One_loop_bubblesort(Front_Distance_ReceiveBuff, 5, 2);
    Series_distance_received_back = One_loop_bubblesort(Back_Distance_ReceiveBuff, 5, 2);
    Series_deviation_received_front = Series_ReceiveBuff[0];
    Series_deviation_received_back = Series_ReceiveBuff[2];
            //×ªÏòÔË¶¯²ßÂÔ//
    //Series_distance_received_last = Series_distance_received;
    //Series_deviation_received_last = Series_deviation_received;
    if(direction_flag == 1)
    {

      if(Series_deviation_received_back == 0)
      {
        Series_deviation_received = Series_deviation_received_front;
        Series_distance_received = Series_distance_received_front;
        direction_flag = 1;
    }
    else
    {
        Series_deviation_received = Series_deviation_received_back;
        Series_distance_received = Series_distance_received_back;
        direction_flag = -1;
    }
}
else if(direction_flag == -1)
{            
  if(Series_deviation_received_front == 0)
  {
    Series_deviation_received = Series_deviation_received_back;
    Series_distance_received = Series_distance_received_back;
    direction_flag = -1;
}
else
{
    Series_deviation_received = Series_deviation_received_front;
    Series_distance_received = Series_distance_received_front;
    direction_flag = 1;
} 
}
			if(Series_distance_received>34)
				Series_distance_received=34;
			if(direction_flag == 1)
                        {
                          if(Series_deviation_received <94)
                          {
                              PID_SetTarget(&Car_Speed_Rotate, distance_deviation_relevance_front_right[Series_distance_received + graph_offset_front] + trail_offset_front_right);
                            trail_flag = 1;
                          }
                          else if(Series_deviation_received >93)
                          {
                              PID_SetTarget(&Car_Speed_Rotate, distance_deviation_relevance_front_left[Series_distance_received + graph_offset_front] + trail_offset_front_left);
                            trail_flag = -1;
                          }
                        }
			else 
                        {
                          if(Series_deviation_received <94)
                          {
                              PID_SetTarget(&Car_Speed_Rotate, distance_deviation_relevance_back_right[Series_distance_received + graph_offset_back] + trail_offset_back_right);
                            trail_flag = 1;
                          }
                          else if(Series_deviation_received >93)
                          {
                              PID_SetTarget(&Car_Speed_Rotate, distance_deviation_relevance_back_left[Series_distance_received + graph_offset_back] + trail_offset_back_left);
                            trail_flag = -1;
                          }
                        }
if(Status_Present == Race_Start)
{
    if(Series_deviation_received != 0)
    {
        Status_Present = Lamp_Found;
        Function_Present = Speed_up;
        //Gear_Flag = 8;
    }
}
else if(Status_Present == Lamp_Found)
{
    if(Series_deviation_received == 0)
    {
        Status_Present = Find_Next;
        Function_Present = Rotate;
    }
}
else if(Status_Present == Find_Next)
{
    if(Series_deviation_received != 0)
    {
        Status_Present = Lamp_Found;
        Function_Present = Speed_up;
    }
}
if (Function_Present == Speed_up)
{
    if(direction_flag == 1)
    {
        if (Series_distance_received >speed_offset_front)
        {
//            V_Ytarget = 250-YSpeed_Solved;
//            if(V_Ytarget <50)
//              V_Ytarget =50;
//            else if(V_Ytarget >130)
//              V_Ytarget = 130;
            Function_Present = Speed_down;
        }
    }
    else
    {
        if (Series_distance_received >speed_offset_back)
        {
//             V_Ytarget = -250+YSpeed_Solved;
//            if(V_Ytarget >-50)
//              V_Ytarget =-50;
//            else if(V_Ytarget <-130)
//              V_Ytarget = -130;
            Function_Present = Speed_down;
        }
    }
}
else if(Function_Present == Speed_down)
{
    if(direction_flag ==1)
    {
        if (Series_distance_received <speed_offset_front+1)
        {
            Function_Present = Speed_up;
            //Gear_Flag = 8;
        }
    }
    else
    {
        if (Series_distance_received <speed_offset_back+1)
        {
            Function_Present = Speed_up;
            //Gear_Flag = 8;
        }
    }
}
}

///<summary>°´¼üÉ¨ÃèÖÐ¶Ï</summary>
void GPIO2_Combined_16_31_IRQHandler(void)
{
    unsigned char keybuff = 0;
    unsigned char n = 24;
    /* clear the interrupt status */
    if (GPIO_GetPinsInterruptFlags(GPIO2)&(1 << n))    //ÅÐ¶ÏÊÇ·ñÎªGPIO5_0ÖÐ¶Ï
    {
        keybuff = GPIO_PinRead(GPIO2, n);
        if (keybuff == 0)
        {
            Button[0] = Press;
        }
        else
        {
            Button[0] = NotPress;
        }
    }
    GPIO_PortClearInterruptFlags(GPIO2, 1U << n);   //Çå³ý±êÖ¾Î»

    n = 25;
    /* clear the interrupt status */
    if (GPIO_GetPinsInterruptFlags(GPIO2)&(1 << n))    //ÅÐ¶ÏÊÇ·ñÎªGPIO5_0ÖÐ¶Ï
    {
        keybuff = GPIO_PinRead(GPIO2, n);
        if (keybuff == 0)
        {
            Button[1] = Press;
        }
        else
        {
            Button[1] = NotPress;
        }
    }
    GPIO_PortClearInterruptFlags(GPIO2, 1U << n);   //Çå³ý±êÖ¾Î»

    /* Change state of switch. */
    __DSB();				//Êý¾ÝÍ¬²½ÆÁ±ÎÖ¸Áî
}

void GPIO1_Combined_16_31_IRQHandler(void)
{
    unsigned char keybuff = 0;
    unsigned char n = 16;
    /* clear the interrupt status */
    if (GPIO_GetPinsInterruptFlags(GPIO1)&(1 << n))    //ÅÐ¶ÏÊÇ·ñÎªGPIO5_0ÖÐ¶Ï
    {
        keybuff = GPIO_PinRead(GPIO1, n);
        if (keybuff == 0)
        {
            Button[2] = Press;
        }
        else
        {
            Button[2] = NotPress;
        }
    }
    GPIO_PortClearInterruptFlags(GPIO1, 1U << n);   //Çå³ý±êÖ¾Î»

    /* Change state of switch. */
    __DSB();				//Êý¾ÝÍ¬²½ÆÁ±ÎÖ¸Áî
}

int ButtonOnceBuffFlag[3] = { 0 };//°´¼ü°´ÏÂÒ»´Î»º´æ±êÖ¾
int ButtonOnceFlag[3] = { 0 };//°´¼ü°´ÏÂÒ»´ÎµÄ±êÖ¾
int QuitSetFlag = 0;
extern float Battery_V;
///// <summary>
/////°´¼ü²Ëµ¥³ÌÐò£¬ÓÃÓÚ²ÎÊýÉè¶¨µÈ¹¦ÄÜ£¬·ÅÓÚÖ÷º¯ÊýµÄÖ÷Òª¹¦ÄÜÇ°
/////<para>×¢£ºÒ»¶¨Òª·ÅÔÚTFT³õÊ¼»¯ºó£¬ÁíÍâÆäËûÓÐÖÐ¶ÏµÄÄ£¿é³õÊ¼»¯¶¼±ØÐë·ÅÔÚÕâ¸öº¯ÊýºóÃæ</para>
/////</summary>
void ButtonMenu()
{
    BatteryVoltageCollect_Init(1);
    ButtonInit();
    
    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (Button[i] == Press)
            {
                ButtonOnceBuffFlag[i] = 1;
            }
            if ((Button[i] == NotPress) && (ButtonOnceBuffFlag[i] == 1))
            {
                ButtonOnceFlag[i] = 1;
                ButtonOnceBuffFlag[i] = 0;
            }
        }

        if (ButtonOnceFlag[0] == 1)
        {
            ButtonOnceFlag[0] = 0;
            /* ÔÚ´Ë±àÐ´°´ÏÂ°´¼ü1µÄ´¦Àí³ÌÐò */
            TFT_showstr(0, 0, "Button1 Pressed", RED, WHITE);
        }
        if (ButtonOnceFlag[1] == 1)
        {
            ButtonOnceFlag[1] = 0;
            /* ÔÚ´Ë±àÐ´°´ÏÂ°´¼ü2µÄ´¦Àí³ÌÐò */
            TFT_showstr(0, 0, "Button2 Pressed", RED, WHITE);

        }
        if (ButtonOnceFlag[2] == 1)
        {
            ButtonOnceFlag[2] = 0;
            /* ÔÚ´Ë±àÐ´°´ÏÂ°´¼ü3µÄ´¦Àí³ÌÐò */
            TFT_showstr(0, 0, "Button3 Pressed", RED, WHITE);
            QuitSetFlag = 1;
        }
        if (QuitSetFlag == 1)
        {
            dsp_single_colour(WHITE);
            TFT_showstr(0, 0, "QuitButtonSet", RED, WHITE);
            Flag_TFTShow = !Flag_TFTShow;
            break;
        }
        Battery_V = GetBatteryVoltage(7.5);
        LCD_ShowBatteryVoltage(0,1,Battery_V);
    }
    if(Battery_V < 7.5)
      BeepAlarm(2);
    else if(Battery_V < 7.9)
      BeepAlarm(3);  
    else
      BeepAlarm(4); 
    
}

typedef enum
{
    FrontCameraSet=0,
    BackCameraSet,
    SpeedSet
}Enum_MenuStatus;
Enum_MenuStatus MenuStatus = FrontCameraSet;

int Flag_SpeedFixed = 0;
int Flag_BackCar = 0;
int Flag_StopCarWithTimer = 0;
int StopTimeLength = 0;

int ParaSetColor_FrontCameraSet[4] = { BLACK, BLACK, BLACK, BLACK };
int ParaSetColor_BackCameraSet[4] = { BLACK, BLACK, BLACK, BLACK };
int ParaSetColor_ModeSet[4] = { BLACK, BLACK, BLACK, BLACK };
void ShowMenu(int Para_SelectedIndex)
{
    //dsp_single_colour(WHITE);

    if (MenuStatus == FrontCameraSet)
    {
        TFT_showstr(0, 0, "FrontCameraSet", BLACK, WHITE);

        for (int i = 0; i < 4; i++)
        {
            ParaSetColor_FrontCameraSet[i] = BLACK;
        }
        ParaSetColor_FrontCameraSet[Para_SelectedIndex] = RED;

        TFT_showstr(0, 1, "Graph_F", ParaSetColor_FrontCameraSet[0], WHITE);
        TFT_showint8(90, 1, graph_offset_front, ParaSetColor_FrontCameraSet[0], WHITE);
        TFT_showstr(0, 2, "Trail_F_L", ParaSetColor_FrontCameraSet[1], WHITE);
        TFT_showint8(90, 2, trail_offset_front_left, ParaSetColor_FrontCameraSet[1], WHITE);
        TFT_showstr(0, 3, "Trail_F_R", ParaSetColor_FrontCameraSet[2], WHITE);
        TFT_showint8(90, 3, trail_offset_front_right, ParaSetColor_FrontCameraSet[2], WHITE);
        TFT_showstr(0, 4, "Speed_F", ParaSetColor_FrontCameraSet[3], WHITE);
        TFT_showint8(90, 4, speed_offset_front, ParaSetColor_FrontCameraSet[3], WHITE);

    }
    else if (MenuStatus == BackCameraSet)
    {
        TFT_showstr(0, 0, "BackCameraSet", BLACK, WHITE);

        for (int i = 0; i < 4; i++)
        {
            ParaSetColor_BackCameraSet[i] = BLACK;
        }
        ParaSetColor_BackCameraSet[Para_SelectedIndex] = RED;

        TFT_showstr(0, 1, "Graph_B", ParaSetColor_BackCameraSet[0], WHITE);
        TFT_showint8(90, 1, graph_offset_back, ParaSetColor_BackCameraSet[0], WHITE);
        TFT_showstr(0, 2, "Trail_B_L", ParaSetColor_BackCameraSet[1], WHITE);
        TFT_showint8(90, 2, trail_offset_back_left, ParaSetColor_BackCameraSet[1], WHITE);
        TFT_showstr(0, 3, "Trail_B_R", ParaSetColor_BackCameraSet[2], WHITE);
        TFT_showint8(90, 3, trail_offset_back_left, ParaSetColor_BackCameraSet[2], WHITE);
        TFT_showstr(0, 4, "Speed_B", ParaSetColor_BackCameraSet[3], WHITE);
        TFT_showint8(90, 4, speed_offset_back, ParaSetColor_BackCameraSet[3], WHITE);
    }
    else if (MenuStatus == SpeedSet)
    {
        TFT_showstr(0, 0, "SpeedSet", BLACK, WHITE);

        for (int i = 0; i < 4; i++)
        {
            ParaSetColor_ModeSet[i] = BLACK;
        }
        ParaSetColor_ModeSet[Para_SelectedIndex] = RED;

        TFT_showstr(0, 1, "FixedSpeed?", ParaSetColor_ModeSet[0], WHITE);
        TFT_showint8(90, 1, Flag_SpeedFixed, ParaSetColor_ModeSet[0], WHITE);
        TFT_showstr(0, 2, "BackCar?", ParaSetColor_ModeSet[1], WHITE);
        TFT_showint8(90, 2, Flag_BackCar, ParaSetColor_ModeSet[1], WHITE);
        TFT_showstr(0, 3, "StopCar?", ParaSetColor_ModeSet[2], WHITE);
        TFT_showint8(90, 3, Flag_StopCarWithTimer, ParaSetColor_ModeSet[2], WHITE);
        TFT_showstr(0, 4, "StopTime", ParaSetColor_ModeSet[3], WHITE);
        TFT_showint8(90, 4, StopTimeLength, ParaSetColor_ModeSet[3], WHITE);
    }
}
int ParaSetIndex = 0;
int ButtonPressedCount[3] = { 0, 0, 0 };
void ButtonMenu_SetPara()
{
    BatteryVoltageCollect_Init(1);
    ButtonInit();
    int * NowSetPara = &graph_offset_front;
    while (1)
    {
        ShowMenu(ParaSetIndex);
        if (MenuStatus == FrontCameraSet)
        {
            if (ParaSetIndex == 0)
                NowSetPara = &graph_offset_front;
            else if (ParaSetIndex == 1)
                NowSetPara = &trail_offset_front_left;
            else if (ParaSetIndex == 2)
                NowSetPara = &trail_offset_front_right;
            else if (ParaSetIndex == 3)
                NowSetPara = &speed_offset_front;
        }
        else if (MenuStatus == BackCameraSet)
        {
            if (ParaSetIndex == 0)
                NowSetPara = &graph_offset_back;
            else if (ParaSetIndex == 1)
                NowSetPara = &trail_offset_back_left;
            else if (ParaSetIndex == 2)
                NowSetPara = &trail_offset_back_right;
            else if (ParaSetIndex == 3)
                NowSetPara = &speed_offset_back;
        }
        else if (MenuStatus == SpeedSet)
        {
            if (ParaSetIndex == 0)
                NowSetPara = &Flag_SpeedFixed;
            else if (ParaSetIndex == 1)
                NowSetPara = &Flag_BackCar;
            else if (ParaSetIndex == 2)
                NowSetPara = &Flag_StopCarWithTimer;
            else if (ParaSetIndex == 3)
                NowSetPara = &StopTimeLength;
        }
        /*扫描按键标志位*/
        for (int i = 0; i < 3; i++)
        {
            if (Button[i] == Press)
            {
                ButtonOnceBuffFlag[i] = 1;
                ButtonPressedCount[i]++;
            }
            if ((Button[i] == NotPress) && (ButtonPressedCount[i] >= 10))
            {
                ButtonOnceFlag[i] = 2;
                ButtonOnceBuffFlag[i] = 0;
                ButtonPressedCount[i] = 0;
            }
            else if ((Button[i] == NotPress) && (ButtonOnceBuffFlag[i] == 1))
            {
                ButtonOnceFlag[i] = 1;
                ButtonOnceBuffFlag[i] = 0;
                ButtonPressedCount[i] = 0;
            }
        }
        /*按键参数+1*/
        if (ButtonOnceFlag[0] == 1)
        {
            ButtonOnceFlag[0] = 0;
            /* 在此编写按下按键1的处理程序 */
            if (MenuStatus == SpeedSet && ParaSetIndex != 3)
            {
                (*NowSetPara) = !(*NowSetPara);
            }
            else
                (*NowSetPara)++;
        }
        /*长按切换至下一个配置模式*/
        else if (ButtonOnceFlag[0] == 2)
        {
            ButtonOnceFlag[0] = 0;
            /* 在此编写按下按键1的处理程序 */
            MenuStatus++;
            if (MenuStatus > 2)
                MenuStatus = 0;
            ParaSetIndex = 0;
            dsp_single_colour(WHITE);
        }
        /*按键参数-1*/
        if (ButtonOnceFlag[1] == 1)
        {
            ButtonOnceFlag[1] = 0;
            /* 在此编写按下按键2的处理程序 */
            if (MenuStatus == SpeedSet && ParaSetIndex != 3)
            {
                (*NowSetPara) = !(*NowSetPara);
            }
            else
                (*NowSetPara)--;
        }
        /*长按切换至上一个配置模式*/
        else if (ButtonOnceFlag[1] == 2)
        {
            ButtonOnceFlag[1] = 0;
            /* 在此编写按下按键1的处理程序 */
            MenuStatus--;
            if (MenuStatus < 0)
                MenuStatus = 2;
            ParaSetIndex = 0;
            dsp_single_colour(WHITE);
        }
        /*短按切设置选项*/
        if (ButtonOnceFlag[2] == 1)
        {
            ButtonOnceFlag[2] = 0;
            /* 在此编写按下按键3的处理程序 */         
            if (ParaSetIndex >= 3)
                ParaSetIndex = 0;
            else
                ParaSetIndex++;
        }
        /*长按跳出参数配置*/
        else if (ButtonOnceFlag[2] == 2)
        {
            ButtonOnceFlag[2] = 0;
            /* 在此编写按下按键3的处理程序 */
            break;
        }
    }
    dsp_single_colour(WHITE);
    TFT_showstr(0, 0, "QuitButtonSet", RED, WHITE);
    Flag_TFTShow = !Flag_TFTShow;
}
int UseBeepFlag = 0;
///<summary>µçÁ¿²É¼¯</summary>
void BatteryVoltageCollect_Init(int IfUseBeep)
{
    LQADC_Init(BatteryCollectADC);
    UseBeepFlag = IfUseBeep;
    if(IfUseBeep)
    {
      IOMUXC_SetPinMux(
        IOMUXC_GPIO_EMC_41_GPIO3_IO27,          /* GPIO_EMC_41 is configured as GPIO3_IO27 */
        0U); 
      /* GPIO configuration of DC on GPIO_SD_B1_03 (pin M4) */
      gpio_pin_config_t Out_config = {
          .direction = kGPIO_DigitalOutput,
          .outputLogic = 1U,
          .interruptMode = kGPIO_NoIntmode
      };
      /* Initialize GPIO functionality on GPIO_SD_B1_03 (pin M4) */
      GPIO_PinInit(GPIO3, 27, &Out_config);
  }
}

///<summary>·äÃùÆ÷</summary>
unsigned char AlarmLenght = 4;
BeepAlarmStatus AlarmStatus;
void BeepAlarm(AlarmNum)
{
    if (AlarmStatus == Off)
    {
        AlarmLenght = AlarmNum;
        AlarmStatus = Alarming;
    }
}

uint8_t Flag_BeepOn = 0;
long BeepOnCount_1ms = 0;
uint8_t AlarmNumCount = 0;
void BeepTimerInter()
{
    if (AlarmStatus == Alarming)
    {
        if (Flag_BeepOn)
        {
            Flag_BeepOn = 0;
            GPIO_WritePinOutput(GPIO3, 27, 1);
            AlarmNumCount++;
        }
        else
        {
            Flag_BeepOn = 1;
            GPIO_WritePinOutput(GPIO3, 27, 0);        
        }
        if (AlarmNumCount >= AlarmLenght)
        {
            AlarmNumCount = 0;
            BeepOnCount_1ms = 0;
            AlarmStatus = Off;
        }
    }


}

uint16_t adc_value = 0;
float GetBatteryVoltage(float HintVoltage)
{
    adc_value = ReadADC(BatteryCollectADC, BatteryCollectADCChn);

    float result = adc_value*0.0097232;
    
//    if(UseBeepFlag)
//    {
//      if(result < HintVoltage && result > 2)
//      {
//        GPIO_WritePinOutput(GPIO3, 27, 0);
//    }     
//    else
//    {
//        GPIO_WritePinOutput(GPIO3, 27, 1);        
//    }
//      }
return result;
}

void LCD_ShowBatteryVoltage(unsigned char x, unsigned char y, float num)
{
    TFT_showUfloat(x, y, num, 1, 2, RED, WHITE);
    TFT_showchar(x+32,y+15,'V',RED,WHITE);
}

///<summary>FIFO</summary>
void FIFO(uint8 *head, uint8 depth, uint8 num)
{
  int k;
  for( k = 0;k < depth-1;k++)
  {
    *(head+k) = *(head+k+1);
}
*(head+k) = num;
return;
}

void FIFO_Clean(uint8 *head, uint8 depth)
{
    int i = 0;
    for (i = 0; i < depth; i++)
        *(head+i) = 0;
    return;
}

///<summary>µ¥²ã×îÖµ¼ìË÷</summary>
uint8 One_loop_bubblesort(uint8 *lis, uint8 depth, int flag)
{
  uint8 biggest = 0;
  uint8 smallest = 0;
  uint8 mid =0;
  uint8 *const arry = (uint8 *)malloc(sizeof(uint8)*depth);//ÏòÏµÍ³ÉêÇëÄÚ´æ£¬³¤¶ÈÎª´«µÝ½øÀ´µÄÁÐ±íÉî¶È
  uint8 temp_exchange = 0;
  for (int i = 0; i <= depth-1; i++)
  {
      arry[i] = lis[i];
  }
  if(flag ==1)
  {
        ///<summary>µ¥²ã×î´óÖµ¼ìË÷</summary>
   for (int i = 0; i < depth-1; i++)
   {
    if (arry[i] > arry[i + 1])
    {
        temp_exchange = arry[i + 1];
        arry[i + 1] = arry[i];
        arry[i] = temp_exchange;
    }
}
biggest = arry[depth-1];
return biggest;
}
else if(flag ==2)
{
        ///<summary>µ¥²ã×îÐ¡Öµ¼ìË÷</summary>
    for (int i = 0; i < depth-1; i++)
    {
        if (arry[i] < arry[i + 1])
        {
            temp_exchange = arry[i + 1];
            arry[i + 1] = arry[i];
            arry[i] = temp_exchange;
        }
    }
    smallest = arry[depth-1];
    return smallest;
}
else if(flag ==3)
{        
  for (int i = depth-1; i > (depth-1)/2-1; i--)
  {
      for(int j =0;j < i-1;j++)
      {
        if (arry[j] > arry[j + 1])
        {
            temp_exchange = arry[j + 1];
            arry[j + 1] = arry[j];
            arry[j] = temp_exchange;
        }
    }
}
mid = arry[(int)((depth-1)/2)];
return mid;
}
}
