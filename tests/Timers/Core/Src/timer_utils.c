#include "main.h"
#include "timer_utils.h"

static uint32_t counterMAX;

void reset_timer(TIM_HandleTypeDef* htim){
	HAL_TIM_Base_Stop(htim);
	counterMAX = __HAL_TIM_GET_COUNTER(htim);
	__HAL_TIM_SET_COUNTER(htim, 0);
	HAL_TIM_Base_Start(htim);
}
