#include "motor.h"

#include <stdio.h>

short readAndClearEncoder(Motor *motor) 
{
	short cnt = (short)__HAL_TIM_GET_COUNTER(motor->encoderHtimx);
	__HAL_TIM_SET_COUNTER(motor->encoderHtimx, 0);
	return cnt;
}

void setTrgtSpeed(Motor *motor, int speed)
{
	if(speed > motor->maxAbsSpeed) {	// speed limiting
		speed = motor->maxAbsSpeed;
	} else if(speed < -motor->maxAbsSpeed) {
		speed = -motor->maxAbsSpeed;
	}
	
	motor->trgtSpeed = speed;
}

void motorDrive(Motor *motor, int speed)
{
	GPIO_PinState in1State, in2State;
	if(speed > 0) {
		in1State = GPIO_PIN_RESET;
		in2State = GPIO_PIN_SET;
	} else if (speed < 0) {
		in1State = GPIO_PIN_SET;
		in2State = GPIO_PIN_RESET;
	} else if (speed == 0) {
		in1State = GPIO_PIN_RESET;
		in2State = GPIO_PIN_RESET;
	}
	
	HAL_GPIO_WritePin(motor->in1Gpiox, motor->in1Pinx, in1State);
	HAL_GPIO_WritePin(motor->in2Gpiox, motor->in2Pinx, in2State);
	__HAL_TIM_SetCompare(motor->pwmHtimx, motor->pwmChannelx, 
						 speed > 0 ? speed : -speed);
}

void motorControlwithPID(Motor *motor) 
{
	motor->curSpeed = readAndClearEncoder(motor);
	int control = PIDControl(&motor->pid,
							 motor->curSpeed,
							 motor->trgtSpeed);
	motorDrive(motor, control);
}

int getCurSpeed(Motor *motor)
{
	return motor->curSpeed;
}
