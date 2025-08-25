#ifndef INC_TIMER_UTILS_H_
#define INC_TIMER_UTILS_H_

#include "main.h"
#include "stdbool.h"

#define ADC_CHANNELS_NUM 3

extern TIM_HandleTypeDef htim1;

extern bool half_wave_Condition;

extern uint16_t valueADC_ch[]; // adc
extern uint16_t adc_value; // adc
extern uint16_t angle_alpha; //
extern uint16_t R_CmpAB_T;
extern uint16_t R_CmpAB_Y;
extern uint16_t count_reset;
extern uint16_t count_max;
extern uint16_t count_AB;
extern uint16_t current_value;
extern uint8_t current_state;

#endif /* INC_TIMER_UTILS_H_ */
