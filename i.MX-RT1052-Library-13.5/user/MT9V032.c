/*!
* @file       MT9V032.c
* @brief      ���ݹٷ����������ͷ���д��������������ͷ����������
* @details    
* @author     pig's grief
* @version    v1.0
* @date       2019-4-22
* @to do      
*/
# include "include.h"

AT_NONCACHEABLE_SECTION_ALIGN(uint16_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH], FRAME_BUFFER_ALIGN); //��������ͷ���ݻ�����

typedef struct _MT9V032_resource
{
    sccb_i2c_t sccbI2C;                     //!< I2C for SCCB transfer. 
    void(*pullResetPin)(bool pullUp);     /*!< Function to pull reset pin high or low. */
    void(*pullPowerDownPin)(bool pullUp); /*!< Function to pull the power down pin high or low. */
    uint32_t inputClockFreq_Hz;             //!< Input clock frequency. 
} MT9V032_resource_t;

void MT9V032PinInit()
{
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_04_CSI_PIXCLK,        /* GPIO_AD_B1_04 is configured as CSI_PIXCLK */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_06_CSI_VSYNC,         /* GPIO_AD_B1_06 is configured as CSI_VSYNC */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_07_CSI_HSYNC,         /* GPIO_AD_B1_07 is configured as CSI_HSYNC */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_08_CSI_DATA09,        /* GPIO_AD_B1_08 is configured as CSI_DATA09 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_09_CSI_DATA08,        /* GPIO_AD_B1_09 is configured as CSI_DATA08 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_10_CSI_DATA07,        /* GPIO_AD_B1_10 is configured as CSI_DATA07 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_11_CSI_DATA06,        /* GPIO_AD_B1_11 is configured as CSI_DATA06 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_12_CSI_DATA05,        /* GPIO_AD_B1_12 is configured as CSI_DATA05 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_13_CSI_DATA04,        /* GPIO_AD_B1_13 is configured as CSI_DATA04 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_14_CSI_DATA03,        /* GPIO_AD_B1_14 is configured as CSI_DATA03 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_AD_B1_15_CSI_DATA02,        /* GPIO_AD_B1_15 is configured as CSI_DATA02 */
        0U);                                    /* Software Input On Field: Input Path is determined by functionality */
}

static csi_resource_t csiResource = {   //ָ��csi����ַ
    .csiBase = CSI,
};




static csi_private_data_t csiPrivateData;  //csi˽������

camera_receiver_handle_t cameraReceiver = {//����ͷ�������ýṹ��
    .resource = &csiResource,
    .ops = &csi_ops,
    .privateData = &csiPrivateData,
};


// ����������豸�ͽ�������
camera_config_t cameraConfig = {
    .pixelFormat = kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatYUYV,//kVIDEO_PixelFormatBGR565,
    .bytesPerPixel = APP_BPP,//   ÿ�����ص㼸������
    .resolution = FSL_VIDEO_RESOLUTION(APP_CAMERA_WIDTH, APP_CAMERA_HEIGHT), //�ֱ���
    .frameBufferLinePitch_Bytes = APP_CAMERA_WIDTH * APP_BPP,                //�м��
    .interface = kCAMERA_InterfaceGatedClock,                            //������ӿ�����
    .controlFlags = APP_CAMERA_CONTROL_FLAGS,
    .framePerSec = 50,                                                     //fps �޸���Ҫ�޸��ع�ʱ�� �� �ֱ��� ���
};

static MT9V032_resource_t MT9V032Resource = {   //����ͷ��ʼ���ṹ��
    .sccbI2C = LPI2C1,

    .inputClockFreq_Hz = 27000000,
};

status_t MT9V032_Init(camera_device_handle_t *handle, const camera_config_t *config)
{
    return kStatus_Success;
}

status_t MT9V032_Deinit(camera_device_handle_t *handle)
{
    ((MT9V032_resource_t *)(handle->resource))->pullPowerDownPin(true);
    return kStatus_Success;
}

status_t MT9V032_Control(camera_device_handle_t *handle, camera_device_cmd_t cmd, int32_t arg)
{
    return kStatus_InvalidArgument;
}

status_t MT9V032_Start(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t MT9V032_Stop(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t MT9V032_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
    return MT9V032_Init(handle, config);
}

const camera_device_operations_t MT9V032_ops = {
    .init = MT9V032_Init,
    .deinit = MT9V032_Deinit,
    .start = MT9V032_Start,
    .stop = MT9V032_Stop,
    .control = MT9V032_Control,
    .init_ext = MT9V032_InitExt,
};

camera_device_handle_t cameraDevice = {           //����ͷ�������ýṹ��
    .resource = &MT9V032Resource,
    .ops = &MT9V032_ops,
};




void MT9V032Init()
{
    /*���ų�ʼ��*/
    MT9V032PinInit();
    memset(csiFrameBuf, 0, sizeof(csiFrameBuf));
    /*CSIģ���ʼ��*/
    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);
    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);
    CAMERA_DEVICE_Start(&cameraDevice);

    for (uint32_t i = 0; i < APP_CAMERA_FRAME_BUFFER_COUNT; i++) //����֡�������ύ������������
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(csiFrameBuf[i]));
    }

    CAMERA_RECEIVER_Start(&cameraReceiver);   // ��������camera����

    //delayms(200);        //��ʱ200����  ����ͷ���������ϵ� ���Բ�Ҫ��ʱ
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {//ע�⣬ʹ��csiFrameBuf����ʱ����ùر�D-Cache ��Ȼ�ϴ����ݿ��ܻ�����cache���棬������ݴ���
        SCB_DisableDCache();
    }
}