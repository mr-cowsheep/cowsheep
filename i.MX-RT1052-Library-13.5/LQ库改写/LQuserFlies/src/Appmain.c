/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1052���İ�-���ܳ���
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2018��11��13��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.20.1�����ϰ汾
��Target �� i.MX RT1052
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 528MHz
��USB PLL�� 480MHz
================================================
���ؼ�����ģʽ��0010
����      ģʽ��0001
����      ģʽ��0010
------------------------------------------------
��JLINKV9/OB�����ϰ汾��������SDRAM��QSPI FLASH;
��LQDAPLINKV3��������SDRAM��QSPI FLASH;
------------------------------------------------
���ֺ�����δ��֤��ϣ�Ĭ�ϲ���OV7725ģ�������4.3������
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include "AHRS.h"    //ʹ����̬����ʱ��Ҫ��Ӹ�ͷ�ļ�
//==========================================================================================  
//���Ժ�������        �����ڲ�ģ�鼰����           ���ܳ���Ӧ�ÿ���               ���״��
//----------------------------------------------------------------------------------------- 
//Test_LED();        //����GPIO�����               LED��ʾ���������              �����
//Test_OLED();       //����ģ��SPI����              OLEDģ�鹦��                   �����
//Test_GPIO_KEY();   //����GPIO����                 ������⹦��                   ����� 
//Test_GPIO_EXINT(); //����GPIO���뼰�ⲿ�ж�       �������жϼ�⹦��             �����     
//LQ_UART_Test();    //����UART���ж�               ������USBתTTL���Լ�ͨ��       �����
//Test_ADC();        //����ADC�ɼ�����              ��Ŵ���������Դ��ѹ���       �����
//Test_PIT();        //����PIT��ʱ�жϹ���                                         �����
//Test_PWM_nCH();    //����PWM                      PWM1��PWM2��PWM4 ����PWM��     �����
//Test_Servo();      //������PWM����                ���ֶ����������               �����
//Test_Motor();      //������PWM����                �����������                   �����  
//Test_OLED_Camera();//����OLED��MT9V034����        OLED��ʾ������������ͷ��ͼ     �����
//Test_ENCoder();    //�������ɼ�                                                  �����
//Text_TFT_480_272();//����480 * 272 ��                                            �����
//Test_TFT_Camera(); //����OV7725 or ���ۺ�TFT4.3 + ������                         �����
//Test_WDG();        //���Կ��Ź�����               ������                       �����
//Test_9AX();        //����I2C���������            �����Ǽ����ٶȹ���             ����� 
//Test_MPU6050();    //����I2C��6�������ǹ���       �����Ǽ����ٶȹ���             ����� 
//Test_EEPROM();     //����EEPROM����               �ڲ�EEPROM���ݴ���             ����� 
//-----------------------------------------------------------------------------------------  
//========================================================================================== 
/*      ����˵��                                                                    ʱ��
 *      1.�Ż� Test_OLED_Camera() 
 *      2.����systick��ʱ������ 
 *      3.�Ż���ʱ����                                                             2018/11/08
 *
 *      1.�Ż� PID
 *      2.�Ż� Scheduler                                                           2018/11/09
 *
 *      1.���Ӵ���+DMA ����                                                        
 *      2.����QTMRģ���PWM��������                                                2018/11/19
 *
 *      1.����QTMRģ���������������                                               
 *      2.�Ż���������ͷSCCB                                                       2018/11/21
 *
 *      1.�Ż���������ͷ֡������                                                   2018/11/27
 *
 *      1.�Ż�7725����ͷ֡������
 *      2.������λ����ʾͼ����                                                   2018/11/28
 *
 *      1.�������188*120�ֱ��ʲ���ͼ����
 *      2.���ͼ�񶶶�����                                                         2019/01/15
 */

