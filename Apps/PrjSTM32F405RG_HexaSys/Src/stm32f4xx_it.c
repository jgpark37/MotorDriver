/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* USER CODE BEGIN 0 */
#include "ServoControllerInterface.h"

#include "math.h"
#include "string.h"
#include "main.h" //pjg++190322

uint8_t gRunCurrentController = 0;
uint16_t nRunLed = 0;
uint32_t nExt2Irq = 0;
uint32_t nAdcIrq = 0;
uint32_t nDma2Irq = 0;

int16_t adc12Result[2];
int16_t adc3Result[2];
int32_t encoderPulse = 0;
uint8_t hallSensorStatus = 0x00;
uint8_t encoder1Type = 0;
uint8_t encoder2Type = 0;
uint16_t nStatus2Led = 0;

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc3;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern CAN_HandleTypeDef hcan1;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI line2 interrupt.
*/
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */
#if defined(HEXA_BD_10A_V10) //single leg hexar version
	EXTI2_INT_TP_H;//GPIOB->BSRR |= GPIO_PIN_8;			//	Set -> High
#endif	
	hallSensorStatus = (GPIOA->IDR >> 8) & 0x7;
      /*
	if(hallSensorStatus < 1 || hallSensorStatus > 6) {  //sec++171204 hall err
		__asm("nop"); __asm("nop"); 	//pjg++171204
		hallSensorStatus = (GPIOA->IDR >> 8) & 0x7;
		if(hallSensorStatus < 1 || hallSensorStatus > 6) { 	//pjg++171204
			__asm("nop"); __asm("nop"); 
			hallSensorStatus = (GPIOA->IDR >> 8) & 0x7;
		}
	}*/
	encoderPulse = (int32_t)TIM2->CNT;
	CalculateElecAngle(hallSensorStatus, encoderPulse);
	
  /* USER CODE END EXTI2_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_IRQn 1 */
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
	
	nExt2Irq++;
	GREEN_LED_TOGGLE;
#if defined(HEXA_BD_10A_V10) //single leg hexar version
	EXTI2_INT_TP_L;//GPIOB->BSRR |= (GPIO_PIN_8 << 16);	//	Reset -> Low (TP1)
#endif
  /* USER CODE END EXTI2_IRQn 1 */
}

/**
* @brief This function handles ADC1, ADC2 and ADC3 global interrupts.
*/
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */
#if defined(HEXA_BD_10A_V10) //single leg hexar version
	ADC_INT_TP_H;//GPIOB->BSRR |= GPIO_PIN_9;			//	Set -> High
#endif	
	adc12Result[0] = (int16_t)HAL_ADC_GetValue(&hadc1); //U current
	adc12Result[1] = (int16_t)HAL_ADC_GetValue(&hadc2); //V current
	
	RunCurrentController(adc12Result[0], adc12Result[1]);
	
	if(++gRunCurrentController == 10) {
		CalculateVelocity(encoderPulse);
		CalculateLoadTorque();
		RunVelocityPositionController();
		gRunCurrentController = 0;
	}

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  HAL_ADC_IRQHandler(&hadc2);
  HAL_ADC_IRQHandler(&hadc3);
  /* USER CODE BEGIN ADC_IRQn 1 */
  	nAdcIrq++;
	
	if(++nRunLed == 12500) {
		RUN_LED_TOGGLE;//GPIOC->ODR ^= GPIO_PIN_13;	//BLUE:RUN_LED
		nRunLed = 0;
	}
	
#if defined(HEXA_BD_10A_V10) //single leg hexar version
	ADC_INT_TP_L;//GPIOB->BSRR |= (GPIO_PIN_9 << 16);	//	Reset -> Low (TP2)
#endif
  /* USER CODE END ADC_IRQn 1 */
}

/**
* @brief This function handles CAN1 RX0 interrupts.
*/
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

  /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
* @brief This function handles DMA2 stream0 global interrupt.
*/
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */
	CalculateVoltageTemperature(adc3Result[0], adc3Result[1]);
    //RUN_LED_TOGGLE;
  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc3);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/* USER CODE BEGIN 1 */
/**
  * @brief  CAN RX OK callbacks
  * @param  hcan: CAN handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
	if(HAL_CAN_Receive_IT(hcan, CAN_FIFO0) != HAL_OK) {
    	//Error_Handler();
	}  
  
	if(ProcessSdoCan(&hcan->pRxMsg->StdId, hcan->pRxMsg->Data, &hcan->pRxMsg->DLC) >= 0) {
		memcpy(hcan->pTxMsg, hcan->pRxMsg, sizeof(CanRxMsgTypeDef));
		HAL_CAN_Transmit(hcan, 10);
		if (nStatus2Led++ > 50) {
			nStatus2Led = 0;
			STATUS2_LED_TOGGLE;
		}
	}
	while (Callback_MotorInfoSend(&hcan->pRxMsg->StdId, hcan->pRxMsg->Data, &hcan->pRxMsg->DLC)) { //pjg++180710
		memcpy(hcan->pTxMsg, hcan->pRxMsg, sizeof(CanRxMsgTypeDef));
		HAL_CAN_Transmit(hcan, 10);
		if (nStatus2Led++ > 50) {
			nStatus2Led = 0;
			STATUS2_LED_TOGGLE;
		}
	}
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
