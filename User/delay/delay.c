/**
  ******************************************************************************
  * @file    delay.c
  * @author  CSA
  * @version V1.0
  * @date    2015-05-08
  * @brief   延时应用函数
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "delay.h"   

 /**
  * @brief  延时函数
  * @param  无
  * @retval 无
  */
	void delay(uint32_t count)	
	{
		for(;count!=0;count--);
  }
/*********************************************END OF FILE**********************/
