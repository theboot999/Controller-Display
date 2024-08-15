#include "GraphicsDevice.h"
#include "Pins.h"

static uint8_t is8347;

void GraphicsDevice::Init()
{
  pinMode(pDiMultiPlexEnable, OUTPUT); //Its already output but this is a backup
  digitalWriteFast(pAnMultiPlexEnable, HIGH); //Disable Analogue Multiplexer
  analogWriteFrequency(pDisplayBackLight, 24000);
  Reset();

  CS_ACTIVE;
  delay(50);
    
//Set PLL Multiplier to 120mhz
    WriteCmd(0xE2); delay(1);
    WriteSlow8(0x23);
    WriteSlow8(0x02);
    WriteSlow8(0x04);

//PLL Enable
    WriteCmd(0xE0); delay(1);
    WriteSlow8(0x01);
    delay(10);
    WriteCmd(0xE0); delay(1);
    WriteSlow8(0x03);
    delay(10);

//Soft Reset
    WriteCmd(0x01); delay(1);
    delay(100);

//PLL Setting for PCLK - Depends on resolution
    WriteCmd(0xE6); delay(1);
    WriteSlow8(0x04);
    WriteSlow8(0x93);
    WriteSlow8(0xE0);

//LCD Specification
    WriteCmd(0xB0); delay(1);
    //          76543210
    WriteSlow8(B00100000);
    WriteSlow8(0x00);
    WriteSlow8(0x03);
    WriteSlow8(0x1F);
    WriteSlow8(0x01);//
    WriteSlow8(0xDF);
    WriteSlow8(0x00);

//HSYNC HT=930, HPS=46, HPW=48, LPS=15   //For 60hz
    //uint16_t verticalScans = 881; Use this for 70 HZ
    //uint16_t verticalScans = 845; //845 is lowest it will go
    uint16_t verticalScans = 930; //845 is lowest it will go
    WriteCmd(0xB4); delay(1);
    WriteSlow8((verticalScans >> 8));
    WriteSlow8(verticalScans & 0xFF);
    WriteSlow8(0x00);
    WriteSlow8(0x2E);
    WriteSlow8(0x30);
    WriteSlow8(0x00);
    WriteSlow8(0x0F);
    WriteSlow8(0x00);

//VSYNC VT=588, VPS=16, VPW=16, FPS=8
    //uint16_t horizontalScans = 532; Use this for 70HZ
    //uint16_t horizontalScans = 647;
    uint16_t horizontalScans = 588;
    WriteCmd(0xB6); delay(1);
    WriteSlow8((horizontalScans >> 8));
    WriteSlow8(horizontalScans & 0xFF);
    WriteSlow8(0x00);
    WriteSlow8(0x10);
    WriteSlow8(0x10);
    WriteSlow8(0x00);
    WriteSlow8(0x08);

//GPIO[3:0] out 1  
    WriteCmd(0xBA); delay(1);
    WriteSlow8(0x0F);
    //B00000011

//GPIO3=input, GPIO[2:0]=output
    WriteCmd(0xB8); delay(1);
    WriteSlow8(0x07);
    WriteSlow8(0x01);

//pixel data interface
    WriteCmd(0xF0); delay(1);
    WriteSlow8(B00000011);
    delay(1);

//Display Off
    WriteCmd(0x28); delay(1);

//Sleep Out
    WriteCmd(0x11); delay(1);
    delay(100);

//set TE pin
    WriteCmd(0x35); delay(1);
    WriteSlow8(0x00);

//Set the rotation
    WriteCmd(0x36); delay(1);
    WriteSlow8(B00000000);
              //76543210
/*
//Write Gamme
  WriteCmd(0xBC); 
  delay(10);
  WriteSlow8(settings->displayContrast);  //p1 contrast
  WriteSlow8(settings->displayBrightness);  //p2 brightness
  WriteSlow8(settings->displaySaturation);  //p3 saturation
  WriteSlow8(B00000001);  //p4
*/

   //clear screen to black
   SetAddressWindow(0, 0, dWidth, dHeight);
   WriteCmd(0x2C);
   WriteFast16(0x0000);

    for (int i = 0; i < 384000; i++) 
    {
      PIN_LOW(WR_PORT, WR_PIN);
      PIN_LOW(WR_PORT, WR_PIN);
      PIN_HIGH(WR_PORT, WR_PIN);
    }

  CS_IDLE;
  analogWrite(pDisplayBackLight, 255);
}

