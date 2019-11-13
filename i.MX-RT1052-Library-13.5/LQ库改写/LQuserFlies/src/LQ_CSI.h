#ifndef __LQ_CSI_H
#define __LQ_CSI_H


#define LQMT9V034    //ѡ��ʹ����������ͷģ��

#define APP_CAMERA_WIDTH  (188)
#define APP_CAMERA_HEIGHT (120/2)  //һ��uint16_t����װ���������أ� һ��IMAGEW ������ʵװ�����е����ص㣬���Ը߶�/2
#define APP_BPP 2 //���ظ�ʽ��

#define APP_CAMERA_FRAME_BUF  FER_COUNT 4 
#define FRAME_BUFFER_ALIGN 64  //���� 64
/*ʹ��TFT1.8����ʱ*/
#define  Use_ROWS  160    //ʹ��ͼ��߶�
#define  Use_Line  128   //ʹ��ͼ����

#define APP_CAMERA_CONTROL_FLAGS  (kCAMERA_VsyncActiveLow| kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)//(kCAMERA_DataLatchOnRisingEdge | kCAMERA_VsyncActiveLow) //ʹ������ͷ��������
extern void LQ_Camera_Init(void);
extern camera_device_handle_t cameraDevice;    
extern camera_receiver_handle_t cameraReceiver;
//extern uint16_t csiFrameBuf[APP_CAMERA_FRAME_BUFFER_COUNT][APP_CAMERA_HEIGHT][APP_CAMERA_WIDTH];
extern void Test_Camera_Reprot(void);
extern camera_config_t cameraConfig;   //����ͷ���ýṹ��
#endif