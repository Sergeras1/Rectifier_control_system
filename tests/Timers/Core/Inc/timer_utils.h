#ifndef INC_TIMER_UTILS_H_
#define INC_TIMER_UTILS_H_

#include "main.h"
#include "stdbool.h"

extern TIM_HandleTypeDef htim1;

extern bool half_wave_Condition;

extern uint16_t adc_value;
extern uint16_t current_value;
extern uint16_t R_CmpAB; // Значение счетчика, который записываем в регистр сравнения/захвата
extern uint16_t count_reset; // Состоение счетчика в момент сброса
extern uint16_t count_max;	// Максимальное значение счетчика
extern uint16_t count_AB;

#endif /* INC_TIMER_UTILS_H_ */
