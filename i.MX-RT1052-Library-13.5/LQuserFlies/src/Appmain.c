/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】CHIUSIR
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2018年11月13日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.20.1及以上版本
【Target 】 i.MX RT1052
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 528MHz
【USB PLL】 480MHz
================================================
下载及调试模式：0010
下载      模式：0001
运行      模式：0010
------------------------------------------------
用JLINKV9/OB及以上版本可以下载SDRAM和QSPI FLASH;
用LQDAPLINKV3可以下载SDRAM和QSPI FLASH;
------------------------------------------------
部分函数尚未验证完毕，默认测试OV7725模块和配套4.3吋彩屏
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
//#include "AHRS.h"    //使用姿态解算时需要添加该头文件

uint32_t fullCameraBufferAddr;     
unsigned char * image;
uint64_t time;
uint64_t now;
void Camera1PinInit();
void Camera2PinInit();
///<summary>速度环参数</summary>
float P_Set[4] = {71, 74, 74, 74};//{33, 19, 22, 21}{20, 20, 20, 17}
float D_Set[4] = {29, 31, 29.2, 29.2};//{8.8, 5, 5, 4}{4, 5, 3, 3}
float I_Set[4] = {8.8, 8.8, 8.8, 8.8};//{4.8, 5, 5, 5}{3.6, 4, 4, 4}
float DeadBand_Set[4] = {300, 200, 260, 240};//{300, 200, 260, 240}
float I_limit = 8000;
float Max_output = 9500;
///<summary>方向环参数</summary>
float P_Direction_Set_init= 5.9;
float D_Direction_Set_init= 0.042;
float Max_output_Direction= 300;
///<summary>定时器参数部分</summary>
volatile int16_t PIT0_Flag = 0;

extern struct RunSpeed RS_Now;
extern long YSpeed_Solved;
long Speed_watch[4];
extern uint8 Series_deviation_received;
extern uint8 Series_distance_received;
extern int direction_flag;

extern float ControlValue_Closeloop[4];
extern long SpeedCount[4];
extern long Speed_get[4];
extern struct PIDControl Car_Speed_Rotate;//追灯转向闭环
extern struct PIDControl WheelControl[4];


uint64_t TestStartTime_us = 0;
uint64_t TestEndTime_us = 0;
uint32_t GYRO_OriginData_Z[5]={0};
uint32_t Angel_YAW;
float Angle_Z = 0;
  int k = 0;

void InitAll();
uint64_t TimeTest_us = 0;
void RunTimeTest()
{
	SEND(ControlValue_Closeloop[0], ControlValue_Closeloop[1], ControlValue_Closeloop[2], ControlValue_Closeloop[3]);

      //TFT_showfloat(0,0,(float)TimeTest_us,7,0,BLACK,WHITE);
}
uint8_t Flag_MeassureDistance = 0;
///<summary>主函数</summary>
float Battery_V = 0;
  int main(void)
{         
	InitAll();
	
	_systime.delay_ms(200);

    //TFTSPI_CLS(u16WHITE);
	while(1)
	{
      /*扫描按键标志位*/
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
      /*按键切换是否TFT显示*/
		if (ButtonOnceFlag[2] == 1)
		{
			ButtonOnceFlag[2] = 0;
          /* 在此编写按下按键3的处理程序 */
			if(Flag_TFTShow)
				dsp_single_colour(WHITE);
			Flag_TFTShow = !Flag_TFTShow;
			if(Flag_TFTShow)
				dsp_single_colour(WHITE);
		}
      //TFT_showfloat(1,0,DistanceMeasure_1.Distance_Meassured, 3,2,BLACK,WHITE);
      //TFT_showfloat(1,1,DistanceMeasure_2.Distance_Meassured, 3,2,BLACK,WHITE);      
      Get_Gyro(&GYRO_OriginData);//z轴为地磁轴,逆时针为正方向，串级控制中D逆时针为负
      if(GYRO_OriginData.Z <0)
      {
        Angel_YAW =-GYRO_OriginData.Z;
      }
      else
      {
       Angel_YAW =  GYRO_OriginData.Z;
      }
      for( k = 0;k < 4;k++)
      {
        GYRO_OriginData_Z[k] = GYRO_OriginData_Z[k+1];
      }
      GYRO_OriginData_Z[4] = Angel_YAW;
//      if(Flag_MeassureDistance)
//      {
////        SendCMDToUltrasound(&DistanceMeasure_1);
//        //SendCMDToUltrasound(&DistanceMeasure_2);
//        Flag_MeassureDistance= 0; 
//      }
//      if (GYRO_OriginData.Z > 3 || GYRO_OriginData.Z < -3)
//      	Angle_Z += GYRO_OriginData.Z * 5.143e-5;
      //Series_Receive_Data_Analysis();
      //TFT_showuint8(1,0,Series_deviation_received,BLACK,WHITE);
      //TFT_showuint8(1,1,Series_distance_received,BLACK,WHITE);
      //SEND(Series_deviation_received, Series_distance_received, Speed_watch[2], Speed_watch[3]);
  }
  
}


