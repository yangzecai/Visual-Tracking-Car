#ifndef __CAR_H__
#define __CAR_H__

#include "tim.h"

typedef struct {
	GPIO_TypeDef*		in1Gpiox;
	uint16_t 			in1Pinx;
	GPIO_TypeDef*		in2Gpiox;
	uint16_t 			in2Pinx;
	TIM_HandleTypeDef*	pwmHtimx;
	uint32_t			pwmChannelx;
	TIM_HandleTypeDef* 	encoderHtimx;
} Motor;

extern Motor motor1;
extern Motor motor2;

void motor1Init(void);
void motor2Init(void);
short readAndClearEncoder(Motor*);
void motorDrive(Motor*, int);

#endif