void delayms(uint32_t ms)
{
    uint32_t i,j;
    
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 200000; j++)//600M--1ms
        __NOP();//asm("NOP");   
    }
    return ;
}
void delayus(uint32_t us) //��ʱ1us ʵ��Ϊ1.18us   ��ʱ10us ʵ��Ϊ10.02us   ��ʱ100us ʵ��Ϊ98.4us
{
    uint32_t i,j;
    
    for (i = 0; i < us; i++)
    {
        for (j = 0; j < 290; j++)//600M--1us
        __NOP();//asm("NOP");   
    }
    return ;
} 

void DelayTest(void)
{
    _systime.delay_ms(900);
}

uint32_t fullCameraBufferAddr;     
unsigned char * image;
uint64_t time;
uint64_t now;

//volatile bool g_Transfer_Done = false;
//    edma_handle_t g_EDMA_Handle;
//    edma_handle_t g_EDMA_Handle_5;
//    edma_config_t userConfig;
//
//    edma_transfer_config_t transferConfig;
//
///* User callback function for EDMA transfer. */
//void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
//{
//    if (transferDone)
//    {
//        g_Transfer_Done = true;
//    }
//}
//
//void EDMA_Callback_5(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
//{
//    int a = 0;
//    return;
//}
//const edma_channel_Preemption_config_t eDMA_1_DMA5_preemption = {
//  .enableChannelPreemption = true,
//  .enablePreemptAbility = true,
//  .channelPriority = 0
//};
//void TMR2_IRQHandler(void);

AT_NONCACHEABLE_SECTION_INIT(uint32_t databuff[50]) = {0x00};
AT_NONCACHEABLE_SECTION_INIT(uint32_t databuff2[50]) = {0x00};
    short velocity1, velocity2, velocity3, velocity4;

