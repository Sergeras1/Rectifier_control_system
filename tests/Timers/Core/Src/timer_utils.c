#include "main.h"
#include "timer_utils.h"
#include "stdbool.h"

uint16_t adc_value = 0;
bool half_wave_Condition = false; // true - нарастающий полупериод, false - падающий полупериод
uint16_t current_value = 0;
uint16_t  angle_alpha = 45;
uint16_t valueADC_ch[ADC_CHANNELS_NUM] = {0,};

uint16_t R_CmpAB_T;
uint16_t R_CmpAB_Y;
uint16_t count_reset;

/** II. TIM1 **/
void StartPulseSequence(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim6, uint16_t size){
	uint16_t tim1_value = __HAL_TIM_GET_COUNTER(htim1);
	if(half_wave_Condition == true){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8 | GPIO_PIN_9, 1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
	}
	R_CmpAB_Y = (count_reset * (angle_alpha + 30)) / 180;
	HAL_TIM_Base_Start_IT(htim6);
}
/** I. Analog WatchDog **/
void UpdatePhaseAngleControl(ADC_AnalogWDGConfTypeDef* AnalogWDGConfig,
				   TIM_HandleTypeDef* htim,
				   uint16_t* valueADC_ch,
				   uint16_t size){
	static uint8_t prev_state[3] = {0};

	// 1. Фиксируем значение счетчика
	count_reset = __HAL_TIM_GET_COUNTER(htim);

	// 2. Вычисления для R_CmpAB_T
	R_CmpAB_T = (count_reset * angle_alpha) / 180;
	if(R_CmpAB_T < htim->Instance->ARR){
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, R_CmpAB_T);
	}
	// 3. Анализ каналов АЦП
	for (int i = 0; i < size; ++i) {
		uint8_t current_state = prev_state[i];
		if (valueADC_ch[i] >= AnalogWDGConfig->HighThreshold) current_state = 1; // Восходящий фронт
		else if (valueADC_ch[i] <= AnalogWDGConfig->LowThreshold) current_state = 0; // Низходящий фронт

		//Фиксируем фронты
		if(prev_state[i] == 0 && current_state == 1) half_wave_Condition = true;
		else if(prev_state[i] == 1 && current_state == 0) half_wave_Condition = false;

		prev_state[i] = current_state;
	}

}


