#ifndef __PID_H__
#define __PID_H__

typedef struct {
	float kp;
	float ki;
	float kd;
	int lastError;
	int lastLastError;
	int current;	// for debug
} PID;

int PIDControl(PID *pid, int current, int target);
int getCurrent(PID *pid);

#endif
