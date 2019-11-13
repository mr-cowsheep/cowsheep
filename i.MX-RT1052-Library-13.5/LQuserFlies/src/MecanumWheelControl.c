
/*!
* @file       MecanumWheelControl.c
* @brief      四轮麦轮控制部分的程序
* @details
* @author     pig's grief
* @version    v1.0
* @date       2019-2-12
* @to do      
*             解决编码器测速程序的DMA无法计数问题
*/

# include "include.h"
# include "MIMXRT1052.h"
//规定了每个轮子对应的FTM输出通道，调正反转应该这个数组
pwm_submodule_t Wheels_PWMChannel[4] = { PWMChannel_Use_W1,
 PWMChannel_Use_W2,
 PWMChannel_Use_W3,
                                         PWMChannel_Use_W4};//1左前 A+ B- 2右前 A- B+ 3右后 A- B+ 4左后 A- B+

//PWM_Type *PWM_Type[4] = { PWMType_Use1, PWMType_Use2, PWMType_Use3, PWMType_Use4 };




///<summary>电机通道初始化</summary>
                                         void Motor_init(void)
                                         {
                                          LQ_PWM_Init(PWMType_Use1, Wheels_PWMChannel[0], kPWM_PwmA_B, 10000);
                                          LQ_PWM_Init(PWMType_Use2, Wheels_PWMChannel[1], kPWM_PwmA_B, 10000);
                                          LQ_PWM_Init(PWMType_Use3, Wheels_PWMChannel[2], kPWM_PwmA_B, 10000);
                                          LQ_PWM_Init(PWMType_Use4, Wheels_PWMChannel[3], kPWM_PwmA_B, 10000);
                                        }

///<summary>对应PIDControl结构体内的f_Constructor</summary>
                                        void PIDControl_Constructor(struct PIDControl* PID, float P_set, float D_set, float I_set, float I_Limit_Set, float MaxOutput_Set, float DeadBand_Set)
                                        {
                                          PID->KP = P_set;
                                          PID->KD = D_set;
                                          PID->KI = I_set;
                                          PID->IntegralLimit = I_Limit_Set;
                                          PID->MaxOutput = MaxOutput_Set;
                                          PID->DeadBand = DeadBand_Set;

                                          PID->P_Output = 0;
                                          PID->D_Output = 0;
                                          PID->I_Output = 0;
                                          PID->Output = 0;
                                          PID->Last_Error = 0;
                                          PID->Last_Output = 0;
                                        }

///<summary>PIDControl结构体内的TargetValue设定</summary>
                                        void PID_SetTarget(struct PIDControl* PID,float TargetValue)
                                        {
                                          PID->TargetValue = TargetValue;
                                        }

///<summary>对应PIDControl结构体内的f_GetPIDControlValue</summary>
                                        float GetPIDControlValue(struct PIDControl* PID, PIDControlModel PIDModel, float MeasuredValue)
                                        {
                                          PID->Last_Error = PID->Error;
                                          PID->Last_Output = PID->Output;

                                          PID->MeasuredValue = MeasuredValue;
                                          PID->Error = PID->TargetValue - PID->MeasuredValue;

                                          PID->P_Output = PID->KP * PID->Error;

                                          if (PIDModel == PID_Control || PIDModel == PD_Control)
                                            PID->D_Output = PID->KD * (PID->Error - PID->Last_Error);
                                          else
                                            PID->D_Output = 0;
                                          if (PIDModel == PID_Control || PIDModel == PI_Control)
                                          {
                                            PID->I_Output += (PID->KI * PID->Error);

                                            if (PID->I_Output > PID->IntegralLimit)
                                              PID->I_Output = PID->IntegralLimit;
                                            if (PID->I_Output < -PID->IntegralLimit)
                                              PID->I_Output = -PID->IntegralLimit;
                                          }
                                          else
                                            PID->I_Output = 0;

                                          PID->Output = PID->P_Output + PID->D_Output + PID->I_Output;
                                          if (PID->Output > 0)
                                            PID->Output += PID->DeadBand;
                                          else
                                            PID->Output -= PID->DeadBand;

                                          if (PID->Output > PID->MaxOutput)
                                            PID->Output = PID->MaxOutput;
                                          if (PID->Output < -PID->MaxOutput)
                                            PID->Output = -PID->MaxOutput;

                                          return PID->Output;
                                        }
