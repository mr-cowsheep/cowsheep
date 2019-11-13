/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��V2.3.1�汾�⽨�Ĺ���ģ��
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
# include "include.h"

/*******************************************************************
 * Prototypes
 *******************************************************************/


/*******************************************************************
 * Code
 *******************************************************************/
extern void delayms(uint32_t ms);
void PIT0Inter();
 long count_1s = 0;

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
 int main(void)
{
    uint32_t fullCameraBufferAddr;   
    uint16_t color = 0;

    /* ��ʼ���ڴ汣����Ԫ */
    BOARD_ConfigMPU();
    /* ��ʼ�������趨 */
    BOARD_InitPins();
    /* ��ʼ��ʱ���趨 */
    BOARD_BootClockRUN();
    /* ��ʼ���������� */    
    BOARD_InitBootPeripherals();
    /* ��ʼ�����Դ��� */
    BOARD_InitDebugConsole();
    /* ��ӡϵͳʱ�� */
    PRINTF("\r\n");
    PRINTF("*****��ӭʹ�� Ұ��i.MX RT1052 ������*****\r\n");
    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
    PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
    PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
    PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
    PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));  
    /* �����������Ĵ���^_^. */
  
    TFTSPI_Init();
//    UART_Init(LPUART2, 256000);
//    UART_Send(LPUART2, "123123\r\n", 8);
//    UART_Send(LPUART2, "123123\r\n", 8);
//    UART_Send(LPUART2, "123123\r\n", 8);
    //LQ_PIT_Init(kPIT_Chnl_1, 1000000);
    LQ_Camera_Init();
    TFTSPI_CLS(u16RED);
    while(1)
    {
 
            while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //����ͷCSI����������
            {
            }   
            if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {//ע�⣬ʹ��csiFrameBuf����ʱ����ùر�D-Cache ��Ȼ�ϴ����ݿ��ܻ�����cache���棬������ݴ���
            SCB_DisableDCache();
            }
    
            TFTSPI_Set_Pos(0,100,(uint8_t)(APP_CAMERA_WIDTH/2-1) ,APP_CAMERA_HEIGHT);//ע�� ������ʾ��СҪ�������ʵ����ʾ��С��ȣ���Ȼ����ʾ���������߻���
            for(int i = 0; i < APP_CAMERA_HEIGHT; i+=2)  //  480/4/2/2/2 = 15    //30
            {
                for(int j = 0; j < APP_CAMERA_WIDTH*2; j+=2)//��2��ȡһ��  752*2/4/4 = 188   //�������� һ��47����
                {
                    //�Ҷ���ʾ
                    color = 0;
                    color=(((*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j))>>3))<<11;
                    color=color|((((*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j))>>2))<<5);
                    color=color|(((*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j))>>3));
                    TFTSPI_Write_Word(color);
                    //��ֵ����ʾ
                    //                if(*((uint8_t *)fullCameraBufferAddr +  i * APP_CAMERA_WIDTH * 2 + j) > 0x60)  //��ֵ0x60 ��ֵ����ʾ
                    //                  TFTSPI_Write_Word (0xffff); //��ʾ����
                    //                else
                    //                  TFTSPI_Write_Word (0x0000); //��ʾ����
                }
            }
            CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//��������������ύ���������        
            SCB_EnableDCache();

      
      //if (kStatus_Success == CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr))  //����ͷCSI����������
      //{
        
        
        
         //CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);//��������������ύ���������  
      //}  
    }     

}

 void PIT0Inter()
 {
     count_1s++;
     if (count_1s >= 999)
         count_1s = 0;
 }
/****************************END OF FILE**********************/
