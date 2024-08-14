#ifndef Screen_h
#define Screen_h
#include "Frametime.h"
#include "Display.h"
#include "UiPanel.h"


class Screen
{
public:
  
private:
  UiPanel *uiPanelList[1]; 
  Display *display;
  FrameTime *frameTime;
  int8_t uiPanelCount = 1;

public:
  void Init(UiInput *uiInput, Display *display, FrameTime *frameTime);
  void Update();
  void Draw();

};
#endif
