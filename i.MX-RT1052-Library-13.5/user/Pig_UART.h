/*!
* @file       Pig_UART.c
* @brief      ���ݹٷ���fsl_lpuart���һЩ����Ľӿ�
* @details    ����UART�����š�ʱ����Ҫ�ùٷ����ù���������
* @author     pig's grief
* @version    v1.0
* @date       2019-4-11
* @to do      ����UART��eDMAģʽ
*/
# ifndef _PIG_UART_H_
# define _PIG_UART_H_

#include "include.h"


/*!
* @brief UART��ʼ������ѯģʽ�����ж���DMA)
* ����ֻ�ṩ�޸Ĳ��������õ�ѡ������Ҫ�޸ı�����ã���Ҫ�����޸���صĽṹ���ʱ��Ƶ��
*
* @param UARTn UARTͨ����LPUART1��LPUART2��
* @param bound Ҫ�趨�Ĳ�����
*/
void UART_Init(LPUART_Type * UARTn, uint32_t bound);

/*!
* @brief UART����һ���ַ�������������ǵ��ùٷ��⣩
*
* @param UARTn UARTͨ����LPUART1��LPUART2��
*/
uint8 UART_GetChar(LPUART_Type *UARTn);
/*!
* @brief UART����һ���ַ���������������ǵ��ùٷ��⣩
*
* @param UARTn UARTͨ����LPUART1��LPUART2��
* @param databuff �ַ�������
* @param datasize ������������
*/
void UART_Send(LPUART_Type *UARTn, uint8 * databuff, uint8 datasize);

# endif