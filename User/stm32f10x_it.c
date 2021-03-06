/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_sdio_sdcard.h"	
#include "bsp_ov7725.h"
#include "bsp_led.h"
#include "bsp_usart1.h"
extern volatile uint8_t Ov7725_vsync;
extern volatile uint8_t screen_flag;
extern volatile uint8_t usart_flag;
extern EXTI_InitTypeDef EXTI_InitStructure;
extern uint8_t status_flag;
uint8_t ch,rec_data[20],recieve_count,length_flag,data_flag;
extern volatile uint32_t timer;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		timer++;
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
	}		 	
}
/**
  * @}
  */ 
/*
 * 函数名：SDIO_IRQHandler
 * 描述  ：在SDIO_ITConfig(）这个函数开启了sdio中断	，
 *		     数据传输结束时产生中断
 * 输入  ：无		 
 * 输出  ：无
 */
void SDIO_IRQHandler(void) 
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}

/*USART1串口接收中断*/
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
		  ch = USART_ReceiveData(USART1);
	}
	if(data_flag==1)   //接收s,t,0x30/0x31
	{
		if(recieve_count<16)
		{
			rec_data[recieve_count]=ch;
		  recieve_count++;
		}
		else
		{
			recieve_count=0;
			if(rec_data[15]==0x30)
				status_flag=rec_data[15];		//接收协调节点储存数组
			else if(rec_data[15]==0x31)
				status_flag=rec_data[15];		//接收协调节点储存数组
			else
				usart_flag=rec_data[15];		//接收协调返回标志位(s/t)
			data_flag=0;				
		}
	}
	if (length_flag==1)   //读取数据包的长度
		{					
				if(recieve_count<3)
				{
					rec_data[recieve_count]=ch;
					recieve_count++;
				}			
				else if(rec_data[2]==0x07)   //发送状态返回包
				{
					length_flag=0;
				  recieve_count=0;
				}
				else
				{					
					length_flag=0;
					data_flag=1;
					//recieve_count=0;
					recieve_count++;
				
				}						
		}
	 if(ch==0x7E)   //读取数据头
		{
				rec_data[recieve_count]=ch;					
				recieve_count++;	
				length_flag=1;	
    }
}
//7E 00 0D 90 00 13 A2 00 40 EA 13 E7 FF FE C1 30 A8
/* ov7725 场中断 服务程序 */
void EXTI0_IRQHandler(void)
{
    if ( EXTI_GetITStatus(EXTI_Line0) != RESET ) 	//检查EXTI_Line0线路上的中断请求是否发送到了NVIC 
    {
        if( Ov7725_vsync == 0 )
        {
            FIFO_WRST_L(); 	                      //拉低使FIFO写(数据from摄像头)指针复位
            FIFO_WE_H();	                        //拉高使FIFO写允许
            
            Ov7725_vsync = 1;	   	
            FIFO_WE_H();                          //使FIFO写允许
            FIFO_WRST_H();                        //允许使FIFO写(数据from摄像头)指针运动
        }
        else if( Ov7725_vsync == 1 )
        {
            FIFO_WE_L();                          //拉低使FIFO写暂停
            Ov7725_vsync = 2;
        }        
        EXTI_ClearITPendingBit(EXTI_Line0);		    //清除EXTI_Line0线路挂起标志位        
    }    
}

/* IO 线中断，中断口为PB6 */
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6) != RESET) //确保是否产生了EXTI Line中断
	{
		if(screen_flag==0)   //2016.04.21为实验所添加部分
		{
		/* LED1 取反 表示有按键按下 */		
		LED1_TOGGLE;
		
		{/* 截图标志置位，同时LED2灭 */
		//	LED2_OFF;
			screen_flag = 1;			
		}		
		}
		EXTI_ClearITPendingBit(EXTI_Line6);     //清除中断标志位
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	  
	}  
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