///<summary>对应PIDControl结构体内的f_para_Refresh</summary>
                                        void Para_Refresh(struct PIDControl* PID, float kp, float kd, float ki)
                                        {
                                          PID->KP = kp;
                                          PID->KD = kd;
                                          PID->KI = ki;
                                        }

/// <summary>
///使结构体PIDControl中的函数指针指向具体定义的函数
///<para>example:  PIDControl_FunctionDefine(&amp;PIDTest);</para>
///</summary>
/// <param name="PID">PIDControl结构体</param>
                                        void PIDControl_FunctionDefine(struct PIDControl* PID)
                                        {
                                          PID->f_Constructor = PIDControl_Constructor;
                                          PID->f_GetPIDControlValue = GetPIDControlValue;
                                          PID->f_para_Refresh = Para_Refresh;
                                        }





struct PIDControl WheelControl[4];//四个轮子的PID控制数组
struct PIDControl Car_Speed_Rotate;//追灯转向闭环




///麦轮控制部分

/// <summary>
///设定整车运动速度（x、y轴平动速度和自转速度）
///<para>example:  SetTargetSpeed_Car(&amp;PIDTest, 10, 15, 20);</para>
///</summary>
/// <param name="TargetSpeed">用来存储三个速度的结构体</param>
/// <param name="Vx">x轴平动速度</param>
/// <param name="Vy">y轴平动速度</param>
/// <param name="W_yaw">自转角速度</param>
void SetTargetSpeed_Car(struct RunSpeed * TargetSpeed, float Vx, float Vy, float W_yaw)
{
  TargetSpeed->XSpeed = Vx;
  TargetSpeed->YSpeed = Vy;
  TargetSpeed->YawSpeed = W_yaw;
}
/// <summary>
///根据整车的运动状态来计算每个麦轮需要输出的速度值
///<para>example:  CalTargetSpeed_EachWheel(&amp;TargetSpeed);</para>
///</summary>
/// <param name="TargetSpeed">车整体的三个运动速度结构体</param>
void CalTargetSpeed_EachWheel(struct RunSpeed * TargetSpeed)
{
    ///O-长方形麦轮底盘的逆运动学模型
    ///用底盘运动状态解算四个轮子应有的速度
  WheelControl[0].TargetValue = TargetSpeed->YSpeed + TargetSpeed->XSpeed - TargetSpeed->YawSpeed;
  WheelControl[1].TargetValue = TargetSpeed->YSpeed - TargetSpeed->XSpeed + TargetSpeed->YawSpeed;
  WheelControl[2].TargetValue = TargetSpeed->YSpeed + TargetSpeed->XSpeed + TargetSpeed->YawSpeed;
  WheelControl[3].TargetValue = TargetSpeed->YSpeed - TargetSpeed->XSpeed - TargetSpeed->YawSpeed;
}

//PID结构体内缺乏一个范用的的设定目标的函数

///<summary>根据四个控制量ControlValue来输出四个电机的占空比</summary>
void MotorOutput(float * ControlValue)
{
  int i = 0;
  if (ControlValue[i] >= 0)
  {
    LQ_PWMA_B_SetDuty(PWMType_Use1, Wheels_PWMChannel[i], (uint16_t)(ControlValue[i]), 0);
  }
  else
  {
    LQ_PWMA_B_SetDuty(PWMType_Use1, Wheels_PWMChannel[i], 0, (uint16_t)(-ControlValue[i]));
  }
  i = 1;
  if (ControlValue[i] >= 0)
  {
    LQ_PWMA_B_SetDuty(PWMType_Use2, Wheels_PWMChannel[i], 0, (uint16_t)(ControlValue[i]));
  }
  else
  {
    LQ_PWMA_B_SetDuty(PWMType_Use2, Wheels_PWMChannel[i], (uint16_t)(-ControlValue[i]), 0);
  }
  i = 2;
  if (ControlValue[i] >= 0)
  {
    LQ_PWMA_B_SetDuty(PWMType_Use3, Wheels_PWMChannel[i], 0, (uint16_t)(ControlValue[i]));
  }
  else
  {
    LQ_PWMA_B_SetDuty(PWMType_Use3, Wheels_PWMChannel[i], (uint16_t)(-ControlValue[i]), 0);
  }
  i = 3;
  if (ControlValue[i] >= 0)
  {
    LQ_PWMA_B_SetDuty(PWMType_Use4, Wheels_PWMChannel[i], 0, (uint16_t)(ControlValue[i]));
  }
  else
  {
    LQ_PWMA_B_SetDuty(PWMType_Use4, Wheels_PWMChannel[i], (uint16_t)(-ControlValue[i]), 0);
  }
}
float ControlValue_Start[4] = { 0 };
float ControlValue_Closeloop[4];//闭环占空比输出
struct RunSpeed RS_Now;//用于解算CalTargetSpeed_EachWheel的结构体

