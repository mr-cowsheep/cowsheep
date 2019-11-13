//# include "include.h"
//# include "PigMT9V032.h"
//
//# define UART_Put_Buff LQ_UART_PutBuff
//# define LPTMR_delay_ms(x) _systime.delay_ms(x)
//#define uint8 unsigned char
//#define uint16 unsigned int
//#define int16 int
//
//uint8   image_1[ROW][COL];      //ͼ������
//uint8   image_2[ROW][COL];      //ͼ������
//
//uint8   receive_1[3];
//uint8   receive_2[3];
//uint8   receive_num_1 = 0;
//uint8   receive_num_2 = 0;
//uint8   uart_receive_flag_1 = 1;
//uint8   uart_receive_flag_2 = 1;
//
//ImageDealState Image_1_DealState_Now = Image_Collecting;
//ImageDealState Image_2_DealState_Now = Image_Collecting;
//
////��Ҫ���õ�����ͷ������
//int16 MT9V032_CFG_1[CONFIG_FINISH][2] =
//{
//    { AUTO_EXP, 0 },   //�Զ��ع�����      ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
//    //һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
//    { EXP_TIME, 300 },//3118, //�ع�ʱ��          ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
//    { FPS, 50 },  //50ͼ��֡��          ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
//    { SET_COL, COL }, //ͼ��������        ��Χ1-752     K60�ɼ���������188
//    { SET_ROW, ROW }, //ͼ��������        ��Χ1-480
//    { LR_OFFSET, 0 },   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
//    { UD_OFFSET, 0 },   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
//    { GAIN, 32 },  //32 ͼ������          ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�
//
//
//    { INIT, 0 }    //����ͷ��ʼ��ʼ��
//};
////��Ҫ���õ�����ͷ������
//int16 MT9V032_CFG_2[CONFIG_FINISH][2] =
//{
//    { AUTO_EXP, 0 },   //�Զ��ع�����      ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
//    //һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
//    { EXP_TIME, 300 },//3118, //�ع�ʱ��          ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
//    { FPS, 50 },  //50ͼ��֡��          ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
//    { SET_COL, COL }, //ͼ��������        ��Χ1-752     K60�ɼ���������188
//    { SET_ROW, ROW }, //ͼ��������        ��Χ1-480
//    { LR_OFFSET, 0 },   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
//    { UD_OFFSET, 0 },   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
//    { GAIN, 32 },  //32 ͼ������          ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�
//
//
//    { INIT, 0 }    //����ͷ��ʼ��ʼ��
//};
////������ͷ�ڲ���ȡ������������
//int16 GET_CFG_1[CONFIG_FINISH - 1][2] =
//{
//    { AUTO_EXP, 0 },   //�Զ��ع�����      
//    { EXP_TIME, 0 },   //�ع�ʱ��          
//    { FPS, 0 },   //ͼ��֡��          
//    { SET_COL, 0 },   //ͼ��������        
//    { SET_ROW, 0 },   //ͼ��������        
//    { LR_OFFSET, 0 },   //ͼ������ƫ����    
//    { UD_OFFSET, 0 },   //ͼ������ƫ����    
//    { GAIN, 0 },   //ͼ������          
//};
////������ͷ�ڲ���ȡ������������
//int16 GET_CFG_2[CONFIG_FINISH - 1][2] =
//{
//    { AUTO_EXP, 0 },   //�Զ��ع�����      
//    { EXP_TIME, 0 },   //�ع�ʱ��          
//    { FPS, 0 },   //ͼ��֡��          
//    { SET_COL, 0 },   //ͼ��������        
//    { SET_ROW, 0 },   //ͼ��������        
//    { LR_OFFSET, 0 },   //ͼ������ƫ����    
//    { UD_OFFSET, 0 },   //ͼ������ƫ����    
//    { GAIN, 0 },   //ͼ������          
//};
//
////void LPUART5_IRQHandler(void)
////{
////    static uint8_t res = 0;
////
////    if ((LPUART5->STAT)&kLPUART_RxDataRegFullFlag) //�����ж�
////    {
////        receive_1[receive_num_1] = LPUART_ReadByte(MT9V032_COF_UART_1);				//��ȡ����
////        receive_num_1++;
////
////        if (1 == receive_num_1 && 0XA5 != receive_1[0])  receive_num_1 = 0;
////        if (3 == receive_num_1)
////        {
////            receive_num_1 = 0;
////            uart_receive_flag_1 = 1;
////        }
////    }
////    __DSB();				//����ͬ������ָ��
////}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      ��ȡ����ͷ�ڲ�������Ϣ
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:				���øú���ǰ���ȳ�ʼ��uart3
////-------------------------------------------------------------------------------------------------------------------
//void get_config_1(void)
//{
//    uint16 temp, i;
//    uint8  send_buffer[4];
//
//    for (i = 0; i<CONFIG_FINISH - 1; i++)
//    {
//        send_buffer[0] = 0xA5;
//        send_buffer[1] = GET_STATUS;
//        temp = GET_CFG_1[i][0];
//        send_buffer[2] = temp >> 8;
//        send_buffer[3] = (uint8)temp;
//
//        LQ_UART_PutBuff(MT9V032_COF_UART_1, send_buffer, 4);
//
//        //�ȴ����ܻش�����
//        while (!uart_receive_flag_1);
//        uart_receive_flag_1 = 0;
//
//        GET_CFG_1[i][1] = receive_1[1] << 8 | receive_1[2];
//
//    }
//}
//void get_config_2(void)
//{
//    uint16 temp, i;
//    uint8  send_buffer[4];
//
//    for (i = 0; i<CONFIG_FINISH - 1; i++)
//    {
//        send_buffer[0] = 0xA5;
//        send_buffer[1] = GET_STATUS;
//        temp = GET_CFG_2[i][0];
//        send_buffer[2] = temp >> 8;
//        send_buffer[3] = (uint8)temp;
//
//        LQ_UART_PutBuff(MT9V032_COF_UART_2, send_buffer, 4);
//
//        //�ȴ����ܻش�����
//        while (!uart_receive_flag_2);
//        uart_receive_flag_2 = 0;
//
//        GET_CFG_2[i][1] = receive_2[1] << 8 | receive_2[2];
//
//    }
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      ��ȡ����ͷ�̼��汾
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:				���øú���ǰ���ȳ�ʼ��uart3
////-------------------------------------------------------------------------------------------------------------------
//uint16 get_version(void)
//{
//    uint16 temp;
//    uint8  send_buffer[4];
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = GET_STATUS;
//    temp = GET_VERSION;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    LQ_UART_PutBuff(MT9V032_COF_UART_1, send_buffer, 4);
//
//    //�ȴ����ܻش�����
//    while (!uart_receive_flag_1);
//    uart_receive_flag_1 = 0;
//
//    return ((uint16)(receive_1[1] << 8) | receive_1[2]);
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      ������������ͷ�ع�ʱ��
////  @param      light   �����ع�ʱ��Խ��ͼ��Խ��������ͷ�յ������ݷֱ��ʼ�FPS��������ع�ʱ��������õ����ݹ�����ô����ͷ��������������ֵ
////  @return     uint16  ��ǰ�ع�ֵ������ȷ���Ƿ���ȷд��
////  @since      v1.0
////  Sample usage:				
////-------------------------------------------------------------------------------------------------------------------
//uint16 set_exposure_time(uint16 light)
//{
//    uint16 temp;
//    uint8  send_buffer[4];
//
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = SET_EXP_TIME;
//    temp = light;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    LQ_UART_PutBuff(MT9V032_COF_UART_1, send_buffer, 4);
//
//    //�ȴ����ܻش�����
//    while (!uart_receive_flag_1);
//    uart_receive_flag_1 = 0;
//
//    temp = receive_1[1] << 8 | receive_1[2];
//    return temp;
//
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      ������ͷ�ڲ��Ĵ�������д����
////  @param      addr    ����ͷ�ڲ��Ĵ�����ַ
////  @param      data    ��Ҫд�������
////  @return     uint16  �Ĵ�����ǰ���ݣ�����ȷ���Ƿ�д��ɹ�
////  @since      v1.0
////  Sample usage:				
////-------------------------------------------------------------------------------------------------------------------
//uint16 set_mt9v032_reg_1(uint8 addr, uint16 data)
//{
//    uint16 temp;
//    uint8  send_buffer[4];
//
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = SET_ADDR;
//    temp = addr;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    LQ_UART_PutBuff(MT9V032_COF_UART_1, send_buffer, 4);
//
//    _systime.delay_ms(10);
//
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = SET_DATA;
//    temp = data;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    LQ_UART_PutBuff(MT9V032_COF_UART_1, send_buffer, 4);
//
//    UART_Put_Buff(MT9V032_COF_UART_1, send_buffer, 4);
//
//    //�ȴ����ܻش�����
//    while (!uart_receive_flag_1);
//    uart_receive_flag_1 = 0;
//
//    temp = receive_1[1] << 8 | receive_1[2];
//    return temp;
//
//}
//uint16 set_mt9v032_reg_2(uint8 addr, uint16 data)
//{
//    uint16 temp;
//    uint8  send_buffer[4];
//
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = SET_ADDR;
//    temp = addr;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    UART_Put_Buff(MT9V032_COF_UART_2, send_buffer, 4);
//    LPTMR_delay_ms(10);
//
//    send_buffer[0] = 0xA5;
//    send_buffer[1] = SET_DATA;
//    temp = data;
//    send_buffer[2] = temp >> 8;
//    send_buffer[3] = (uint8)temp;
//
//    UART_Put_Buff(MT9V032_COF_UART_2, send_buffer, 4);
//
//    //�ȴ����ܻش�����
//    while (!uart_receive_flag_2);
//    uart_receive_flag_2 = 0;
//
//    temp = receive_2[1] << 8 | receive_2[2];
//    return temp;
//
//}
//
//void CameraPinInit()
//{
//  CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */
//  CLOCK_EnableClock(kCLOCK_Xbar1);            /* xbar1 clock (xbar1_clk_enable): 0x03U */
//
//  /* GPIO configuration on GPIO_B0_00 (pin D7) */
//  gpio_pin_config_t gpio2_pinD7_config = {
//      .direction = kGPIO_DigitalInput,
//      .outputLogic = 0U,
//      .interruptMode = kGPIO_NoIntmode
//  };
//  /* Initialize GPIO functionality on GPIO_B0_00 (pin D7) */
//  GPIO_PinInit(GPIO2, 0U, &gpio2_pinD7_config);
//
//  /* GPIO configuration on GPIO_B0_01 (pin E7) */
//  gpio_pin_config_t gpio2_pinE7_config = {
//      .direction = kGPIO_DigitalInput,
//      .outputLogic = 0U,
//      .interruptMode = kGPIO_NoIntmode
//  };
//  /* Initialize GPIO functionality on GPIO_B0_01 (pin E7) */
//  GPIO_PinInit(GPIO2, 1U, &gpio2_pinE7_config);
//
//  /* GPIO configuration on GPIO_B0_02 (pin E8) */
//  gpio_pin_config_t gpio2_pinE8_config = {
//      .direction = kGPIO_DigitalInput,
//      .outputLogic = 0U,
//      .interruptMode = kGPIO_NoIntmode
//  };
//  /* Initialize GPIO functionality on GPIO_B0_02 (pin E8) */
//  GPIO_PinInit(GPIO2, 2U, &gpio2_pinE8_config);
//
//  /* GPIO configuration on GPIO_B0_03 (pin D8) */
//  gpio_pin_config_t gpio2_pinD8_config = {
//      .direction = kGPIO_DigitalInput,
//      .outputLogic = 0U,
//      .interruptMode = kGPIO_NoIntmode
//  };
//  /* Initialize GPIO functionality on GPIO_B0_03 (pin D8) */
//  GPIO_PinInit(GPIO2, 3U, &gpio2_pinD8_config);
//
//  /* GPIO configuration on GPIO_B0_04 (pin C8) */
//  gpio_pin_config_t gpio2_pinC8_config = {
//      .direction = kGPIO_DigitalInput,
//      .outputLogic = 0U,
//      .interruptMode = kGPIO_NoIntmode
//  };
//  /* Initialize GPIO functionality on GPIO_B0_04 (pin C8) */
//  GPIO_PinInit(GPIO2, 4U, &gpio2_pinC8_config);
//
//  /* GPIO configuration on GPIO_B0_05 (pin B8) */
//  gpio_pin_config_t gpio2_pinB8_config = {
//      .direction = kGPIO_DigitalInput,
//      .outputLogic = 0U,
//      .interruptMode = kGPIO_NoIntmode
//  };
//  /* Initialize GPIO functionality on GPIO_B0_05 (pin B8) */
//  GPIO_PinInit(GPIO2, 5U, &gpio2_pinB8_config);
//
//  /* GPIO configuration on GPIO_B0_06 (pin A8) */
//  gpio_pin_config_t gpio2_pinA8_config = {
//      .direction = kGPIO_DigitalInput,
//      .outputLogic = 0U,
//      .interruptMode = kGPIO_NoIntmode
//  };
//  /* Initialize GPIO functionality on GPIO_B0_06 (pin A8) */
//  GPIO_PinInit(GPIO2, 6U, &gpio2_pinA8_config);
//
//  /* GPIO configuration on GPIO_B0_07 (pin A9) */
//  gpio_pin_config_t gpio2_pinA9_config = {
//      .direction = kGPIO_DigitalInput,
//      .outputLogic = 0U,
//      .interruptMode = kGPIO_NoIntmode
//  };
//  /* Initialize GPIO functionality on GPIO_B0_07 (pin A9) */
//  GPIO_PinInit(GPIO2, 7U, &gpio2_pinA9_config);
//
//  /* GPIO configuration on GPIO_EMC_41 (pin C7) */
//  gpio_pin_config_t gpio3_pinC7_config = {
//      .direction = kGPIO_DigitalInput,
//      .outputLogic = 0U,
//      .interruptMode = kGPIO_NoIntmode
//  };
//  /* Initialize GPIO functionality on GPIO_EMC_41 (pin C7) */
//  GPIO_PinInit(GPIO3, 27U, &gpio3_pinC7_config);
//
//  IOMUXC_SetPinMux(
//      IOMUXC_GPIO_B0_00_GPIO2_IO00,           /* GPIO_B0_00 is configured as GPIO2_IO00 */
//      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_SetPinMux(
//      IOMUXC_GPIO_B0_01_GPIO2_IO01,           /* GPIO_B0_01 is configured as GPIO2_IO01 */
//      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_SetPinMux(
//      IOMUXC_GPIO_B0_02_GPIO2_IO02,           /* GPIO_B0_02 is configured as GPIO2_IO02 */
//      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_SetPinMux(
//      IOMUXC_GPIO_B0_03_GPIO2_IO03,           /* GPIO_B0_03 is configured as GPIO2_IO03 */
//      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_SetPinMux(
//      IOMUXC_GPIO_B0_04_GPIO2_IO04,           /* GPIO_B0_04 is configured as GPIO2_IO04 */
//      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_SetPinMux(
//      IOMUXC_GPIO_B0_05_GPIO2_IO05,           /* GPIO_B0_05 is configured as GPIO2_IO05 */
//      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_SetPinMux(
//      IOMUXC_GPIO_B0_06_GPIO2_IO06,           /* GPIO_B0_06 is configured as GPIO2_IO06 */
//      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_SetPinMux(
//      IOMUXC_GPIO_B0_07_GPIO2_IO07,           /* GPIO_B0_07 is configured as GPIO2_IO07 */
//      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_SetPinMux(
//      IOMUXC_GPIO_EMC_41_GPIO3_IO27,          /* GPIO_EMC_41 is configured as GPIO3_IO27 */
//      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_SetPinMux(
//      IOMUXC_GPIO_SD_B0_03_XBAR1_INOUT07,     /* GPIO_SD_B0_03 is configured as XBAR1_INOUT07 */
//      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
//    (~(IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL_MASK | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7_MASK))) /* Mask bits to zero which are setting */
//      | IOMUXC_GPR_GPR6_QTIMER2_TRM0_INPUT_SEL(0x01U) /* QTIMER2 TMR0 input select: input from XBAR */
//      | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_7(0x00U) /* IOMUXC XBAR_INOUT7 function direction select: XBAR_INOUT as input */
//    );
//  XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarInout07, kXBARA1_OutputQtimer2Tmr0Input); /* IOMUX_XBAR_INOUT07 output assigned to XBARA1_IN7 input is connected to XBARA1_OUT90 output assigned to QTIMER2_TMR0_INPUT */
//
//}
//
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
//uint16_t buff_tmr = 0;
//uint8_t buff_num = 0;
//uint32_t TestLiveWatch = 0;
////-------------------------------------------------------------------------------------------------------------------
////  @brief      MT9V032����ͷ��ʼ��
////  @param      NULL
////  @return     void					
////  @since      v1.0
////  Sample usage:		
////-------------------------------------------------------------------------------------------------------------------
//void camera_init_1(void)
//{
//    //���ò���    ������ο�ʹ���ֲ�
//    uint16 temp, i;
//    uint8  send_buffer[4];
//
////    LQ_UART_Init(MT9V032_COF_UART_1, 9600);	//��ʼ������ ��������ͷ    
////
////    //�ȴ�����ͷ�ϵ��ʼ���ɹ�
////    LPTMR_delay_ms(1000);
////    uart_receive_flag_1 = 0;
////
////    //��ʼ��������ͷ�����³�ʼ��
////    for (i = 0; i<CONFIG_FINISH; i++)
////    {
////        send_buffer[0] = 0xA5;
////        send_buffer[1] = MT9V032_CFG_1[i][0];
////        temp = MT9V032_CFG_1[i][1];
////        send_buffer[2] = temp >> 8;
////        send_buffer[3] = (uint8)temp;
////        UART_Put_Buff(MT9V032_COF_UART_1, send_buffer, 4);
////        LPTMR_delay_ms(2);
////    }
////    ////�ȴ�����ͷ��ʼ���ɹ�
////    while (!uart_receive_flag_1);
////    uart_receive_flag_1 = 0;
////    while ((0xff != receive_1[1]) || (0xff != receive_1[2]));
//    //���ϲ��ֶ�����ͷ���õ�����ȫ�����ᱣ��������ͷ��51��Ƭ����eeprom��
//    //����set_exposure_time�����������õ��ع����ݲ��洢��eeprom��
//
//    //set_mt9v032_reg_1(0x05, 113);  //FPS΢��
//
//    //��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
////    get_config_1();
//
//    //����ͷ�ɼ���ʼ��
//    /*�ܽų�ʼ��*/
//    CameraPinInit();        
//
//
//    /*���жϳ�ʼ��*/
////    gpio_pin_config_t GPIO_Input_Config = { kGPIO_DigitalInput,    //GPIOΪ���뷽��
////        1,                    //�ߵ�ƽ
////        kGPIO_NoIntmode,      //�������ж�
////    };
//    //GPIO_PinInit(GPIO1, 9, &GPIO_Input_Config);
//    //GPIO_PinSetInterruptConfig(GPIO1, 9, kGPIO_IntFallingEdge);
//    //EnableIRQ(GPIO1_Combined_0_15_IRQn);
//
//    /*dMA*/
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
//    TestLiveWatch = ROW*COL;        
//    /*eDMA����*/
//    EDMA_PrepareTransfer(&transferConfig, (uint8_t *)(&PT2_BYTE0), 1, image_1, 1
//        , 1, 259, kEDMA_PeripheralToMemory);
//    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
//    //EDMA_SetChannelPreemptionConfig(DMA0, 6, &eDMA_1_DMA5_preemption);
//    EDMA_SetChannelLink(DMA0, 5, kEDMA_MinorLink, 6);
//        
//    EDMA_PrepareTransfer(&transferConfig, (uint32_t *)(0x401E0004), 2, &buff_tmr, 2
//                        , 2, 2, ROW*COL*2);
//    EDMA_SubmitTransfer(&g_EDMA_Handle_5, &transferConfig);
//        
//    DMA0->CR &= ~DMA_CR_EMLM(1);
//    DMA0->TCD[6].CSR &= ~DMA_CSR_INTMAJOR(1);
//    DMA0->TCD[6].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(100);//100
//    DMA0->TCD[6].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(100);//100
//    DMA0->TCD[6].SOFF = DMA_SOFF_SOFF(0);
//    DMA0->TCD[6].DOFF = DMA_DOFF_DOFF(0);
//    
//    EDMA_StartTransfer(&g_EDMA_Handle_5);
//    EDMA_StartTransfer(&g_EDMA_Handle);   
//    QTMR_StartTimer(TMR2, kQTMR_Channel_0, kQTMR_PriSrcRiseEdge);
//    /* Wait for EDMA transfer finish */
//    while (g_Transfer_Done != true)
//    {
//      buff_num = *(uint32_t *)(PT2_BYTE0);
//    }
//
//    while (1)
//    {
//      g_Transfer_Done = false;
//        /*eDMA����*/
//      EDMA_PrepareTransfer(&transferConfig, (uint8_t *)(&PT2_BYTE0), 1, image_1, 1
//          , 1, ROW*COL, kEDMA_PeripheralToMemory);
//      EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
//      //EDMA_SetChannelPreemptionConfig(DMA0, 6, &eDMA_1_DMA5_preemption);
//      EDMA_SetChannelLink(DMA0, 5, kEDMA_MinorLink, 6);
//          
//      EDMA_PrepareTransfer(&transferConfig, (uint32_t *)(0x401E0004), 2, &buff_tmr, 2
//                          , 2, 2, 20);
//      EDMA_SubmitTransfer(&g_EDMA_Handle_5, &transferConfig);
//          
//      DMA0->CR &= ~DMA_CR_EMLM(1);
//      DMA0->TCD[6].CSR &= ~DMA_CSR_INTMAJOR(1);
//      DMA0->TCD[6].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(100);
//      DMA0->TCD[6].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(100);
//      DMA0->TCD[6].SOFF = DMA_SOFF_SOFF(0);
//      DMA0->TCD[6].DOFF = DMA_DOFF_DOFF(0);
//      
//      DMA0->TCD[6].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(ROW*COL);
//      DMA0->TCD[6].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(ROW*COL);
//
//      EDMA_StartTransfer(&g_EDMA_Handle_5);
//      EDMA_StartTransfer(&g_EDMA_Handle); 
//        /* Wait for EDMA transfer finish */
//        while (g_Transfer_Done != true)
//        {
//            buff_num = *(uint32_t *)(PT2_BYTE0);
//        }
//    }
//}
////-------------------------------------------------------------------------------------------------------------------
////  @brief      �ڶ���MT9V032����ͷ��ʼ��
////  @param      NULL
////  @return     void					
////  @since      v1.0
////  Sample usage:		
////-------------------------------------------------------------------------------------------------------------------
//void camera_init_2(void)
//{
//    //���ò���    ������ο�ʹ���ֲ�
//    uint16 temp, i;
//    uint8  send_buffer[4];
//
//
//    LQ_UART_Init(MT9V032_COF_UART_2, 9600);	//��ʼ������ ��������ͷ    
//    //UART_Irq_En(MT9V032_COF_UART_2);
//
//
//    //�ȴ�����ͷ�ϵ��ʼ���ɹ�
//    LPTMR_delay_ms(1000);
//    uart_receive_flag_2 = 0;
//
//    //��ʼ��������ͷ�����³�ʼ��
//    for (i = 0; i<CONFIG_FINISH; i++)
//    {
//        send_buffer[0] = 0xA5;
//        send_buffer[1] = MT9V032_CFG_2[i][0];
//        temp = MT9V032_CFG_2[i][1];
//        send_buffer[2] = temp >> 8;
//        send_buffer[3] = (uint8)temp;
//        UART_Put_Buff(MT9V032_COF_UART_2, send_buffer, 4);
//        LPTMR_delay_ms(2);
//    }
//    //�ȴ�����ͷ��ʼ���ɹ�
//    while (!uart_receive_flag_2);
//    uart_receive_flag_2 = 0;
//    while ((0xff != receive_2[1]) || (0xff != receive_2[2]));
//    //���ϲ��ֶ�����ͷ���õ�����ȫ�����ᱣ��������ͷ��51��Ƭ����eeprom��
//    //����set_exposure_time�����������õ��ع����ݲ��洢��eeprom��
//
//    //set_mt9v032_reg_2(0x05, 113);  //FPS΢��
//
//    //��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
//    get_config_2();
//
//    //����ͷ�ɼ���ʼ��
//
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      MT9V032����ͷ���ж�
////  @param      NULL
////  @return     void			
////  @since      v1.0
////  Sample usage:				��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
////-------------------------------------------------------------------------------------------------------------------
////void VSYNC_1(void)
////{
////    //DMA_PORTx2BUFF_Init(DMA_CH4, (void *)&MT9V032_DATAPORT, (void *)image, MT9V032_PCLK, DMA_BYTE1, COL*ROW, DMA_rising_down_keepon);
////    DMA_DIS(DMA_CH4);
////    DMA_IRQ_CLEAN(DMA_CH4);
////    DMA0->TCD[DMA_CH4].BITER_ELINKNO |= DMA_BITER_ELINKNO_BITER(COL * ROW);
////    DMA0->TCD[DMA_CH4].CITER_ELINKNO |= DMA_CITER_ELINKNO_CITER(COL * ROW);
////    DMA0->TCD[DMA_CH4].DADDR = (uint32)image_1;
////
////    if (Image_1_DealState_Now == Image_Collecting)
////    {
////        Image_1_DealState_Now = Image_CollectFinish;
////    }
////
////    if (Image_1_DealState_Now == Image_DealingFinsh)
////    {
////        Image_1_DealState_Now = Image_Collecting;
////        DMA_IRQ_EN(DMA_CH4);
////        DMA_EN(DMA_CH4);
////    }
////
////}
////
////void VSYNC_2(void)
////{
////    //DMA_PORTx2BUFF_Init(DMA_CH4, (void *)&MT9V032_DATAPORT, (void *)image, MT9V032_PCLK, DMA_BYTE1, COL*ROW, DMA_rising_down_keepon);
////    DMA_DIS(DMA_CH5);
////    DMA_IRQ_CLEAN(DMA_CH5);
////    DMA0->TCD[DMA_CH5].BITER_ELINKNO |= DMA_BITER_ELINKNO_BITER(COL * ROW);
////    DMA0->TCD[DMA_CH5].CITER_ELINKNO |= DMA_CITER_ELINKNO_CITER(COL * ROW);
////    DMA0->TCD[DMA_CH5].DADDR = (uint32)image_2;
////
////    if (Image_2_DealState_Now == Image_Collecting)
////    {
////        Image_2_DealState_Now = Image_CollectFinish;
////    }
////
////    if (Image_2_DealState_Now == Image_DealingFinsh)
////    {
////        Image_2_DealState_Now = Image_Collecting;
////        DMA_IRQ_EN(DMA_CH5);
////        DMA_EN(DMA_CH5);
////    }
////
////}
////-------------------------------------------------------------------------------------------------------------------
////  @brief      ���������ͷͼ��������λ���鿴ͼ��
////  @param      NULL
////  @return     void			
////  @since      v1.0
////  Sample usage:				���øú���ǰ���ȳ�ʼ��uart2
////-------------------------------------------------------------------------------------------------------------------
//extern uint32_t fullCameraBufferAddr;
//void seekfree_sendimg_032(void)
//{
//    LQ_UART_PutChar(LPUART1, 0x00);
//    LQ_UART_PutChar(LPUART1, 0xff);
//    LQ_UART_PutChar(LPUART1, 0x01);
//    LQ_UART_PutChar(LPUART1, 0x01);
//    LQ_UART_PutBuff(LPUART1, (uint8_t *)fullCameraBufferAddr, ROW*COL);
//
//}
