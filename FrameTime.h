#ifndef FrameTime_h
#define FrameTime_h
#include "Arduino.h"

#define FTIME_TOTAL         0
#define FTIME_3DVERTICE     1
#define FTIME_3DDRAW        2
#define FTIME_TEST          3
#define FTIME_OUTPUTSCREEN  4
const int8_t count = 5;

class FrameTime
{
private:
  uint32_t clockStart[count];
  uint32_t clockPassed[count];
  double frameTimeMS[count];
  
public:
  void Start(uint8_t timerM)
  {
    clockStart[timerM] = ARM_DWT_CYCCNT;
  }

  void Update(uint8_t timerM)
  {
    uint32_t clockCurrent = ARM_DWT_CYCCNT;

    if(clockCurrent < clockStart[timerM]) //we have overflowed
    {
      clockPassed[timerM] = (0xFFFFFFFF - clockStart[timerM]) + clockCurrent + 1; // +1 to include the zero count
    }
    else
    {
      clockPassed[timerM] = clockCurrent - clockStart[timerM];
    }
    frameTimeMS[timerM] = (double)clockPassed[timerM] * 0.00166666666666;
  }

  uint32_t GetFrameTimeMicros(uint8_t timerM)
  {
    return frameTimeMS[timerM];
  }


};
#endif