//编码器部分
int16_t temp_Speed[4] = { 0 };
long SpeedCount[4] = { 0 };
long Speed_get[4] = { 0 };
//编码器测速用的ENC编号
ENC_Type* Encoder_ENCIndex[4] = { ENC1, ENC2, ENC3, ENC4 };
//编码器测速用的旋转方向Port总端口
GPIO_Type* EncoderDirectionPort[4] = { GPIO3, GPIO3, GPIO2, GPIO2 };
//编码器测速用的旋转方向Port端口索引
uint32_t Encoder_PORTIndex_Direction[4] = { 15, 17, 27, 26 };

/// <summary>
///逆解算Y轴
///<para>example:  CalTargetSpeed_EachWheel(&amp;TargetSpeed);</para>
///</summary>
/// <param name="TargetSpeed">车整体的三个运动速度结构体</param>

long Solve_YSpeed(void)
{
  long Speed =0;
  Speed = Speed_get[0]+Speed_get[1]+Speed_get[2]+Speed_get[3];
  Speed = (long)Speed*0.25;
  return Speed;
}

///<summary>四个编码器初始化</summary>
void EncoderMeasure_Init(void)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */

  IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B0_03_GPIO3_IO15,        /* GPIO_SD_B0_00 is configured as GPIO3_IO12 */
    0U);

  IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B0_05_GPIO3_IO17,          /* GPIO_EMC_39 is configured as GPIO3_IO25 */
    0U);

  IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_11_GPIO2_IO27,           /* GPIO_B1_11 is configured as GPIO2_IO27 */
    0U);

  IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_10_GPIO2_IO26,           /* GPIO_B1_10 is configured as GPIO2_IO26 */
    0U);

    gpio_pin_config_t GPIO_ENCDirection_Config = { kGPIO_DigitalInput,    //GPIO为输入方向
        1,                    //高电平
        kGPIO_NoIntmode,      //不触发中断
      };

      for (int i = 0; i < 4; i++)
      {
        LQ_ENC_Init(Encoder_ENCIndex[i]);   //正交解码初始化

        GPIO_PinInit(EncoderDirectionPort[i], Encoder_PORTIndex_Direction[i], &GPIO_ENCDirection_Config);

        SpeedCount[i] = 0;
        temp_Speed[i] = 0;
      }

    }
/*===================================================================
功能：排列出N个最大值分别对应数组下标0、1、2、……、n-1
输入：数据->arr数组，数据个数->size,最大值个数->maxnum
函数测试：6.4日测试无BUG
===================================================================*/
/// <summary>
/// 排列出N个最大值
/// </summary>
    void Sort_MAX_N(int16_t arr[], int size, int maxnum)
    {
      int i = 0, k = 0;
      int temp = 0;
      for (k = 0; k<maxnum; k++)
      {
        //降序排列！
        for (i = size - 1; i>k; i--)
        {
          if (arr[i]>arr[i - 1])
          {
            temp = arr[i - 1];
            arr[i - 1] = arr[i];
            arr[i] = temp;
          }
        }
      }
    }
/// <summary>
/// N窗口限幅中值滤波
/// </summary>
    int16_t fiter_Medium(int16_t num, int16_t * queue, int16_t * queue_buff, int size)
    {
      int i = 0;


    ////!+<<<<<<<<<<<<<<<<<<<<限幅>>>>>>>>>>>>>>>>>>>>+//
    //if (num - queue[0] < -4 && num - queue[0]>4)
    //{
    //    num = queue[0];
    //}

    //!+<<<<<<<<<<<<<<<<<<<<更新队列>>>>>>>>>>>>>>>>>>>>+//
      for (i = size - 1; i > 0; i--)
      {
        queue[i] = queue[i - 1];
      }
      queue[0] = num;

    //!+<<<<<<<<<<<<<<<<<<<<找中位值>>>>>>>>>>>>>>>>>>>>+//
      for (i = 0; i < size; i++)
        queue_buff[i] = queue[i];

      Sort_MAX_N(queue_buff, size, (1 + size) / 2);

      return queue_buff[(1 + size) / 2 - 1];
    }

