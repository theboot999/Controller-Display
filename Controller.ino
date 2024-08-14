#include "UiInput.h"
#include "Display.h"
#include "Fonts.h"
#include "Pallete.h"
#include "Screen.h"
#include "FrameTime.h"

//uint8_t serial1_extra_rx_buffer[512];
UiInput uiInput;
Display display;
Screen screen;
FrameTime frameTime;

void setup() {
  // put your setup code here, to run once:
  uiInput.Init();
  display.Init(&uiInput);
  screen.Init(&uiInput, &display, &frameTime);
}


void loop() {
  // put your main code here, to run repeatedly:
  frameTime.Update(FTIME_TOTAL);
  while(!digitalReadFast(pDisplayTearingEffect)) { }
  frameTime.Start(FTIME_TOTAL);
  frameTime.Start(FTIME_OUTPUTSCREEN);
  display.OutputToScreen();
  frameTime.Update(FTIME_OUTPUTSCREEN);





  display.ClearBuffer();
  
  uiInput.Update();
  screen.Update();

  screen.Draw();

}