void InitAll()
{
    BOARD_ConfigMPU();                   /* 初始化内存保护单元 */
    BOARD_InitSDRAMPins();               /* SDRAM初始化 */
    BOARD_BootClockRUN();                /* 初始化开发板时钟 */
    //BOARD_InitDEBUG_UARTPins();          //UART调试口管脚复用初始化 
    //BOARD_InitDebugConsole();            //UART调试口初始化 可以使用 PRINTF函数          
    //LED_Init();                          //初始化核心板和开发板上的LED接口
    UART_DMA_Init();       //串口1初始化 可以使用 printf函数
    _systime.init();                     //开启systick定时器
    NVIC_SetPriorityGrouping(2);/*设置中断优先级组  0: 0个抢占优先级16位个子优先级
                                *1: 2个抢占优先级 8个子优先级 2: 4个抢占优先级 4个子优先级
                                *3: 8个抢占优先级 2个子优先级 4: 16个抢占优先级 0个子优先级
                                */

    Motor_init();
    //BatteryVoltageCollect_Init(1);
    //GPIO_WritePinOutput(GPIO3, 27, 0);
    Series_Receive_init();
    MPU6050_Init();
    TFT_init();
    RemoteData_init();   
    
    ButtonMenu_SetPara();

    //ButtonMenu();
//    DistanceMeasureStructInit(&DistanceMeasure_1, 1);
//    UltrasoundInit(&DistanceMeasure_1);
    //DistanceMeasureStructInit(&DistanceMeasure_2, 2);
    //UltrasoundInit(&DistanceMeasure_2);    
    
    //GPIO_WritePinOutput(GPIO3, 27, 1);
    PID_Speedloop_init(P_Set, D_Set, I_Set, I_limit, Max_output, DeadBand_Set);
    PID_locationloop_init(P_Direction_Set_init, 0, 0, 0, Max_output_Direction, 0);//位置环参数     
    
    EncoderMeasure_Init();
    RemoteInit();
    //camera_init_1();
    
    LQ_PIT_Init(kPIT_Chnl_0, 3500);//3000us
    LQ_PIT_Init(kPIT_Chnl_1, BeepTimeSet*1000);// 
    //LQ_PIT_Init(kPIT_Chnl_2, 15000);// 
}
///<summary>定时器部分</summary>
void PIT_IRQHandler(void)
{
	if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_0)&kPIT_TimerFlag) == kPIT_TimerFlag)
	{
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*可自行添加代码*/
		if (PIT0_Flag != 1)
		{
			SpeedClean();
			int i = 0;
			for (i = 0; i < 4; i++)
			{
				GetSpeed(i);
				Speed_get[i] += SpeedCount[i];
			}
			PIT0_Flag += 1;
		}
		else
		{
            //TestStartTime_us = _systime.get_time_us();
			SpeedClean();
			int i = 0;
			for (i = 0; i < 4; i++)
			{
				GetSpeed(i);
				Speed_get[i] += SpeedCount[i];
			}      
                        Series_Receive_Data_Analysis();
            //SendCMDToUltrasound();//超声波测距请求
            //遥控测试程序
            #ifdef Remote_UseDigitalReceive
            SetSpeed_FromRemote(RunMode);//数字量
            #else
            SetSpeed_FromRemote_Analog();//模拟量
            #endif
            SEND(ControlValue_Closeloop[0], ControlValue_Closeloop[1], ControlValue_Closeloop[2], ControlValue_Closeloop[3]);           
            //控制量输出//	
            Series_Control(Series_deviation_received);
            //电机转向测试//
            LQ_PWMA_B_SetDuty(PWMType_Use4, Wheels_PWMChannel[3], 2000, 0);
            //编码器观测//
			int j = 0;
			for (j = 0; j < 4; j++)
			{
				Speed_watch[j] = Speed_get[j];
                Speed_get[j] = 0;//速度计清零
            }
#ifndef Remote_UseDigitalReceive
            //SendRemoteCMDData();

#endif
//            TestEndTime_us = _systime.get_time_us();
//            TimeTest_us = TestEndTime_us-TestStartTime_us;
            //SEND(Speed_watch[0], Speed_watch[1],Speed_watch[2], Speed_watch[3]);
            //SEND(Series_deviation_received, Car_Speed_Rotate.Error, ((int16_t)(Angle_Z * 10) / 10) % 360, Speed_watch[3]);
            //SEND((int)GYRO_OriginData.Z*0.01, Series_distance_received*10, RS_Now.YSpeed, YSpeed_Solved);
            PIT0_Flag = 0;
        }
    }
    if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_1)&kPIT_TimerFlag) == kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);    /* Clear interrupt flag.*/
        /*可自行添加代码*/
    	BeepTimerInter();
    }
    if ((PIT_GetStatusFlags(PIT, kPIT_Chnl_2)&kPIT_TimerFlag) == kPIT_TimerFlag)
    {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);    /* Clear interrupt flag.*/ 
        Flag_MeassureDistance = 1;
        
    }
}
void Camera1PinInit()
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */
    CLOCK_EnableClock(kCLOCK_Xbar1);            /* xbar1 clock (xbar1_clk_enable): 0x03U */

	IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_00_GPIO2_IO00,           /* GPIO_B0_00 is configured as GPIO2_IO00 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
		IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_01_GPIO2_IO01,           /* GPIO_B0_01 is configured as GPIO2_IO01 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
			IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_02_GPIO2_IO02,           /* GPIO_B0_02 is configured as GPIO2_IO02 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
				IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_03_GPIO2_IO03,           /* GPIO_B0_03 is configured as GPIO2_IO03 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
					IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_04_GPIO2_IO04,           /* GPIO_B0_04 is configured as GPIO2_IO04 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
						IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_05_GPIO2_IO05,           /* GPIO_B0_05 is configured as GPIO2_IO05 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
							IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_06_GPIO2_IO06,           /* GPIO_B0_06 is configured as GPIO2_IO06 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
								IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_07_GPIO2_IO07,           /* GPIO_B0_07 is configured as GPIO2_IO07 */
									0U);

								IOMUXC_SetPinMux(
        IOMUXC_GPIO_EMC_41_GPIO3_IO27,          /* GPIO_EMC_41 is configured as GPIO3_IO27 */
									0U);

								IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B0_01_GPIO3_IO13,        /* GPIO_SD_B0_01 is configured as GPIO3_IO13 */
									0U);

								IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07,     /* GPIO_SD_B0_03 is configured as XBAR1_INOUT07 */
									0U);

								IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
        (~(IOMUXC_GPR_GPR6_QTIMER1_TRM0_INPUT_SEL_MASK | IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15_MASK))) /* Mask bits to zero which are setting */
        | IOMUXC_GPR_GPR6_QTIMER1_TRM0_INPUT_SEL(0x01U) /* QTIMER1 TMR0 input select: input from XBAR */
        | IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL(0x01U) /* QTIMER2 TMR0 input select: input from XBAR */
        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_6(0x00U) /* IOMUXC XBAR_INOUT6 function direction select: XBAR_INOUT as input */
        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U) /* IOMUXC XBAR_INOUT7 function direction select: XBAR_INOUT as input */
        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_11(0x00U) /* IOMUXC XBAR_INOUT11 function direction select: XBAR_INOUT as input */
        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_15(0x00U) /* IOMUXC XBAR_INOUT15 function direction select: XBAR_INOUT as input */
									);

								XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputQtimer1Tmr0Input);

							}

							void Camera2PinInit()
							{
								IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_00_GPIO2_IO16,           /* GPIO_B1_00 is configured as GPIO2_IO16 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
									IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_01_GPIO2_IO17,           /* GPIO_B1_01 is configured as GPIO2_IO17 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
										IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_02_GPIO2_IO18,           /* GPIO_B1_02 is configured as GPIO2_IO18 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
											IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_03_GPIO2_IO19,           /* GPIO_B1_03 is configured as GPIO2_IO19 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
												IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_04_GPIO2_IO20,           /* GPIO_B1_04 is configured as GPIO2_IO20 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
													IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_05_GPIO2_IO21,           /* GPIO_B1_05 is configured as GPIO2_IO21 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
														IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_06_GPIO2_IO22,           /* GPIO_B1_06 is configured as GPIO2_IO22 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
															IOMUXC_SetPinMux(
        IOMUXC_GPIO_B1_07_GPIO2_IO23,           /* GPIO_B1_07 is configured as GPIO2_IO23 */
																0U);

															IOMUXC_SetPinMux(
        IOMUXC_SNVS_PMIC_STBY_REQ_GPIO5_IO02,   /* PMIC_STBY_REQ is configured as GPIO5_IO02 */
																0U);

															IOMUXC_SetPinMux(
        IOMUXC_GPIO_B0_15_GPIO2_IO15,           /* GPIO_B0_15 is configured as GPIO2_IO15 */
																0U);

															IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B0_15_XBAR1_IN25,        /* GPIO_AD_B0_15 is configured as XBAR1_IN25 */
																0U);

															XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn25, kXBARA1_OutputQtimer2Tmr0Input);
														}