# define MediumFilterSize 5
    int16_t temp_Speed_W1[MediumFilterSize];
    int16_t temp_Speed_W2[MediumFilterSize];
    int16_t temp_Speed_W3[MediumFilterSize];
    int16_t temp_Speed_W4[MediumFilterSize];
    int16_t temp_Speed_buff[MediumFilterSize];

//四个轮子反转时的旋转方向IO
    uint8_t Flag_Reverse[4] = { 0, 0, 1, 1 };

///<summary>获得第index个轮子的转速</summary>
    void GetSpeed(int index)
    {
      int16_t * temp_Speed;
      if (index == 0)
        temp_Speed = temp_Speed_W1;
      else if (index == 1)
        temp_Speed = temp_Speed_W2;
      else if (index == 2)
        temp_Speed = temp_Speed_W3;
      else if (index == 3)
        temp_Speed = temp_Speed_W4;

      uint8_t flag_temp = 0;
      int16_t SpeedBuff = 0;

      SpeedBuff = ENC_GetPositionDifferenceValue(Encoder_ENCIndex[index]);
      
      flag_temp = GPIO_PinRead(EncoderDirectionPort[index], Encoder_PORTIndex_Direction[index]);
      if (flag_temp == Flag_Reverse[index])
      {
        SpeedBuff *= -1;
      }

      SpeedBuff = fiter_Medium(SpeedBuff, temp_Speed, temp_Speed_buff, MediumFilterSize);

      SpeedCount[index] += SpeedBuff;
    }

///<summary>速度计数清零</summary>
    void SpeedClean(void)
    {
      int i = 0;
      for (i = 0; i < 4; i++)
        SpeedCount[i] = 0;
    }


///<summary>远程遥控程序</summary>
    extern int Given_Speed;
#ifdef Remote_UseDigitalReceive
    extern RemoteCMDMode RunMode;
#else
    extern ReceiveCMDData RemoteData;
#endif

#ifdef Remote_UseDigitalReceive
    int Remote_Speed = 0;   
    void SetSpeed_FromRemote(RemoteCMDMode mode)
    {
      Remote_Speed = Given_Speed;
      switch (mode)
      {
        case Left_Left:
        SetTargetSpeed_Car(&RS_Now, -Remote_Speed, 0, 0);
        break;
        case Left_Right:
        SetTargetSpeed_Car(&RS_Now, Remote_Speed, 0, 0);
        break;
        case Left_Up:
        SetTargetSpeed_Car(&RS_Now, 0, Remote_Speed, 0);
        break;
        case Left_Down:
        SetTargetSpeed_Car(&RS_Now, 0, -Remote_Speed, 0);
        break;
        case Left_Return0:
        SetTargetSpeed_Car(&RS_Now, 0, 0, 0);
        break;
//        case Right_Left:
//        SetTargetSpeed_Car(&RS_Now, 0, 0, Remote_Speed);
//        break;
//        case Right_Right:
//        SetTargetSpeed_Car(&RS_Now, 0, 0, -Remote_Speed);
//        break;
//        case Right_Return0:
//        SetTargetSpeed_Car(&RS_Now, 0, 0, 0);
//        break;	
        case Start:
        SetTargetSpeed_Car(&RS_Now, 0, 0, 0);   
        default:
        break;
      }
      CalTargetSpeed_EachWheel(&RS_Now);
      for (int i = 0; i < 4; i++)
      {
        ControlValue_Closeloop[i] = GetPIDControlValue(&WheelControl[i], PID_Control, Speed_get[i]); 
      }
      if (mode ==Start)
        MotorOutput(ControlValue_Start);
      else
        MotorOutput(ControlValue_Closeloop);
    }

#else 
    void SetSpeed_FromRemote_Analog(void)
    {
      float XSpeedBuff = 0;
      if(Distance_Meassured <= 90)
        XSpeedBuff = 200;
      else
        XSpeedBuff = 2*(RemoteData.Left_X-127);
      
      SetTargetSpeed_Car(&RS_Now,XSpeedBuff, 2.8*(RemoteData.Left_Y-127), 0.4*(127-RemoteData.Right_X));
      CalTargetSpeed_EachWheel(&RS_Now);
      for (int i = 0; i < 4; i++)
      {
        ControlValue_Closeloop[i] = GetPIDControlValue(&WheelControl[i], PID_Control, Speed_get[i]); 
      }
      MotorOutput(ControlValue_Closeloop);
    }
