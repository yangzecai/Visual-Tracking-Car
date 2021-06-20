#include "car.h"

Motor motor1;
Motor motor2;

int commonMotorMaxAbsSpeed = 100;

void commonMotorPIDInit(Motor *motor)
{
	motor->pid.kp = 0.1;
	motor->pid.ki = 0;
	motor->pid.kd = 0;
	motor->pid.lastError = 0;
	motor->pid.lastLastError = 0;
}

void motor1Init()
{
	// If the motor reverses, exchange the configuration 
	// between in1 and in2
	motor1.in1Gpiox = MD_AIN1_GPIO_Port;
	motor1.in1Pinx = MD_AIN1_Pin;
	
	motor1.in2Gpiox = MD_AIN2_GPIO_Port;
	motor1.in2Pinx = MD_AIN2_Pin;
	
	motor1.pwmHtimx = &htim1;
	motor1.pwmChannelx = TIM_CHANNEL_1;

	motor1.encoderHtimx = &htim2;
	
	motor1.maxAbsSpeed = commonMotorMaxAbsSpeed;
	
	commonMotorPIDInit(&motor1);
}

void motor2Init()
{
	// If the motor reverses, exchange the configuration 
	// between in1 and in2
	motor2.in1Gpiox = MD_BIN1_GPIO_Port;
	motor2.in1Pinx = MD_BIN1_Pin;
	
	motor2.in2Gpiox = MD_BIN2_GPIO_Port;
	motor2.in2Pinx = MD_BIN2_Pin;

	motor2.pwmHtimx = &htim1;
	motor2.pwmChannelx = TIM_CHANNEL_2;
	
	motor2.encoderHtimx = &htim3;
	
	motor2.maxAbsSpeed = commonMotorMaxAbsSpeed;
	
	commonMotorPIDInit(&motor2);
}

short readAndClearEncoder(Motor *motor) 
{
	short cnt = (short)__HAL_TIM_GET_COUNTER(motor->encoderHtimx);
	__HAL_TIM_SET_COUNTER(motor->encoderHtimx, 0);
	return cnt;
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
	} else if(speed == 0) {
		in1State = GPIO_PIN_RESET;
		in2State = GPIO_PIN_RESET;
	}
	
	HAL_GPIO_WritePin(motor->in1Gpiox, motor->in1Pinx, in1State);
	HAL_GPIO_WritePin(motor->in2Gpiox, motor->in2Pinx, in2State);
	__HAL_TIM_SetCompare(motor->pwmHtimx, motor->pwmChannelx, 
						 speed > 0 ? speed : -speed);
}

void motorControlwithPID(Motor *motor, int speed) 
{
	
	if(speed > motor->maxAbsSpeed) {	// speed limiting
		speed = motor->maxAbsSpeed;
	} else if(speed < -motor->maxAbsSpeed) {
		speed = -motor->maxAbsSpeed;
	}
	
	int control = PIDControl(&motor->pid, 
							 readAndClearEncoder(motor), 
							 speed);
	motorDrive(motor, control);
}
