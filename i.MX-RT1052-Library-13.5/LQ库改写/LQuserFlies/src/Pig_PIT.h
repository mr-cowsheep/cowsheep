/*!
* @file       Pig_UART.h
* @brief      ���ݹٷ���fsl_pit���һЩ����Ľӿ�
* @details    ����PIT�ж����ȼ������������Ļ���Ҫ�������ã�һ��Ҫ���ùٷ����ù�������һ���ж�������
* @author     pig's grief
* @version    v1.0
* @date       2019-4-12
* @to do      
*/
# ifndef _PIG_PIT_H_
# define _PIG_PIT_H_
# include "fsl_pit.h"
/*!
* @brief PIT��ʱ�жϳ�ʼ��
* ��ʱʱ����Ҫ����50�룬��TimeInterval <= 50 000 000
*
* @param PITn PITͨ����kPIT_Chnl_0��kPIT_Chnl_1��
* @param TimeInterval Ҫ�趨�Ķ�ʱ�������λ΢��
* @param enableRunInDebug �Ƿ��ø�PIT������DEBUGģʽ
* @example PIT_Init(kPIT_Chnl_0, 1000, false);//�����ʼ��1ms�ж�
*/
void LQ_PIT_Init(pit_chnl_t channel, uint32_t count);

# endif