#endif

///<summary>串级控制部分</summary>
//设定参数//
    float Rotate_sp = 0;
    float Series_Speed = 200;
    float P_Direction_Set= 0;
    float D_Direction_Set= 0;
    float GYRO_Watch= 0;
    float Input_Control= 0;
    long YSpeed_Solved = 0;
    float V_Ytarget = 0;
    float Given_YSpeed = 0;
    float Yspeed_max = 0;
    float Yspeed_min = 0;
    int Set_XSpeed = 0;
//外部引用//
    extern int direction_flag;
    extern uint8 Front_Distance_ReceiveBuff[5];
    extern uint8 Back_Distance_ReceiveBuff[5];
    extern struct GYROData GYRO_OriginData;
    extern uint8 Series_distance_received;
    extern float P_Direction_Set_init;
    extern float D_Direction_Set_init;
    extern int trail_flag ;
    
    
    extern uint32_t GYRO_OriginData_Z[5];
    uint8 reverse_flag = 0;
    uint8 reverse_flag_2 = 0;
    extern int Flag_BackCar;
    extern int Flag_SpeedFixed;

///<summary>速度环初始化函数</summary>
    void PID_Speedloop_init( float *P_set, float *D_set, float *I_set, float I_Limit_Set, float MaxOutput_Set, float *DeadBand_Set)
    {
      int i = 0;
      for(i = 0;i < 4;i++)
      {
        PIDControl_FunctionDefine(&WheelControl[i]);
        PIDControl_Constructor(&WheelControl[i], P_set[i], D_set[i], I_set[i], I_Limit_Set, MaxOutput_Set, DeadBand_Set[i]);
      }
      
    }
///<summary>位置环初始化</summary>
    void PID_locationloop_init( float P_set, float D_set, float I_set, float I_Limit_Set, float MaxOutput_Set, float DeadBand_Set)
    {
      PIDControl_FunctionDefine(&Car_Speed_Rotate);
      PIDControl_Constructor(&Car_Speed_Rotate, P_set, D_set, I_set, I_Limit_Set, MaxOutput_Set, DeadBand_Set);
    Car_Speed_Rotate.TargetValue = 94;//设定位置环目标值94
  }

///<summary>档位切换</summary>
//状态量//
    Gears Gear_Present = Fifth;
    uint8 Gear_Flag = 0;
void Gears_Judge(float Error)
{
  if(Gear_Flag == 0)
  {
    if(Error > 50)
    {
      Gear_Flag = 20;
      Gear_Present = First;
    }
    else if(Error > 40)
    {
      Gear_Flag = 16;
      Gear_Present = Second;
    }
    else if(Error > 30)
    {
      Gear_Flag = 12;
      Gear_Present = Third;
    }
    else if(Error > 20)
    {
      Gear_Flag = 8;
      Gear_Present = Forth;
    }
    else
    {
      Gear_Present = Fifth;
    }
  }
}

