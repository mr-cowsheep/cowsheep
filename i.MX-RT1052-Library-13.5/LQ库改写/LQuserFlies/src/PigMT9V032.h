# ifndef _PIGMT9V032_H_
# define _PIGMT9V032_H_

#define COL     188//ͼ����   ��Χ1-752     K60�ɼ���������188
#define ROW     120//ͼ��߶�	��Χ1-480


//����ͷ����ö��
typedef enum
{
    INIT = 0,               //����ͷ��ʼ������
    AUTO_EXP,               //�Զ��ع�����
    EXP_TIME,               //�ع�ʱ������
    FPS,                    //����ͷ֡������
    SET_COL,                //ͼ��������
    SET_ROW,                //ͼ��������
    LR_OFFSET,              //ͼ������ƫ������
    UD_OFFSET,              //ͼ������ƫ������
    GAIN,                   //ͼ��ƫ������
    CONFIG_FINISH,          //������λ����Ҫ����ռλ����

    SET_EXP_TIME = 0XF0,    //���������ع�ʱ������
    GET_STATUS,             //��ȡ����ͷ��������
    GET_VERSION,            //�̼��汾������

    SET_ADDR = 0XFE,        //�Ĵ�����ַ����
    SET_DATA                //�Ĵ�����������
}CMD;

extern unsigned char mt9v032_finish_flag;  //һ��ͼ��ɼ���ɱ�־λ
extern unsigned char image_1[ROW][COL];      //�ڲ�ʹ��


//��������

#define MT9V032_COF_UART_1    LPUART4 //��������ͷ��ʹ�õ��Ĵ���     
#define MT9V032_COF_UART_2    LPUART3 //��������ͷ��ʹ�õ��Ĵ���     

typedef enum
{
    Image_Collecting,
    Image_CollectFinish,
    Image_Dealing,
    Image_DealingFinsh
}ImageDealState;

extern ImageDealState Image_1_DealState_Now;
extern ImageDealState Image_2_DealState_Now;

void   mt9v032_cof_uart_interrupt(void);
void   VSYNC_1(void);
void   VSYNC_2(void);
void   camera_init_1(void);
void   camera_init_2(void);
void   seekfree_sendimg_032(void);
void   row_finished(void);
void sendimg(void);

# endif