#include "main.h"
#include "timer_utils.h"

void reset_timer(TIM_HandleTypeDef* htim){
	HAL_TIM_Base_Stop(htim);
	__HAL_TIM_SET_COUNTER(htim, 0);
	HAL_TIM_Base_Start(htim);
}
