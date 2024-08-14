#ifndef UiPanel_h
#define UiPanel_h
#include "Display.h"
#include "UiInput.h"
#include "UiControl.h"
#include "UiButton.h"
#include "FrameTime.h"

class UiPanel
{
  UiControl *uiControlList[20]; 
  byte controlCount = 0;


protected:
  UiInput *uiInput;
  Display *display;
  FrameTime *frameTime;
public:
  bool isActive = false;
  UiPanel(UiInput *uiInput, Display *display, FrameTime *frameTime) {this->uiInput = uiInput; this->display = display; this->frameTime = frameTime;}
  virtual void SetText(int index, const String &text);
  virtual void Init();
  virtual void Update();
  virtual void Draw();


protected:
  void AddControl(UiControl &uiControl);

  
  bool IsControlClick(int index);
  bool IsControlClickOrTick(int index);
  bool IsControlEnabled(int index) { return uiControlList[index]->isEnabled; }
  bool IsControlDown(int index) { return uiControlList[index]->isDown; }
  void ToggleEnabled(int index, bool value) { uiControlList[index]->isEnabled = value;}
  
};

#endif
