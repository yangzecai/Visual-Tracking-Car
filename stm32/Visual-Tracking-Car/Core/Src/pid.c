#include "pid.h"

int PIDControl(PID *pid, int current, int target)
{
	int error = target - current;
	pid->output += pid->kp * (error - pid->lastError) + 
				   pid->ki * error * pid->dt +
			       pid->kd * (error - 2*pid->lastError + 
							  pid->lastLastError) / pid->dt;
	
	pid->lastLastError = pid->lastError;
	pid->lastError = error;
	
	return (int)pid->output;
}

