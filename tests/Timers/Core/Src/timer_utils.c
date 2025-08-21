#include "main.h"
#include "timer_utils.h"
#include "stdbool.h"

uint16_t adc_value = 0;
bool half_wave_Condition = false; // true - нарастающий полупериод, false - падающий полупериод
uint16_t current_value = 0;
const static uint16_t ange_alpha = 45;
uint16_t count_reset = 0;
uint16_t R_CmpAB = 45;

/** II. TIM1 **/
void process_ADC_Channel(TIM_HandleTypeDef* htim, uint16_t* valueADC_ch, uint16_t val){
	//for(int i = 0; i < ADC_CHANNELS_NUM; ++i){} - Позже буду использовать цикл, чтобы проходить по всем каналам
	current_value = valueADC_ch[0];
	if(adc_value < val && current_value >=val){ // Ловим передний фронт
		uint16_t timer_val = 0;
		timer_val = __HAL_TIM_GET_COUNTER(htim); // Зафиксировали значение перед сбросом
		__HAL_TIM_SET_COUNTER(htim, 0); // Сбрасываем регистр
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, timer_val);// Записали состояние фронта, который сформировал таймер

	}
	else if (adc_value >= val && current_value < val){ // Ловим задний фронт
		uint16_t timer_val = 0;
		timer_val = __HAL_TIM_GET_COUNTER(htim);
		__HAL_TIM_SET_COUNTER(htim, 0);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, timer_val);
	}
	adc_value = current_value;
}
/** I. WatchDog **/
void checkChanelns(ADC_AnalogWDGConfTypeDef* AnalogWDGConfig, uint16_t* valueADC_ch, uint16_t size){
	for (int i = 0; i < size; ++i) {
		if (valueADC_ch[i] <= AnalogWDGConfig->HighThreshold) {
			switch(i) {
			case 0: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
				break;
			case 1: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
				break;
			case 2: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
				break;
			}
			half_wave_Condition = true; // Запоминаем состояние полуволны
		}
		else if (valueADC_ch[i] >= AnalogWDGConfig->LowThreshold) {
			switch(i) {
			case 0: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
				break;
			case 1: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);
				break;
			case 2: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
				break;
			}
			half_wave_Condition = false;
		}
	}
	//R_CmpAB = (count_reset / 180) * ange_alpha;
	if(R_CmpAB > TIM1->ARR){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, R_CmpAB);
	}
}


