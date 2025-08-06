/*
 * adc.h
 *
 *  Created on: Aug 6, 2025
 *      Author: sbalbashov
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_
/* Includes ----------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Defines -----------------------------------------------------------*/
#define ADC_CHANNELS_NUM 3

/* Variables ---------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern uint16_t valueADC_ch[ADC_CHANNELS_NUM];
extern ADC_AnalogWDGConfTypeDef AnalogWDGConfig;

/* Function prototypes -----------------------------------------------*/
extern void MX_ADC1_Init(void);
extern void checkChanelns(uint16_t* valueADC_ch, uint16_t size);

#endif /* INC_ADC_H_ */
