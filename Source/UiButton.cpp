#include "UiButton.h"

  UiButton::UiButton(int16_t x, int16_t y, int16_t width, int16_t height) : UiControl(x, y, width, height)
  {   

  }  

  void UiButton::StartButtonTimer()
  {
    buttonTimer.SetNewTimeout(1000);
  }

  bool UiButton::IsButtonHeldEnough()
  {
    if(buttonTimer.Fire())
    {
      buttonTimer.SetNewTimeout(holdDelay);
      return true;     
    }
    return false;
  }

  
  void UiButton::Update(UiInput *uiInput)
  {
    isClicked = false;
    isHoldTick = false;
    if(uiInput->IsTouchingOne())
    {
      if (UiControl::IsTouchInBounds(uiInput))
      {
        if(!isDown)  // if we previously where not touching
        {
          isDown = true;
          StartButtonTimer();
        }
        else
        {
          if(IsButtonHeldEnough())
          {
            isHoldTick = true;
          }                           
        }           
      }
      else
      {
        if(isDown)
        {
          isDown = false;
        }
      }
    }
    else
    {
      if(isDown)
      {
        isDown = false;
        isClicked = true;
      }
    }
  }

  void UiButton::Draw(Display *display)
  {

  }



  UiButtonText::UiButtonText(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t colorBack, uint8_t colorHeld, uint8_t colorBorder, uint8_t colorText, uint8_t font, String text) : UiButton(x, y, width, height)
{
  this->colorBack = colorBack;
  this->colorHeld = colorHeld;
  this->colorBorder = colorBorder;
  this->colorText = colorText;
  this->font = font;
  text.toCharArray(this->text, maxButtonChar);
  centerX = x + (width / 2);
  centerY = y + (height / 2);
}

void UiButtonText::Update(UiInput *uiInput)
{
  UiButton::Update(uiInput);  
}

void UiButtonText::SetText(const String &text)
{
  text.toCharArray(this->text, maxButtonChar);
  centerX = x + (width / 2);
  centerY = y + (height / 2);
}

void UiButtonText::Draw(Display *display)
{
  if(isDown)
  {
    display->DrawRectangleFillBorder(x, y, width, height, 2, colorBorder, colorHeld);
  }
  else
  {
    display->DrawRectangleFillBorder(x, y, width, height, 2, colorBorder, colorBack);
  }
  display->DrawCharString(centerX, centerY, text, font, colorText, true, true);
}