void GraphicsDevice::Reset()
{
    GPIO6_GDIR |=  GMASK;
    WR_OUTPUT; 
    CD_OUTPUT; 
    CS_OUTPUT;
    CS_IDLE;
    WR_IDLE;
    delay(80);
}

void GraphicsDevice::WriteCmdData(uint16_t cmd, uint16_t dat)
{
    CS_ACTIVE;
    WriteCmd(cmd);
    WriteSlow16(dat);
    CS_IDLE;
}


void GraphicsDevice::WriteCmdParam2(uint8_t cmd, uint8_t d1, uint8_t d2)
{
    uint8_t d[2];
    d[0] = d1, d[1] = d2;
    WriteCmdParamN(cmd, 2, d);
}


void GraphicsDevice::WriteCmdParamN(uint16_t cmd, int8_t N, uint8_t* block)
{
    CS_ACTIVE;
    WriteCmd(cmd);
    while (N-- > 0) {
        uint8_t u8 = *block++;
        WriteSlow8(u8);
        if (N && is8347) {
            cmd++;
            WriteCmd(cmd);
        }
    }
    CS_IDLE;
}

void GraphicsDevice::SetAddressWindow(int16_t x, int16_t y, int16_t x1, int16_t y1)
{
  CS_ACTIVE;
  WriteCmd(0x2A);
  WriteSlow8(x >> 8);
  WriteSlow8(x);
  WriteSlow8(x1 >> 8);
  WriteSlow8(x1);

  WriteCmd(0x2B);
  WriteSlow8(y >> 8);
  WriteSlow8(y);
  WriteSlow8(y1 >> 8);
  WriteSlow8(y1);
  CS_IDLE;
}



void GraphicsDevice::SetBackLight(int16_t backLight)
{
  if(backLight < -15)
  {
    backLight = constrain(map(backLight, -255, -16, -255, 1), -255, -1);
    backLight *= -1;
  }
  else if(backLight > 15)
  {
    backLight = constrain(map(backLight, 16, 255, 1, 255), 1, 255);
  }
  else
  {
    backLight = 0;
  }

  if(this->backLight != backLight)
  {
    this->backLight = backLight;
    analogWrite(pDisplayBackLight, backLight);
  }
}

void GraphicsDevice::OutputBuffer(uint8_t *buffer)
{
  digitalWriteFast(pAnMultiPlexEnable, HIGH); //Disable Analogue Multiplexer it shares a pin


  //set_arm_clock(24000000);  //need to test clock scaling. will it effect spi and interrupt on sending our radio signals


  //set_arm_clock(600000000);


  SetAddressWindow(0, 0, dWidth, dHeight);
  
  CS_ACTIVE;
  WriteCmd(0x2C);
  
  uint8_t index;
  uint16_t color;

  index = buffer[0];
  color = pallete.palleteList[index];
  WriteFast16(color);
      
  for (uint32_t pointer = 383999; pointer > 0; pointer--) 
  {
    PIN_LOW(WR_PORT, WR_PIN);
    //PIN_LOW(WR_PORT, WR_PIN);
    

    if(index != buffer[pointer - 1])
    {
      index = buffer[pointer - 1];
      color = pallete.palleteList[index];
      PIN_HIGH(WR_PORT, WR_PIN);
      WriteFast16(color);
    }
    else
    {
      PIN_HIGH(WR_PORT, WR_PIN);
    }
  }
  CS_IDLE;

}

