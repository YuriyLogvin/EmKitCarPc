/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define Rs485TxEn_Pin GPIO_PIN_13
#define Rs485TxEn_GPIO_Port GPIOC
#define SPEEDS1_Pin GPIO_PIN_0
#define SPEEDS1_GPIO_Port GPIOC
#define SPEEDS2_Pin GPIO_PIN_1
#define SPEEDS2_GPIO_Port GPIOC
#define ADC_VCC12_Pin GPIO_PIN_3
#define ADC_VCC12_GPIO_Port GPIOA
#define GPIO_Out4_Pin GPIO_PIN_4
#define GPIO_Out4_GPIO_Port GPIOA
#define GPIO_Out3_Pin GPIO_PIN_5
#define GPIO_Out3_GPIO_Port GPIOA
#define GPIO_Out2_Pin GPIO_PIN_6
#define GPIO_Out2_GPIO_Port GPIOA
#define GPIO_Out1_Pin GPIO_PIN_7
#define GPIO_Out1_GPIO_Port GPIOA
#define SPI2_HOLD_Pin GPIO_PIN_4
#define SPI2_HOLD_GPIO_Port GPIOC
#define SPI2_CS_MEM_Pin GPIO_PIN_5
#define SPI2_CS_MEM_GPIO_Port GPIOC
#define InpOpto2_Pin GPIO_PIN_0
#define InpOpto2_GPIO_Port GPIOB
#define InpOpto1_Pin GPIO_PIN_1
#define InpOpto1_GPIO_Port GPIOB
#define SPI2_WP_Pin GPIO_PIN_12
#define SPI2_WP_GPIO_Port GPIOB
#define SPI2_CS_ADC2_Pin GPIO_PIN_6
#define SPI2_CS_ADC2_GPIO_Port GPIOC
#define SPI2_CS_ADC1_Pin GPIO_PIN_7
#define SPI2_CS_ADC1_GPIO_Port GPIOC
#define LED_RED_Pin GPIO_PIN_8
#define LED_RED_GPIO_Port GPIOC
#define LED_BLUE_Pin GPIO_PIN_9
#define LED_BLUE_GPIO_Port GPIOC
#define BmsTx_Pin GPIO_PIN_10
#define BmsTx_GPIO_Port GPIOC
#define BmsRx_Pin GPIO_PIN_11
#define BmsRx_GPIO_Port GPIOC
#define Rs485Tx_Pin GPIO_PIN_12
#define Rs485Tx_GPIO_Port GPIOC
#define Rs485Rx_Pin GPIO_PIN_2
#define Rs485Rx_GPIO_Port GPIOD
#define Out5_Pin GPIO_PIN_3
#define Out5_GPIO_Port GPIOB
#define GPIO_Out6_Pin GPIO_PIN_4
#define GPIO_Out6_GPIO_Port GPIOB
#define GPIO_Out6B5_Pin GPIO_PIN_5
#define GPIO_Out6B5_GPIO_Port GPIOB
#define HlkTx_Pin GPIO_PIN_6
#define HlkTx_GPIO_Port GPIOB
#define HlkRx_Pin GPIO_PIN_7
#define HlkRx_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
