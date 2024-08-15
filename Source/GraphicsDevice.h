#ifndef GraphicsDevice_H
#define GraphicsDevice_H

#include "Arduino.h"
#include "Pins.h"
#include "Pallete.h"
//LCD pins  |D15 |D14 |D13 |D12 |D11 |D10 |D9  |D8  | |D7  |D6  |D5  |D4  |D3  |D2  |D1  |D0  | 
//MXRT pin  |6.31|6.30|6.29|6.28|6.27|6.26|6.25|6.24| |6.23|6.22|6.21|6.20|6.19|6.18|6.17|6.16| 
//4.1 pins  |27  |26  |39  |38  |21  |20  |23  |22  | |16  |17  |41  |40  |15  |14  |18  |19  |





#define WR_PORT GPIO7
#define WR_PIN 16   //white is in Pin 8  //
#define CD_PORT GPIO7 //
#define CD_PIN 11    //black is in Pin 9
#define CS_PORT GPIO7   
#define CS_PIN 17     //brown is in Pin 7 //
#define GMASK ((1<<16) | (1<<17) | (1<<18) | (1<<19) | (1<<20) | (1<<21) | (1<<22) | (1<<23) |  (1<<24) | (1<<25) | (1<<26) | (1<<27) | (1<<28) | (1<<29) | (1<<30) | (1<<31) )

#define WR_ACTIVE  PIN_LOW(WR_PORT, WR_PIN)
#define WR_IDLE    PIN_HIGH(WR_PORT, WR_PIN)
#define WR_OUTPUT  PIN_OUTPUT(WR_PORT, WR_PIN)
#define CD_COMMAND PIN_LOW(CD_PORT, CD_PIN)
#define CD_DATA    PIN_HIGH(CD_PORT, CD_PIN)
#define CD_OUTPUT  PIN_OUTPUT(CD_PORT, CD_PIN)
#define CS_ACTIVE  PIN_LOW(CS_PORT, CS_PIN)
#define CS_IDLE    PIN_HIGH(CS_PORT, CS_PIN)
#define CS_OUTPUT  PIN_OUTPUT(CS_PORT, CS_PIN)

#define PASTE(x, y) x ## y
#define PIN_LOW(port, pin) PASTE(port, _DR_CLEAR) = (1<<(pin))
#define PIN_HIGH(port, pin) PASTE(port, _DR_SET) = (1<<(pin))
#define PIN_OUTPUT(port, pin) PASTE(port, _GDIR) |= (1<<(pin))

#define WriteCmd(x)  { CD_COMMAND; WriteSlow8(x); CD_DATA; }
#define WriteSlow8(x)     {WriteFast8(x); WR_ACTIVE; WR_ACTIVE; WR_IDLE; }
#define WriteSlow16(x)    { WriteFast16(x); WR_ACTIVE; WR_ACTIVE; WR_IDLE;; WR_IDLE; WR_IDLE; ;}

#define WriteFast8(d) { GPIO6_DR_CLEAR = GMASK;\
        GPIO6_DR_SET = (((d) & (1 << 0)) << 16) \
                     | (((d) & (1 << 1)) << 16) \
                     | (((d) & (1 << 2)) << 16) \
                     | (((d) & (1 << 3)) << 16) \
                     | (((d) & (1 << 4)) << 16) \
                     | (((d) & (1 << 5)) << 16) \
                     | (((d) & (1 << 6)) << 16) \
                     | (((d) & (1 << 7)) << 16);}

#define WriteFast16(d) { GPIO6_DR_CLEAR = GMASK;\
        GPIO6_DR_SET = (((d) & (1 << 0)) << 16) \
                     | (((d) & (1 << 1)) << 16) \
                     | (((d) & (1 << 2)) << 16) \
                     | (((d) & (1 << 3)) << 16) \
                     | (((d) & (1 << 4)) << 16) \
                     | (((d) & (1 << 5)) << 16) \
                     | (((d) & (1 << 6)) << 16) \
                     | (((d) & (1 << 7)) << 16)  \
		                 | (((d) & (1 << 8)) << 16) \
                     | (((d) & (1 << 9)) << 16) \
                     | (((d) & (1 << 10)) << 16) \
                     | (((d) & (1 << 11)) << 16) \
                     | (((d) & (1 << 12)) << 16) \
                     | (((d) & (1 << 13)) << 16) \
                     | (((d) & (1 << 14)) << 16) \
                     | (((d) & (1 << 15)) << 16);}
    
class GraphicsDevice
{
  private:
    const int16_t dWidth = 799;       ///< Display width as modified by current rotation
    const int16_t dHeight = 479;      ///< Display height as modified by current rotation


    
    uint8_t rotation;     ///< Display rotation (0 thru 3)
    int16_t backLight = 0;
    Pallete pallete;
public:


public:
    void Init();
    void Reset();
    void OutputBuffer(uint8_t *buffer);
    void SetBackLight(int16_t backLight);

private:

    void WriteCmdData(uint16_t cmd, uint16_t dat);
    uint16_t read16bits(void);
    void WriteCmdParam2(uint8_t cmd, uint8_t d1, uint8_t d2);
    inline void WriteCmdParam4(uint8_t cmd, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);
    void WriteCmdParamN(uint16_t cmd, int8_t N, uint8_t* block);
    void SetAddressWindow(int16_t x, int16_t y, int16_t x1, int16_t y1);

    
};

#endif