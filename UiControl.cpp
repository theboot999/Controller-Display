#include "UiControl.h"

  UiControl::UiControl(int16_t x, int16_t y)
  {
    this->x = x;
    this->y = y;
  }

  UiControl::UiControl(int16_t x, int16_t y, int16_t width, int16_t height)
  {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
  }

  bool UiControl::IsTouchInBounds(UiInput *uiInput)
  {
    if(isEnabled)
    {
      int16_t touchX = uiInput->GetTouchOneX();
      int16_t touchY = uiInput->GetTouchOneY();
      if(touchX > x && touchX < x + width && touchY > y && touchY < y + height)
      {
        return true;
      }
    }
    return false;
  }

  void UiControl::SetText(const String &text)
  {
    
  }
 

  void UiControl::Update(UiInput *uiInput)
  {

  }
  void UiControl::Draw(Display *display)
  {

  }