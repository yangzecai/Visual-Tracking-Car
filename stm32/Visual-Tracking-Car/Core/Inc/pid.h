#ifndef __PID_H__
#define __PID_H__

typedef struct {
	float kp;
	float ki;
	float kd;
	float dt;
	int lastError;
	int lastLastError;
	float output;
} PID;

int PIDControl(PID *pid, int current, int target);

#endif
