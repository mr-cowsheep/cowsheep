/*!
* @file       MT9V032.h
* @brief      ���ݹٷ����������ͷ���д��������������ͷ����������
* @details
* @author     pig's grief
* @version    v1.0
* @date       2019-4-22
* @to do
*/
# ifndef _MT9V032_H_
# define _MT9V032_H_
#define IMAGEW 188
#define IMAGEH 120
#define APP_CAMERA_WIDTH  (IMAGEW)
#define APP_CAMERA_HEIGHT (IMAGEH/2)  //һ��uint16_t����װ���������أ� һ��IMAGEW ������ʵװ�����е����ص㣬���Ը߶�/2
#define APP_BPP 2 //���ظ�ʽ��
#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge) //ʹ������ͷ��������
#define APP_CAMERA_FRAME_BUFFER_COUNT 4 
#define FRAME_BUFFER_ALIGN 64  //���� 

extern camera_device_handle_t cameraDevice;
extern camera_receiver_handle_t cameraReceiver;
extern camera_config_t cameraConfig;   //����ͷ���ýṹ��


#endif