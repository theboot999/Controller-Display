#ifndef Pallete_h
#define Pallete_h

#include "Arduino.h"

#define CBACKGROUND     0
#define CWHITE          1
#define CBLACK          2
#define CGREY           3
#define CRED            4
#define CGREEN          5
#define CYELLOW         6

class Pallete
{
  public:
    uint16_t palleteList[10] = 
    {
      65535,      //BACKGROUND    0
      65535,      //WHITE         1
      0,          //BLACK         2
      32000,      //GREY          3
      48000,      //CRED          3
      36000,      //CGREEN          3
      18000
    };

};

#endif
