/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1052���İ�-���ܳ���
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2018��2��1��
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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_INLUDE_FILES_H_
#define _LQ_INLUDE_FILES_H_
    #include "stdio.h"
    #include "math.h"
//SDK�ײ�������
    #include "fsl_adc.h"
    #include "fsl_cache.h"
    #include "fsl_common.h"
    #include "fsl_clock.h"
    #include "fsl_debug_console.h"
    #include "fsl_elcdif.h"
    #include "fsl_enc.h"    
    #include "fsl_gpio.h"
    #include "fsl_gpt.h"
    #include "fsl_iomuxc.h"
    #include "fsl_lpuart.h"
    #include "fsl_lpi2c.h"
    #include "fsl_pwm.h"
    #include "fsl_pit.h"
    #include "fsl_qtmr.h"
    #include "fsl_trng.h"
    #include "fsl_usdhc.h"
    #include "fsl_wdog.h"
    #include "fsl_xbara.h"
    #include "board.h"
    #include "pin_mux.h"
    #include "fsl_flexspi.h"
    #include "fsl_lpuart_edma.h"
    #include "fsl_edma.h"
    #include "fsl_dmamux.h"
    #include "fsl_lpspi.h"
    #include "fsl_sccb.h"

#define delayms(x) _systime.delay_ms(x)
#define delayus(x) _systime.delay_us(x)
#define uint16 uint16_t
#define int16 int16_t
#define uint8 uint8_t
#define int8 int8_t


//�û��Զ����ļ�    
    #include "LQ_IIC.h"
    #include "LQ_GPIOmacro.h" //GPIO��51������ʽ 
    #include "LQ_SGP18T.h"    //TFT1.8������ģ��
    #include "LQ_LED.h"       //LEDָʾ
    #include "LQ_KEY.h"       //��������
    #include "LQ_PWM.h"       //��������PWM���� XBARA
    #include "LQ_UART.h"       //UART����
    #include "LQ_ADC.h"       //ADCת���ɼ�
    #include "LQ_PIT.h"       //PIT��ʱ
    #include "LQ_TRNG.h"      //�����������
    #include "LQ_Encoder.h"   //�����������������ݲɼ�
    #include "m_systime.h"    //systick ��ʱ�������ڼ�ʱ
    #include "status.h"       //��־λ
    #include "LQ_QTMR.h"      //��ʱ������������PWM  �� ��������
    #include "LQ_SPI.h"

    #include "Pig_edma.h"
    #include "PigMT9V032.h"
    #include "DebugFunction.h"
    #include "MecanumWheelControl.h" 

    #include "Pig_SPI.h"
    #include "TFTDriver.h"
    #include "LQ_MPU6050.h"
    #include "AvoidObstacle.h"
//    void LQ_Get_CameraBuff(void);//�õ�����ͷ���� 
//    void LQ_Camera_0_1_Handle(void);  //��ֵ������ʾ
//    void Seek_Road(void);
//    void Test_TFT_Camera(void);    //����ͷ+��������

    extern void Test_OLED_Camera(void);//����ͷ+oled����
#endif //_LQ_INLUDE_FILES_H_
