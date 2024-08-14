#ifndef UiButton_h
#define UiButton_h

#include "UiControl.h"

const int maxButtonChar = 15;

class UiButton : public UiControl
{

private:
  Timer buttonTimer = Timer(1000);
  int16_t holdDelay = 100;
  void StartButtonTimer();
  bool IsButtonHeldEnough();
  void UpdateTouch(UiInput *uiInput);
  
public:
  UiButton(int16_t x, int16_t y, int16_t width, int16_t height);
  void Update(UiInput *uiInput);
  void Draw(Display *display);
};


class UiButtonText : public UiButton
{
private:
  char text[maxButtonChar];
  uint8_t colorBack = 0;
  uint8_t colorHeld = 0;
  uint8_t colorBorder = 0;
  uint8_t colorText = 0;
  int16_t centerX = 0;
  int16_t centerY = 0;
  uint8_t font = 0;
  
private:
  void DrawText(Display *display);

public:
  UiButtonText(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t colorBack, uint8_t colorHeld, uint8_t colorBorder, uint8_t colorText, uint8_t font, String text);
  void SetText(const String &text);
  void Update(UiInput *uiInput);
  void Draw(Display *display);
};

#endif

