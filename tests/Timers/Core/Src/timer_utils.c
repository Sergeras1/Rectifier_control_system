#include "main.h"
#include "timer_utils.h"
#include "stdbool.h"

static uint16_t adc_value = 0;
static bool front_type = false; // true - нарастающая синусоида, false - падающая синусоида
static uint16_t current_value = 0;

// Функция нахождения порога синусоиды и сброса таймера
void Process_ADC_Channel(TIM_HandleTypeDef* htim, uint16_t* valueADC_ch, uint16_t val){
	uint16_t timer_val = 0;
	//for(int i = 0; i < ADC_CHANNELS_NUM; ++i){} - Позже буду использовать цикл, чтобы проходить по всем каналам
	current_value = valueADC_ch[0];
	if(adc_value < val && current_value >=val){ // Ловим передний фронт
		front_type = true; // Устанавливаем тип фронта
		timer_val = __HAL_TIM_GET_COUNTER(htim); // Зафиксировали значение перед сбросом
		__HAL_TIM_SET_COUNTER(htim, 0); // Сбрасываем регистр
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, timer_val);// Записали состояние фронта, который сформировал таймер

	}
	else if (adc_value >= val && current_value < val){ // Ловим задний фронт
		uint16_t timer_val = 0;
		front_type = 0;
		timer_val = __HAL_TIM_GET_COUNTER(htim);
		__HAL_TIM_SET_COUNTER(htim, 0);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, timer_val);
	}
	adc_value = current_value;
}


