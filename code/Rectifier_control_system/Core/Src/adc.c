#include "adc.h"

uint16_t valueADC_ch[ADC_CHANNELS_NUM] = {0,}; // Буфер значений АЦП (каналы 0-2)
ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0}; // Конфигурация Analog WatchDog
DMA_HandleTypeDef hdma_adc1;
ADC_HandleTypeDef hadc1;

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

/*Инициализация АЦП1 с DMA и WatchDog*/
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 3;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_ALL_REG;
  AnalogWDGConfig.HighThreshold = 2200; // Порог вверхнего уровня
  AnalogWDGConfig.LowThreshold = 2390; // Порог нижнего уровня
  AnalogWDGConfig.ITMode = ENABLE;
  if (HAL_ADC_AnalogWDGConfig(&hadc1, &AnalogWDGConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES; // Время выборки
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

/*Функция проверки каждого канала на выход из значений Analog WatchDog*/
void checkChanelns(uint16_t* valueADC_ch, uint16_t size){
	for (int i = 0; i < size; ++i) {
		if (valueADC_ch[i] >= AnalogWDGConfig.HighThreshold) {
			switch(i) {
			case 0: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0); break;

			case 1: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0); break;

			case 2: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0); break;

			}
		}
		else if (valueADC_ch[i] <= AnalogWDGConfig.LowThreshold) {
			switch(i) {
			case 0: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1); break;

			case 1: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1); break;

			case 2: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1); break;

			}
		}
	}
}


