#include "main.h"
#include "timer_utils.h"
#include "stdbool.h"

uint16_t adc_value = 0;
bool half_wave_Condition = false; // true - нарастающий полупериод, false - падающий полупериод
uint16_t current_value = 0;

uint16_t R_CmpAB_T;
uint16_t R_CmpAB_Y;
uint16_t count_reset = 0;

/** II. TIM1 **/
void start_Tim6(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim6){
	current_value = __HAL_TIM_GET_COUNTER(htim1);
	if(current_value >= R_CmpAB_T){
	// Установить выход Out_AB
	R_CmpAB_Y = (count_reset * 75) / 180;
	__HAL_TIM_SET_COMPARE(htim1, TIM_CHANNEL_1, R_CmpAB_Y);
	}

	for (int i = 0; i < size; ++i) {
		if (valueADC_ch[i] >= AnalogWDGConfig->HighThreshold) {
			current_state = 1;
		}
		else if (valueADC_ch[i] <= AnalogWDGConfig->LowThreshold) {
			current_state = 0;
		}
		else {
			current_value = prev_state[i];
		}

		//Фиксируем фронтов
		if(prev_state[i] == 0 && current_state == 1){
			half_wave_Condition = true;
		}
		else if(prev_state[i] == 1 && count_reset == 0){
			half_wave_Condition = false;
		}

		prev_state[i] = current_state;
	}

	HAL_TIM_Base_Start_IT(htim6);
}
/** I. WatchDog **/
void checkChanelns(ADC_AnalogWDGConfTypeDef* AnalogWDGConfig,
				   TIM_HandleTypeDef* htim,
				   uint16_t* valueADC_ch,
				   uint16_t size){
	static uint8_t prev_state[3] = {0};
	uint8_t current_state;

	// 1. Фиксируем значение счетчика
	count_reset = __HAL_TIM_GET_COUNTER(htim);

	// 2. Вычисления для R_CmpAB_T
	R_CmpAB_T = (count_reset * 45) / 180;
	if(R_CmpAB_T < htim->Instance->ARR){
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, R_CmpAB_T);
	}
	// Сброс счетчика
	//__HAL_TIM_SET_COUNTER(htim, 0);
	// 3. Анализ каналов АЦП
	for (int i = 0; i < size; ++i) {
		if (valueADC_ch[i] >= AnalogWDGConfig->HighThreshold) {
			switch(i) {
			case 0: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
				break;
			case 1: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
				break;
			case 2: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
				break;
			}
				current_state = 1;
		}
		else if (valueADC_ch[i] <= AnalogWDGConfig->LowThreshold) {
			switch(i) {
			case 0: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
				break;
			case 1: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);
				break;
			case 2: HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
				break;
			}
			current_state = 0;
		}
		else {
			current_value = prev_state[i];
		}

		//Фиксируем фронтов
		if(prev_state[i] == 0 && current_state == 1){
			half_wave_Condition = true;
		}
		else if(prev_state[i] == 1 && count_reset == 0){
			half_wave_Condition = false;
		}

		prev_state[i] = current_state;
	}

}


