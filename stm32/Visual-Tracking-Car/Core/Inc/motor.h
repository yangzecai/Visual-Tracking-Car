#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "tim.h"
#include "pid.h"

typedef struct {
	GPIO_TypeDef*		in1Gpiox;
	uint16_t 			in1Pinx;
	GPIO_TypeDef*		in2Gpiox;
	uint16_t 			in2Pinx;
	TIM_HandleTypeDef*	pwmHtimx;
	uint32_t			pwmChannelx;
	TIM_HandleTypeDef* 	encoderHtimx;
	int 				maxAbsSpeed;
	PID					pid;
	int					curSpeed;
	int					trgtSpeed;
} Motor;

void motorControlwithPID(Motor *motor);
void setTrgtSpeed(Motor *motor, int speed);
void motorDrive(Motor *motor, int speed);
short readAndClearEncoder(Motor *motor);
int getCurSpeed(Motor *motor); // for debug

#endif