//uint16_t buff_tmr = 0;
int main(void)
{        
    uint8_t count = 0;

    BOARD_ConfigMPU();                   /* ��ʼ���ڴ汣����Ԫ */      
    BOARD_InitSDRAMPins();               /* SDRAM��ʼ�� */
    BOARD_BootClockRUN();                /* ��ʼ��������ʱ�� */   
    BOARD_InitDEBUG_UARTPins();          //UART���ԿڹܽŸ��ó�ʼ�� 
    BOARD_InitDebugConsole();            //UART���Կڳ�ʼ�� ����ʹ�� PRINTF����          
    LED_Init();                          //��ʼ�����İ�Ϳ������ϵ�LED�ӿ�
    LQ_UART_Init(LPUART1, 256000);       //����1��ʼ�� ����ʹ�� printf����
    _systime.init();                     //����systick��ʱ��
    NVIC_SetPriorityGrouping(2);/*�����ж����ȼ���  0: 0����ռ���ȼ�16λ�������ȼ� 
                                 *1: 2����ռ���ȼ� 8�������ȼ� 2: 4����ռ���ȼ� 4�������ȼ� 
                                 *3: 8����ռ���ȼ� 2�������ȼ� 4: 16����ռ���ȼ� 0�������ȼ�
                                 */
    
    //Test_PIT();

    /****************��ӡϵͳʱ��******************/
    PRINTF("\r\n");
    PRINTF("*****LQ_1052*****\r\n");
    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
    PRINTF("Video:           %d Hz\r\n", CLOCK_GetFreq(kCLOCK_VideoPllClk));
    PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
    PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
    PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
    PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
    PRINTF("USB1PLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
    PRINTF("USB1PLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
    PRINTF("USB1PLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
    PRINTF("USB1PLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
    PRINTF("USB1PLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));
    
//-----------------------------------------------------------------------------------------  
//  ���Ժ���������ѭ����ÿ��ֻ�ܿ���һ�����ۺ�Ӧ�������е��ø�ģ���ʼ������
//-----------------------------------------------------------------------------------------
//    Test_ADC();          //����ADC����ʾ��TFT1.8��               ��Ŵ���������Դ��ѹ���  
//    Test_UART();         //����UART���ж�                      ������USBתTTL���Լ�ͨ��  
//    Test_LED();          //����GPIO�����     LED��ʾ���������             
//    Test_GPIO_KEY();     //����GPIO����+TFT1.8                   ������⹦�ܲ���ʾ��TFT1.8��              
//    Test_GPIO_ExInt();   //����GPIO���뼰�ⲿ�ж�              �������жϼ�⹦��    
//    Test_PIT();          //����PIT��ʱ�жϹ���  
//    Test_OLED();         //����OLED
//    Test_RNG();          //���������      
//    Test_9AX();          //������� 
//    Test_MPU6050();      //����I2C��6�������ǹ���              �����Ǽ����ٶȹ��� 
//    Test_ahrs();         //����������̬����   
//    Test_ANO_DT();       //������λ���۲����ݲ���  ��ʹ��DMAģʽ���뽫�궨��#define USE_DMA  0  
//    Test_UART_DMA();     //������λ���۲첨�Σ�ʹ��DMA���䣬��ռ��CPUʱ�䣬���������λ����������У׼�����İ�����
//    Test_PWM_nCH();      //����PWM                             PWM1�� ����PWM��  ��ʾ�����۲�
//    Test_QTMR_PWM();     //����QTMR ������PWM                  ʾ�����۲�
//    Test_QTMR_QD();      //����QTMR�������빦��
//    Test_Servo();        //���ֶ������      ע�� ����ʹ�ô˺���ʱ�������Ҫװ���̣���ֹ���������������ֵ����װ������������������ֵ
//    Test_Motor();        //ֱ������������ԣ�                  ������ȫ��������  ע�⣺ ���1�� ʹ�� L5  M5   ���2��ʹ��A8  A9  ���3��ʹ�� C9 B9  ���4��A10 J1
//    Test_ENCoder();      //�������ɼ�
//    Test_Camera_Reprot();  //��λ����ͼ  7725RGB565 / 7725 �Ҷ� / ���ۻҶ�
//    Test_SGP18_OV7725();   //����OV7725RGB ��TFT1.8
    //Test_SGP18_Camera();   //�������� Or 7725 ��ֵ�� + TFT1.8  ע�⣬7725ʹ�ûҶ�ͼ��ʱʹ��YUYV��ʽ ��Ҫ���� cameraConfig = { .pixelFormat = kVIDEO_PixelFormatYUYV }
//----------------------------------------------------------------------------------------- 
    PRINTF("������ %d \r\n", time);
    time = MeasureRunTime_ms(DelayTest);

//    for(int i = 0; i < 50; i++)
//    {
//      databuff[i] = i;
//    }
    
//    const qtmr_config_t QuadTimer_1_Channel_0_config = {
//    .primarySource = kQTMR_ClockDivide_2,
//    .secondarySource = kQTMR_Counter0InputPin,
//    .enableMasterMode = false,
//    .enableExternalForce = false,
//    .faultFilterCount = 0,
//    .faultFilterPeriod = 0,
//    .debugMode = kQTMR_RunNormalInDebug
//    };
//    /*QTMR���벶׽*/
//    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */
//    CLOCK_EnableClock(kCLOCK_Xbar1);            /* xbar1 clock (xbar1_clk_enable): 0x03U */
//
//    IOMUXC_SetPinMux(
//        IOMUXC_GPIO_B0_14_XBAR1_INOUT12,        /* GPIO_B0_14 is configured as XBAR1_INOUT12 */
//        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//    IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
//        (~(IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12_MASK))) /* Mask bits to zero which are setting */
//        | IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL(0x01U) /* QTIMER2 TMR0 input select: input from XBAR */
//        | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_12(0x00U) /* IOMUXC XBAR_INOUT12 function direction select: XBAR_INOUT as input */
//        );
//    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout12, kXBARA1_OutputQtimer2Tmr0Input); /* IOMUX_XBAR_INOUT12 output assigned to XBARA1_IN12 input is connected to XBARA1_OUT90 output assigned to QTIMER2_TMR0_INPUT */
//
//    //QTMR_GetDefaultConfig(&qtmrcpatureconfig);
//    QTMR_Init(TMR2, kQTMR_Channel_0, &QuadTimer_1_Channel_0_config);
//    QTMR_SetupInputCapture(TMR2, kQTMR_Channel_0, kQTMR_Counter0InputPin, false, false, kQTMR_FallingEdge);
//    QTMR_EnableDma(TMR2, kQTMR_Channel_0, kQTMR_InputEdgeFlagDmaEnable);
//    //PWM_StartTimer(PWM1, 1u << kPWM_Module_0); //������ʱ��
//    /*eDMA��ʼ��*/
//    DMAMUX_Init(DMAMUX);
//    //DMAMUX_EnableAlwaysOn(DMAMUX, 0, true);
//    DMAMUX_SetSource(DMAMUX, 5, kDmaRequestMuxQTIMER2CaptTimer0);
//    DMAMUX_EnableChannel(DMAMUX, 5);
//    DMAMUX_SetSource(DMAMUX, 6, 10);
//    DMAMUX_EnableChannel(DMAMUX, 6);    
//    
//
//    EDMA_GetDefaultConfig(&userConfig);
//    //userConfig.enableContinuousLinkMode = false;
//    EDMA_Init(DMA0, &userConfig);
//    EDMA_CreateHandle(&g_EDMA_Handle, DMA0, 5);
//    EDMA_CreateHandle(&g_EDMA_Handle_5, DMA0, 6);
//    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
//    //EDMA_SetCallback(&g_EDMA_Handle_5, EDMA_Callback_5, NULL);
//        
//    /*eDMA����*/
//    EDMA_PrepareTransfer(&transferConfig, databuff, sizeof(databuff[0]), databuff2, sizeof(databuff[2])
//        , sizeof(databuff[0]), sizeof(databuff), kEDMA_MemoryToMemory);
//    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
//    //EDMA_SetChannelPreemptionConfig(DMA0, 6, &eDMA_1_DMA5_preemption);
//    EDMA_SetChannelLink(DMA0, 5, kEDMA_MinorLink, 6);
//    
//    EDMA_PrepareTransfer(&transferConfig, (uint32_t *)(0x401E0004), 2, &buff_tmr, 2
//                        , 2, 2, 20);
//    EDMA_SubmitTransfer(&g_EDMA_Handle_5, &transferConfig);
//    
//    DMA0->CR &= ~DMA_CR_EMLM(1);
//    DMA0->TCD[6].CSR &= ~DMA_CSR_INTMAJOR(1);
//    DMA0->TCD[6].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(100);
//    DMA0->TCD[6].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(100);
//    DMA0->TCD[6].SOFF = DMA_SOFF_SOFF(0);
//    DMA0->TCD[6].DOFF = DMA_DOFF_DOFF(0);
//    
//    EDMA_StartTransfer(&g_EDMA_Handle_5);
//    EDMA_StartTransfer(&g_EDMA_Handle);   
//    QTMR_StartTimer(TMR2, kQTMR_Channel_0, kQTMR_QuadCountMode);
//    /* Wait for EDMA transfer finish */
//    while (g_Transfer_Done != true)
//    {
//    }

    
    TFTSPI_Init();                 //TFT1.8��ʼ��  
    TFTSPI_CLS(u16BLUE);           //����
    LQ_PWM_Init(PWM2, kPWM_Module_0, kPWM_PwmA_B, 12000);//PWM�����Ƶ�� = 6250 000 / VAL1  = 96Hz     A8 A9
    LQ_PWM_Init(PWM2, kPWM_Module_1, kPWM_PwmA_B, 12000);//PWM�����Ƶ�� = 6250 000 / VAL1  = 96Hz     B9 C9
    //camera_init_1();
    _systime.delay_ms(200);
    //LQ_Camera_Init();
    uint16_t color = 0;
    LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_0, 2000, 0);
    LQ_PWMA_B_SetDuty(PWM2, kPWM_Module_1, 4000, 0);
    LQ_ENC_Init(ENC1);   //���������ʼ��

    while(1)
    {         
        _systime.delay_ms(100);
        velocity1 = (int16_t)ENC_GetPositionDifferenceValue(ENC1);  //�õ�������΢��ֵ

    }             
}
