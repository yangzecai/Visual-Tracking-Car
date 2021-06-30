#include "car.h"
#include "tim.h"
#include "usart.h"

#include <stdio.h>

#define BUFFER_SIZE 32

struct {
	Motor 				motor1;
	Motor 				motor2;
	TIM_HandleTypeDef*  pidTimer;
	UART_HandleTypeDef* nanopcUart;
	uint8_t				rxDr;
	uint8_t				rxBuf[BUFFER_SIZE];
	uint16_t 			rxSta;
} car;

int commonMotorMaxAbsSpeed = 150;

void commonMotorPIDInit(PID *pid)
{
	pid->kp = 10;
	pid->ki = 200;
	pid->kd = 0;
	pid->dt = 0.05;
	pid->lastError = 0;
	pid->lastLastError = 0;
	pid->output = 0;
}

void motor1Init()
{
	// If the motor reverses, exchange the configuration 
	// between in1 and in2
	car.motor1.in1Gpiox = MD_BIN1_GPIO_Port;
	car.motor1.in1Pinx = MD_BIN1_Pin;
	
	car.motor1.in2Gpiox = MD_BIN2_GPIO_Port;
	car.motor1.in2Pinx = MD_BIN2_Pin;

	car.motor1.pwmHtimx = &htim1;
	car.motor1.pwmChannelx = TIM_CHANNEL_2;
	
	car.motor1.encoderHtimx = &htim2;
	
	car.motor1.maxAbsSpeed = commonMotorMaxAbsSpeed;
	car.motor1.curSpeed = 0;
	car.motor1.trgtSpeed = 0;
	
	commonMotorPIDInit(&car.motor1.pid);
}

void motor2Init()
{
	// If the motor reverses, exchange the configuration 
	// between in1 and in2
	car.motor2.in1Gpiox = MD_AIN2_GPIO_Port;
	car.motor2.in1Pinx = MD_AIN2_Pin;
	
	car.motor2.in2Gpiox = MD_AIN1_GPIO_Port;
	car.motor2.in2Pinx = MD_AIN1_Pin;
	
	car.motor2.pwmHtimx = &htim1;
	car.motor2.pwmChannelx = TIM_CHANNEL_1;

	car.motor2.encoderHtimx = &htim3;
	
	car.motor2.maxAbsSpeed = commonMotorMaxAbsSpeed;
	car.motor2.curSpeed = 0;
	car.motor2.trgtSpeed = 0;
	
	commonMotorPIDInit(&car.motor2.pid);
}

void carInit()
{
	motor1Init();
	motor2Init();
	car.pidTimer = &htim4;
	car.nanopcUart = &huart2;
	car.rxSta = 0;
	
	HAL_TIM_Encoder_Start(car.motor1.encoderHtimx, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(car.motor2.encoderHtimx, TIM_CHANNEL_ALL);
	HAL_TIM_PWM_Start(car.motor1.pwmHtimx, car.motor1.pwmChannelx);
	HAL_TIM_PWM_Start(car.motor2.pwmHtimx, car.motor2.pwmChannelx);

	HAL_TIM_Base_Start_IT(car.pidTimer);
	HAL_UART_Receive_IT(car.nanopcUart, &car.rxDr, 1);
}

void carMove(int v, int w)
{
	setTrgtSpeed(&car.motor1, v + w);
	setTrgtSpeed(&car.motor2, -v + w);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
	static int cnt = 0;
	if(htim == car.pidTimer) {
		if(++cnt > 20) {
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			cnt = 0;
		}
		
		motorControlwithPID(&car.motor1);
		motorControlwithPID(&car.motor2);
		
		printf("motor1 : %3d, motor2 : %3d\r\n", 
			   getCurSpeed(&car.motor1), getCurSpeed(&car.motor2));
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
	if(huart == car.nanopcUart) {
		if((car.rxSta & 0x8000) == 0) {
			if(car.rxSta & 0x4000) {
				car.rxBuf[car.rxSta & 0x3FFF] = car.rxDr;
				if((car.rxSta & 0x3FFF) >= 5) {
					uint8_t sum = 0;
					for(int i = 0; i < 5; ++i) {
						sum += car.rxBuf[i];
					}
					if(sum == car.rxBuf[5]) {
						short v = (short) car.rxBuf[2]<<8 | car.rxBuf[1];
						short w = (short) car.rxBuf[4]<<8 | car.rxBuf[3];
						carMove(v, w);
					}
					car.rxSta = 0;
				} else {
					++car.rxSta;
				}
			} else {
				if(car.rxDr == 0xAA) {
					car.rxSta |= 0x4000;
					car.rxBuf[car.rxSta & 0x3FFF] = car.rxDr;
					++car.rxSta;
				}
			}
		}
		HAL_UART_Receive_IT(car.nanopcUart, &car.rxDr, 1);
	}
}

void test() 
{
	motorDrive(&car.motor1, 1000);
	motorDrive(&car.motor2, 1000);
}
