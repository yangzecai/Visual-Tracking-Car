#include "pid.h"

int PIDControl(PID *pid, int current, int target)
{
	int error = target - current;
	int control = current;
	control += pid->kp * (error - pid->lastError) + 
			   pid->ki * error +
			   pid->kd * (error - 2*pid->lastError + pid->lastLastError);
	pid->lastLastError = pid->lastError;
	pid->lastError = error;
	pid->current = current;
	return control;
}

int getCurrent(PID *pid) {
	return pid->current;
}