///<summary>串级控制 输入量为位置环偏差 中值94 Y轴</summary>
  void Series_Control(float deviation)
  {
    GYRO_Watch = (int)(GYRO_OriginData.Z*D_Direction_Set_init);            
    YSpeed_Solved = Solve_YSpeed();
    
#ifdef Remote_UseDigitalReceive
    if(RunMode == Start)
      SetTargetSpeed_Car(&RS_Now, 0, 0, 0);
    else
    {
      if(Function_Present == Stationary)
      {
        SetTargetSpeed_Car(&RS_Now, 0, 0, 0);
      }
      else if(Function_Present == Rotate)
        SetTargetSpeed_Car(&RS_Now, 0, 0, 30);
      else
      {
        if(Flag_BackCar == 1)
        {
            uint32_t biggest_t = 0;
            uint32_t arry_t[5] = {0};
  uint32_t temp_exchange_t = 0;
  for (int i = 0; i <= 4; i++)
  {
      arry_t[i] = GYRO_OriginData_Z[i];
  }
        ///<summary>µ¥²ã×î´óÖµ¼ìË÷</summary>
   for (int i = 0; i < 4; i++)
   {
    if (arry_t[i] > arry_t[i + 1])
    {
        temp_exchange_t = arry_t[i + 1];
        arry_t[i + 1] = arry_t[i];
        arry_t[i] = temp_exchange_t;
    }
   }
  biggest_t = arry_t[4];
  if(biggest_t <700)
    reverse_flag = reverse_flag+1;
  else
    reverse_flag = 0;
  if(reverse_flag >120)
  {
   reverse_flag_2 = 40;
  }
        }
  if(reverse_flag_2 !=0)
  { 
   Set_XSpeed = 70;
   Series_Speed = -50;
   Input_Control = 0;
   reverse_flag_2 = reverse_flag_2-1;
  }
    

//        if(direction_flag == -1)
//        {
//        if(DistanceMeasure_1.Distance_Meassured< 25) 
//          Set_XSpeed = 120;
//        else
//          Set_XSpeed = 0;
//        }   
//        else if(direction_flag == 1)
//        {
//        if(DistanceMeasure_2.Distance_Meassured< 25) 
//          Set_XSpeed = 120;
//        else
//          Set_XSpeed = 0;
//        }
  else
  {
    Set_XSpeed = 0;
  Rotate_sp = GetPIDControlValue(&Car_Speed_Rotate, P_Control, deviation);   
  Input_Control=Rotate_sp-GYRO_Watch;
  if(Flag_SpeedFixed == 0)
  {
        if(Function_Present ==Speed_up)
        {
          Series_Speed = 200;
        }
        else if(Function_Present ==Speed_down)
        {
          //YSpeed_Solved = Solve_YSpeed();
          if(direction_flag==1)
          {
            V_Ytarget=90;
//            if(V_Ytarget<0)
//              V_Ytarget = 130;
            Yspeed_max=V_Ytarget*1.6;
            Yspeed_min=V_Ytarget-(200-V_Ytarget)*0.6;
            Given_YSpeed = V_Ytarget - ((long)YSpeed_Solved-V_Ytarget)*0.6;
            if(Given_YSpeed <Yspeed_min)
              Given_YSpeed = Yspeed_min;
            else if(Given_YSpeed >Yspeed_max)
              Given_YSpeed = Yspeed_max;
            Series_Speed = Given_YSpeed;
          }
          else if(direction_flag==-1)
          {
            V_Ytarget=-90;
//            if(V_Ytarget>0)
//              V_Ytarget =-130;
            Yspeed_max=V_Ytarget*1.6;
            Yspeed_min=V_Ytarget-(-200-V_Ytarget)*0.6;
            Given_YSpeed = V_Ytarget - ((long)YSpeed_Solved-V_Ytarget)*0.6;
            if(Given_YSpeed >Yspeed_min)
              Given_YSpeed = Yspeed_min;
            else if(Given_YSpeed <Yspeed_max)
              Given_YSpeed = Yspeed_max;
            Series_Speed = -Given_YSpeed;
          }
        }
        if(Gear_Flag !=0)
        {
          if(Series_Speed > (int)(200-Gear_Flag*10))
            Series_Speed = (int)(200-Gear_Flag*10);
          Gear_Flag = Gear_Flag -1;
        }
      }
  else
  {
    Series_Speed = 90;
  }
  }
        SetTargetSpeed_Car(&RS_Now, trail_flag*direction_flag*Set_XSpeed, direction_flag*Series_Speed, Input_Control);
      }
    }
#else
  Series_Speed = direction_flag*3*(RemoteData.Left_Y-126);//遥控控制前进速度
  if(deviation == 0)
  {
    SetTargetSpeed_Car(&RS_Now, 0, 0, 75);
    FIFO_Clean(Front_Distance_ReceiveBuff, 5);
    FIFO_Clean(Back_Distance_ReceiveBuff, 5);
  }
  else
  {
    Rotate_sp = GetPIDControlValue(&Car_Speed_Rotate, P_Control, deviation);
    Input_Control=Rotate_sp-GYRO_Watch;
    SetTargetSpeed_Car(&RS_Now, 0, Series_Speed, Input_Control);
  }
#endif  
  CalTargetSpeed_EachWheel(&RS_Now);
  for (int i = 0; i < 4; i++)
  {
    ControlValue_Closeloop[i] = GetPIDControlValue(&WheelControl[i], PID_Control, Speed_get[i]); 
  }
  MotorOutput(ControlValue_Closeloop);
}
