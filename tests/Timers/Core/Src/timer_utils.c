#include "main.h"
#include "timer_utils.h"

uint32_t counterMAX;
static uint16_t adc_value = 0;
static uint8_t front_type = 0;
static uint16_t current_value = 0;
static uint16_t timer_val = 0;

// Функция нахождения порога синусоиды и сброса таймера
void Process_ADC_Channel(TIM_HandleTypeDef* htim, uint16_t* valueADC_ch, uint16_t val){
	//for(int i = 0; i < ADC_CHANNELS_NUM; ++i){}
	current_value = valueADC_ch[0];
	if(adc_value < val && current_value >=val){ // Ловим передний фронт
		front_type = 1;
		timer_val = __HAL_TIM_GET_COUNTER(htim); // Получаем значение
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, timer_val);// Фиксируем значение в регистр перед сбросом
		__HAL_TIM_SET_COUNTER(htim, 0); // Сбрасываем регистр
	}
	else if (adc_value >= val && current_value < val){ // Ловим задемй фронт
		front_type = 0;
		timer_val = __HAL_TIM_GET_COUNTER(htim);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, timer_val);
		__HAL_TIM_SET_COUNTER(htim, 0);
	}
	adc_value = current_value;
}


