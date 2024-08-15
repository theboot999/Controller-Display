#ifndef UiControl_h
#define UiControl_h
#include "UiInput.h"
#include "Display.h"

class UiControl
{
protected:
  int16_t x = 0;
  int16_t y = 0;
  int16_t width = 0;
  int16_t height = 0;

public:
  bool isClicked = false;
  bool isDown = false;
  bool isHoldTick = false;
  bool isEnabled = true;

  UiControl(int16_t x, int16_t y);
  UiControl(int16_t x, int16_t y, int16_t width, int16_t height);
  virtual void SetText(const String &text);
  virtual void Update(UiInput *uiInput);
  virtual void Draw(Display *display);
  

protected:
  bool IsTouchInBounds(UiInput *uiInput);

private:


};

#endif
