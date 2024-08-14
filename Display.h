#ifndef Display_h
#define Display_h
#include <Arduino.h>
#include "Pallete.h"
#include "GraphicsDevice.h"
#include "UiInput.h"
#include "Fonts.h"
#include "Timer.h"
#include "Vectors.h"

//Using Font Agane
#define FONT7     0
#define FONT7B    1
#define FONT8     2
#define FONT8B    3
#define FONT9     4
#define FONT9B    5
#define FONT10    6
#define FONT10B   7
#define FONT12    8
#define FONT12B   9
#define FONT14    10
#define FONT14B   11
#define FONT20    12
#define FONT20B   13
#define FONT30    14
#define FONT30B   15

class Display
{
public:
  UiInput *uiInput;
  GraphicsDevice graphicsDevice;
  bool isDebugPicker = false;

private:
  const int16_t dWidth = 800;
  const int16_t dHeight = 480;

  char charBuffer[24];
  int16_t clippingLeft = 0;
  int16_t clippingTop = 0;
  int16_t clippingRight = 800;
  int16_t clippingBottom = 480;
  bool isClipping = false;

  const int32_t bufferSize = 384000;
  uint8_t buffer[384000];
  const uint8_t* fontList[16];
  uint8_t *symbolList[6];

public:
	void Init(UiInput *uiInput);

private:
  void SetSymbols();
  
public:
void DrawTriangle(Vector2 Vt, Vector2 Vl, Vector2 Vr, uint8_t color);
  //Pixels
  inline void DrawPixelFast(int16_t x, int16_t y, uint8_t color) { buffer[y * dWidth + x] = color; } //remember no bounds checking for performance
  inline void DrawPixel(int16_t x, int16_t y, uint8_t color) { if(isClipping) { if(InClippingBounds(x, y)) { buffer[y * dWidth + x] = color; }} else { buffer[y * dWidth + x] = color; }}

  //Shapes
  void DrawVLine(int16_t x, int16_t y, int16_t length, uint8_t color);  
  void DrawVLine(int16_t x, int16_t y, int16_t length, int16_t thickness, uint8_t color);
  void DrawHLine(int16_t x, int16_t y, int16_t length, uint8_t color);
  void DrawHLineF(float xf, int16_t y, float xf2, uint8_t color);
  void DrawHLine(int16_t x, int16_t y, int16_t length, int16_t thickness, uint8_t color);
  void DrawRectangle(int16_t x, int16_t y, int16_t width, int16_t height, int16_t thickness, uint8_t color);
  void DrawRectangleFill(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t color);
  void DrawRectangleFillBorder(int16_t x, int16_t y, int16_t width, int16_t height, int16_t borderThickness, uint8_t colorBorder, uint8_t colorBackground);
  void DrawRoundedRectangle(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius, int16_t thickness, uint8_t color);
  void DrawRoundedRectangleFillBorder(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius, int16_t borderThickness, uint8_t colorBorder, uint8_t colorBackground);
  void DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
  void DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2,  int16_t thickness, uint8_t color);
  void DrawQuadTest(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint8_t color);
  void DrawQuadFill(Vector2 v0, Vector2 v1, Vector2 v2, Vector2 v3, uint8_t color);
  void DrawQuad(Quad& quad);
  void DrawTriangleFill(Vector2 v0, Vector2 v1, Vector2 v2, uint8_t color);
  void DrawTriangleFill(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t color);
  void DrawCircle(int16_t xc, int16_t yc, int16_t inner, int16_t outer, uint8_t color, int8_t quadrant = 0);
  void DrawCircleFilled(int16_t xc, int16_t yc, int16_t radius, uint8_t color, int8_t quadrant = 0) {DrawCircle(xc, yc, 0, radius, color, quadrant);}

  //text
  int16_t DrawChar(int16_t x, int16_t y, char c, int8_t font, uint8_t color, bool isCenterWidth = false, bool isCenterHeight = false);
  void DrawCharString(int16_t x, int16_t y, char* cList, int8_t font, uint8_t color, bool isCenterWidth = false, bool isCenterHeight = false);
  void DrawString(int16_t x, int16_t y, const String &value, int8_t font, uint8_t color, bool isCenterWidth = false, bool isCenterHeight = false);

  //numbers
  template <typename T> void DrawInt(int16_t x, int16_t y, T number, int8_t font, uint8_t color, bool isCenterWidth = false, bool isCenterHeight = false, char trailingChar = false);

  void DrawFloat(int16_t x, int16_t y, float number, int8_t font, uint8_t color, uint8_t digits = 0, bool isCenterWidth = false, bool isCenterHeight = false, char trailingChar = ' ');
  void DrawDouble(int16_t x, int16_t y, double number, int8_t font, uint8_t color, uint8_t digits = 0, bool isCenterWidth = false, bool isCenterHeight = false, char trailingChar = ' ');

  //Utility
  int16_t GetHalfCharWidth(char c, int8_t font);
  int16_t GetHalfCharStringWidth(char* cList, int8_t font);
  int16_t GetHalfStringWidth(String value, int8_t font);
  int16_t GetHalfFontHeight(int8_t font);

  void EnableClipping(int16_t clippingLeft = 0, int16_t clippingTop = 0, int16_t clippingRight = 800, int16_t clippingBottom = 480);
  void DisableClipping();
  inline bool InClippingBounds(int16_t x, int16_t y) { return x >= clippingLeft && x < clippingRight && y >= clippingTop && y < clippingBottom; }

public:
  void ClearBuffer();
  void OutputToScreen();
  
};

template <typename T>
void Display::DrawInt(int16_t x, int16_t y, T number, int8_t font, uint8_t color, bool isCenterWidth, bool isCenterHeight, char trailingChar)
{
  String value = String(number);
  if (trailingChar != ' ')
  {
    value += trailingChar;
  }
  DrawString(x, y, value, font, color, isCenterWidth, isCenterHeight);
}

#endif