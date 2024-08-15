#ifndef Timer_h
#define Timer_h
#include "Arduino.h"

class Timer
{
public:
	Timer(const unsigned long& timeout);
	void SetNewTimeout(const unsigned long& timeout);
	void Reset();
	bool Fire();

private:
	const unsigned long UL_MAX = 4294967295;
	unsigned long timeDiff = 0;
	unsigned long timeBench = 0;
	unsigned long period = 0;
};
#endif