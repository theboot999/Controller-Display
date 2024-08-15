

#include "Display.h"
#include "Glyphs.h"

void Display::Init(UiInput *uiInput)
{
  this->uiInput = uiInput; 

  graphicsDevice.Init();
  ClearBuffer();
  fontList[0] = aFont7;
  fontList[1] = aFont7B;
  fontList[2] = aFont8;
  fontList[3] = aFont8B;
  fontList[4] = aFont9;
  fontList[5] = aFont9B;
  fontList[6] = aFont10;
  fontList[7] = aFont10B;
  fontList[8] = aFont12;
  fontList[9] = aFont12B;
  fontList[10] = aFont14;
  fontList[11] = aFont14B;
  fontList[12] = aFont20;
  fontList[13] = aFont20B;
  fontList[14] = aFont30;
  fontList[15] = aFont30B;
}

void Display::DrawVLine(int16_t x, int16_t y, int16_t length, uint8_t color)
{
  if(length < 1) {return; }

  if(isClipping)
  {
    if(x < clippingLeft || x >= clippingRight)
    {
      return;
    }
    if(y < clippingTop)
    {
      length -= (clippingTop - y);
      y = clippingTop;
    }
    if(y + length > clippingBottom)
    {
      length = clippingBottom - y;
    }
    if(length < 1) {return; }
  }

  for(int16_t row = y; row < y + length; row++)
  {
    int32_t index = row * dWidth + x;
    buffer[index] = color;
  }
}

void Display::DrawVLine(int16_t x, int16_t y, int16_t length, int16_t thickness, uint8_t color)
{
  if(length < 1) {return; }

  if(isClipping)
  {
    if(x + thickness < clippingLeft)
    {
      return;
    }
    if(x > clippingRight)
    {
      return;
    }
    if(x < clippingLeft)
    {
      thickness -= (clippingLeft - x);
      x = clippingLeft;
    }
    if(x + thickness > clippingRight)
    {
      thickness = clippingRight - x;
    }

    if(y < clippingTop)
    {
      length -= (clippingTop - y);
      y = clippingTop;
    }
    if(y + length > clippingBottom)
    {
      length = clippingBottom - y;
    }
    if(length < 1) {return; }
  }

  for(int16_t column = x; column < x + thickness; column++)
  {
    for(int16_t row = y; row < y + length; row++)
    {
      int32_t index = row * dWidth + column;
      buffer[index] = color;
    }
  }

}


void Display::DrawHLine(int16_t x, int16_t y, int16_t length, uint8_t color)
{
  if(length < 1) {return; }

  if(isClipping)
  {
    if(y < clippingTop || y >= clippingBottom)
    {
      return;
    }

    if(x < clippingLeft)
    {
      length -= (clippingLeft - x);
      x = clippingLeft;
    }
    if(x + length > clippingRight)
    {
      length = clippingRight - x;
    }
    if(length < 1) {return; }
  }

  int32_t startIndex = y * dWidth + x;
  memset(buffer + startIndex, color, length);
}

void Display::DrawHLineF(float xf, int16_t y, float xf2, uint8_t color)
{
  int16_t x = (int16_t)xf;
  int16_t length = (int16_t)xf2 - x;
  if(length < 1) {return; }

  if(isClipping)
  {
    if(y < clippingTop || y >= clippingBottom)
    {
      return;
    }

    if(x < clippingLeft)
    {
      length -= (clippingLeft - x);
      x = clippingLeft;
    }
    if(x + length > clippingRight)
    {
      length = clippingRight - x;
    }
    if(length < 1) {return; }
  }

  int32_t startIndex = y * dWidth + x;
  memset(buffer + startIndex, color, length);
}

void Display::DrawHLine(int16_t x, int16_t y, int16_t length, int16_t thickness, uint8_t color)
{
  if(length < 1) {return; }

  if(isClipping)
  {
    if(y + thickness < clippingTop)
    {
      return;
    }
    if(y > clippingBottom)
    {
      return;
    }

    if(y < clippingTop)
    {
      thickness -= (clippingTop - y);
      y = clippingTop;
    }
    if(y + thickness > clippingBottom)
    {
      thickness = clippingBottom - y;
    }

    if(x < clippingLeft)
    {
      length -= (clippingLeft - x);
      x = clippingLeft;
    }
    if(x + length > clippingRight)
    {
      length = clippingRight - x;
    }
    if(length < 1) {return; }
  }
  
  for(int16_t row = y; row < y + thickness; row++)
  {
    int32_t startIndex = row * dWidth + x;
    memset(buffer + startIndex, color, length);
  }

}

void Display::DrawRectangle(int16_t x, int16_t y, int16_t width, int16_t height, int16_t thickness, uint8_t color)
{
    DrawHLine(x, y, width, thickness, color);
    DrawHLine(x, y + height - thickness, width, thickness, color);
    DrawVLine(x, y, height, thickness, color);
    DrawVLine(x + width - thickness, y, height, thickness, color);
}

void Display::DrawRectangleFill(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t color)
{
  DrawHLine(x, y, width, height, color);
}

void Display::DrawRectangleFillBorder(int16_t x, int16_t y, int16_t width, int16_t height, int16_t borderThickness, uint8_t colorBorder, uint8_t colorBackground)
{
  int16_t border = borderThickness + borderThickness;
  DrawRectangle(x, y, width, height, borderThickness, colorBorder);
  DrawRectangleFill(x + borderThickness, y + borderThickness, width - border, height - border, colorBackground);
}

//TODO: NEED TO ADD SOME CLIPPING CHECKING AND CLIP WHAT WE CAN
void Display::DrawRoundedRectangle(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius, int16_t thickness, uint8_t color)
{
  DrawHLine(x + radius , y, width - radius - radius, thickness, color);   
  DrawHLine(x + radius , y + height - thickness, width - radius - radius, thickness, color);
  DrawVLine(x, y + radius, height - radius - radius, thickness, color);
  DrawVLine(x + width - thickness, y + radius, height - radius - radius, thickness, color);   

  DrawCircle(x + width - radius - 1, y + radius, radius - thickness + 1, radius, color, 1);//top right
  DrawCircle(x + width - radius - 1, y + height - radius - 1, radius - thickness + 1, radius, color, 2);//bot right
  DrawCircle(x + radius, y + height - radius - 1, radius - thickness + 1, radius, color, 3);//bot left
  DrawCircle(x + radius, y + radius, radius - thickness + 1, radius, color, 4);//top left
}

void Display::DrawRoundedRectangleFillBorder(int16_t x, int16_t y, int16_t width, int16_t height, int16_t radius, int16_t borderThickness, uint8_t colorBorder, uint8_t colorBackground)
{

  DrawCircle(x + width - radius - 1, y + radius, 0, radius-borderThickness + 1, colorBackground, 1);//top right
  DrawCircle(x + width - radius - 1, y + height - radius - 1, 0,  radius-borderThickness + 1, colorBackground, 2);//bot right
  DrawCircle(x + radius, y + height - radius - 1, 0, radius-borderThickness + 1, colorBackground, 3);//bot left
  DrawCircle(x + radius, y + radius, 0, radius-borderThickness + 1, colorBackground, 4);//top left

  DrawRoundedRectangle(x, y, width, height, radius, borderThickness, colorBorder);
  DrawRectangleFill(x + borderThickness, y + radius,radius - borderThickness, height - radius - radius, colorBackground); //left
  DrawRectangleFill(x + radius, y + borderThickness, width - radius - radius, height - borderThickness - borderThickness, colorBackground);   //middle
  DrawRectangleFill(x + width - radius, y + radius, radius - borderThickness, height - radius - radius, colorBackground); //left
}

void Display::DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
  if(isClipping)
  {
    if(x1 > clippingRight && x2 > clippingRight) {return; }
    if(x1 < clippingLeft && x2 < clippingLeft) {return; }

    if(y1 > clippingBottom && y2 > clippingBottom) {return; }
    if(y1 < clippingTop && y2 < clippingTop) {return; }
  }

  int16_t slope = abs(y2 - y1) > abs(x2 - x1);

  if (slope) 
  {
    int16_t temp = x1;  x1 = y1; y1 = temp;
    temp = x2;  x2 = y2; y2 = temp;
  }

  if (x1 > x2) 
  {
    int16_t temp = x1;  x1 = x2; x2 = temp;
    temp = y1;  y1 = y2; y2 = temp;
  }

  int16_t dx, dy;
  dx = x2 - x1;
  dy = abs(y2 - y1);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y1 < y2) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x1 <= x2; x1++) 
  {
    if(isClipping)
    {
      if (slope) 
      {
        DrawPixel(y1, x1, color);
      } 
      else 
      {
        DrawPixel(x1, y1, color);
      }
    }
    else
    {
      if (slope) 
      {
        DrawPixelFast(y1, x1, color);
      } 
      else 
      {
        DrawPixelFast(x1, y1, color);
      }      
    }

    err -= dy;
    if (err < 0) {
      y1 += ystep;
      err += dx;
    }
  }
}

const float downRight = 0;                 //0.785
const float downLeft = HALF_PI;      //1.57
const float upLeft = PI;             //3.925
const float upRight = PI + HALF_PI;  //4.71

void Display::DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t thickness, uint8_t color)
{
  bool changeX = false;

  float angle = atan2(y2 - y1, x2 - x1) - 0.7853981f;  //Subtract PI quarter to move 45 degrees to be able to use cardinals

  if(angle < 0)
  {
    angle += TWO_PI;
  }
  else if(angle > TWO_PI)
  {
    angle -= TWO_PI;
  }

  if((angle > upLeft && angle < upRight) || (angle > downRight && angle < downLeft))
  {
    changeX = true;
  }

  int16_t startThickness;
  
  if(thickness > 2)
  {
    startThickness = 0 - (thickness / 2);
  }
  else
  {
    startThickness = -1;
  }

  int16_t endThickness = startThickness + thickness;

  for(int16_t offset = startThickness; offset < endThickness; offset++)
  {
    if(changeX)
    {
      DrawLine(x1 + offset, y1, x2 + offset, y2, color);
    }
    else
    {
      DrawLine(x1, y1 + offset, x2, y2 + offset, color);
    }
  }
}


#define GET_DELTAS(v0, v1) (((float)vList[v1]->screenX - (float)vList[v0]->screenX) / ((float)vList[v1]->screenY - (float)vList[v0]->screenY))

void Display::DrawQuad(Quad& quad)
{
  Vertice *vList[4];
  //Fast rotate so first Vertice in order is at the top keeping clockwise drawing
  if ((quad.verticeList[0]->screenY <= quad.verticeList[1]->screenY) && (quad.verticeList[0]->screenY <= quad.verticeList[2]->screenY) && (quad.verticeList[0]->screenY <= quad.verticeList[3]->screenY)) { vList[0] = quad.verticeList[0]; vList[1] = quad.verticeList[1]; vList[2] = quad.verticeList[2]; vList[3] = quad.verticeList[3]; } 
  else if ((quad.verticeList[1]->screenY <= quad.verticeList[0]->screenY) && (quad.verticeList[1]->screenY <= quad.verticeList[2]->screenY) && (quad.verticeList[1]->screenY <= quad.verticeList[3]->screenY)) { vList[0] = quad.verticeList[1]; vList[1] = quad.verticeList[2]; vList[2] = quad.verticeList[3]; vList[3] = quad.verticeList[0]; } 
  else if ((quad.verticeList[2]->screenY <= quad.verticeList[0]->screenY) && (quad.verticeList[2]->screenY <= quad.verticeList[1]->screenY) && (quad.verticeList[2]->screenY <= quad.verticeList[3]->screenY)) { vList[0] = quad.verticeList[2]; vList[1] = quad.verticeList[3]; vList[2] = quad.verticeList[0]; vList[3] = quad.verticeList[1]; } 
  else { vList[0] = quad.verticeList[3]; vList[1] = quad.verticeList[0]; vList[2] = quad.verticeList[1]; vList[3] = quad.verticeList[2];}

  int16_t bottomY = 0;
  
  for(int i = 0; i < 4; i++)
  {
    if(vList[i]->screenY > bottomY)
    {
      bottomY = vList[i]->screenY;
    }
  }

  int8_t color = quad.color;
  int16_t y = vList[0]->screenY;
  int16_t vLeft = 3;
  int16_t vRight = 1;

  float leftX = vList[0]->screenX;
  float rightX = vList[0]->screenX;
  float toAddLeft = GET_DELTAS(0, vLeft);
  float toAddRight = GET_DELTAS(0, vRight);

  //if right on same plane
  if(vList[0]->screenY == vList[1]->screenY)
  {
    rightX = vList[1]->screenX;
    vRight = 2;
    toAddRight = GET_DELTAS(vRight - 1, vRight);
  } 

  //left on same plane
  if(vList[0]->screenY == vList[3]->screenY)
  {
    leftX = vList[3]->screenX;
    vLeft = 2;
    toAddLeft = GET_DELTAS(vLeft + 1, vLeft);
  }

  while(1)
  {
    DrawHLineF(leftX, y, rightX + 1, color);
    y++;

    if(y > bottomY){ break; } //weve hit bottom
    
    leftX += toAddLeft;
    rightX += toAddRight;

    if(y >= vList[vLeft]->screenY)  //hit next left side vertice
    {
      vLeft--;
      if(vLeft > -1)
      {
        toAddLeft = GET_DELTAS(vLeft + 1, vLeft);
      }
    }
    
    if(y >= vList[vRight]->screenY)  //hit next right side vertice
    {
      vRight++;
      Serial.println(vRight);
      if(vRight < 4)
      {
        toAddRight = GET_DELTAS(vRight - 1, vRight);
      }
    }
  }
}

void Display::DrawTriangleFill(Vector2 v0, Vector2 v1, Vector2 v2, uint8_t color)
{
  DrawTriangleFill(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, color);
}

void Display::DrawTriangleFill(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t color) 
{
    int32_t temp;
    int32_t a, b, y, last;

    if (y0 > y1) 
    {
        temp = y0; y0 = y1; y1 = temp;
        temp = x0; x0 = x1; x1 = temp;
    }
    if (y1 > y2) 
    {
        temp = y1; y1 = y2; y2 = temp;
        temp = x1; x1 = x2; x2 = temp;
    }
    if (y0 > y1) 
    {
        temp = y0; y0 = y1; y1 = temp;
        temp = x0; x0 = x1; x1 = temp;
    }

    if (y0 == y2) 
    {
        a = b = x0;
        if (x1 < a) a = x1;
        else if (x1 > b) b = x1;
        if (x2 < a) a = x2;
        else if (x2 > b) b = x2;
        DrawHLine(a, y0, b - a + 1, color);
        return;
    }

    int32_t dx01 = x1 - x0, dy01 = y1 - y0;
    int32_t dx02 = x2 - x0, dy02 = y2 - y0;
    int32_t dx12 = x2 - x1, dy12 = y2 - y1;
    int32_t sa = 0, sb = 0;

    if (y1 == y2) last = y1;
    else last = y1 - 1;

    for (y = y0; y <= last; y++) 
    {
        if (y >= clippingBottom) break;
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;

        if (a > b) 
        {
            temp = a; a = b; b = temp;
        }
        DrawHLine(a, y, b - a + 1, color);
    }

    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);

    for (; y <= y2; y++) 
    {
      if (y >= clippingBottom) break;
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;

        if (a > b) 
        {
            temp = a; a = b; b = temp;
        }
        DrawHLine(a, y, b - a + 1, color);
    }
}

void Display::DrawCircle(int16_t xc, int16_t yc, int16_t inner, int16_t outer, uint8_t color, int8_t quadrant)
{
  int16_t xo = outer;
  int16_t xi = inner;
  int16_t y = 0;
  int16_t erro = 1 - xo;
  int16_t erri = 1 - xi;
 
  if(isClipping)
  {
    if (xc + outer < clippingLeft || xc - outer > clippingRight || yc + outer < clippingTop || yc - outer > clippingBottom)
    {
      return;
    }
  }

  while(xo >= y) 
  {
    if(quadrant == 0)
    {
      DrawHLine(xc + xi, yc - y, xo - xi + 1, color);       //top right quadrant
      DrawVLine(xc + y, yc - xo, xo - xi + 1,  color);   //top right quadrant 
      DrawHLine(xc + xi, yc + y, xo - xi + 1, color);       //bottom right quadrant
      DrawVLine(xc + y, yc + xi, xo - xi + 1,  color);   //bottom right quadrant
      DrawHLine(xc - xo, yc + y, xo - xi + 1, color);       //bottom left quadrant
      DrawVLine(xc - y, yc + xi, xo - xi + 1,  color);   //bottom left quadrant
      DrawHLine(xc - xo, yc - y, xo - xi + 1, color);       //top left quadrant
      DrawVLine(xc - y, yc - xo , xo - xi + 1,  color);  //top left quadrant
    }
    else if(quadrant == 1)
    {
      DrawHLine(xc + xi, yc - y, xo - xi + 1, color);       //top right quadrant
      DrawVLine(xc + y, yc - xo, xo - xi + 1,  color);   //top right quadrant 
    }
    else if(quadrant == 2)
    {
      DrawHLine(xc + xi, yc + y, xo - xi + 1, color);       //bottom right quadrant
      DrawVLine(xc + y, yc + xi, xo - xi + 1,  color);   //bottom right quadrant
    }
    else if(quadrant == 3)
    {
      DrawHLine(xc - xo, yc + y, xo - xi + 1, color);       //bottom left quadrant
      DrawVLine(xc - y, yc + xi, xo - xi + 1,  color);   //bottom left quadrant
    }
    else if(quadrant == 4)
    {
      DrawHLine(xc - xo, yc - y, xo - xi + 1, color);       //top left quadrant
      DrawVLine(xc - y, yc - xo , xo - xi + 1,  color);  //top left quadrant
    }

    y++;

    if (erro < 0) 
    {
      erro += 2 * y + 1;
    } 
    else 
    {
      xo--;
      erro += 2 * (y - xo + 1);
    }

    if (y > inner) 
    {
      xi = y;
    } 
    else 
    {
      if (erri < 0) 
      {
        erri += 2 * y + 1;
      } 
      else 
      {
        xi--;
        erri += 2 * (y - xi + 1);
      }
    }
  }
}

//TODO: Update clipping to be per character. Not per pixe
//Calculate character size at start them clip
int16_t Display::DrawChar(int16_t x, int16_t y, char c, int8_t font, uint8_t color, bool isCenterWidth, bool isCenterHeight)
{
  if(isCenterWidth) { x -= GetHalfCharWidth(c, font); }
  if(isCenterHeight) { y -= GetHalfFontHeight(font); }

  uint8_t charByte = (uint8_t)c;
  const int16_t* glyph = glyphList[(charByte - 32) + (font * 95)];
  const uint8_t* fontL = fontList[font];
  uint16_t bo = glyph[0];
  uint8_t w = glyph[1];
  uint8_t h = glyph[2];
  int8_t xo = glyph[4];
  int8_t yo = glyph[5];
  uint8_t xx, yy, bits = 0, bit = 0;
  int16_t finalX;
  int16_t finalY;

  if(isClipping)
  {
    for (yy = 0; yy < h; yy++) 
    {
      for (xx = 0; xx < w; xx++) 
      {
        if (!(bit++ & 7)) 
        {
          bits = fontL[bo++];
        }
        if (bits & 0x80) 
        {
          finalX = x + xo + xx;
          finalY = y + yo + yy;      
          if(InClippingBounds(finalX, finalY))
          {
            int32_t index = (y + yo + yy) * dWidth + (x + xo + xx);
            buffer[index] = color;
          }
        }
        bits <<= 1;
      }
    }
    return glyph[3];
  }
  else
  {
    for (yy = 0; yy < h; yy++) 
    {
      for (xx = 0; xx < w; xx++) 
      {
        if (!(bit++ & 7)) 
        {
          bits = fontL[bo++];
        }
        if (bits & 0x80) 
        {
          int32_t index = (y + yo + yy) * dWidth + (x + xo + xx);
          buffer[index] = color;
        }
        bits <<= 1;
      }
    }
    return glyph[3];
  }
}

void Display::DrawCharString(int16_t x, int16_t y, char* cList, int8_t font, uint8_t color, bool isCenterWidth, bool isCenterHeight)
{
  int16_t index = 0;

  if(isCenterWidth) { x -= GetHalfCharStringWidth(cList, font); }
  if(isCenterHeight) { y += GetHalfFontHeight(font); }

  while (cList[index] != '\0')
  {
    int16_t offsetX = DrawChar(x, y, cList[index], font, color);
    x += offsetX;
    index++;
  }
}

void Display::DrawString(int16_t x, int16_t y, const String &value, int8_t font, uint8_t color, bool isCenterWidth, bool isCenterHeight)
{
  if(value.length() < 1)
  {
    return;
  }
  int16_t index = 0;

  if(isCenterWidth) { x -= GetHalfStringWidth(value, font); }
  if(isCenterHeight) { y += GetHalfFontHeight(font); }

  while (value[index] != '\0')
  {
    int16_t offsetX = DrawChar(x, y, value[index], font, color);
    x += offsetX;
    index++;
  }
}

void Display::DrawFloat(int16_t x, int16_t y, float number, int8_t font, uint8_t color, uint8_t digits, bool isCenterWidth, bool isCenterHeight, char trailingChar)
{
  memset(charBuffer, 0, sizeof(charBuffer));
  dtostrf(number, 1, digits, charBuffer);

  if(trailingChar != ' ')
  {
    int16_t length = strlen(charBuffer);
    if(length < 23)
    {
      charBuffer[length] = trailingChar;
      charBuffer[length + 1] = '\0';
    }
  }

  DrawCharString(x, y, charBuffer, font, color, isCenterWidth, isCenterHeight);
}

void Display::DrawDouble(int16_t x, int16_t y, double number, int8_t font, uint8_t color, uint8_t digits, bool isCenterWidth, bool isCenterHeight, char trailingChar)
{
  memset(charBuffer, 0, sizeof(charBuffer));
  dtostrf(number, 1, digits, charBuffer);

  if(trailingChar != ' ')
  {
    int16_t length = strlen(charBuffer);
    if(length < 23)
    {
      charBuffer[length] = trailingChar;
      charBuffer[length + 1] = '\0';
    }
  }

  if(isCenterWidth) { x -= GetHalfCharStringWidth(charBuffer, font); }
  if(isCenterHeight) { y += GetHalfFontHeight(font); }

  DrawCharString(x, y, charBuffer, font, color, false, false);
}

int16_t Display::GetHalfCharWidth(char c, int8_t font)
{
  int16_t halfWidth = glyphList[c - 32 + (font * 95)][3];

  if(halfWidth > 1)
  {
    halfWidth /= 2;
  }
  return halfWidth;
}

int16_t Display::GetHalfCharStringWidth(char* cList, int8_t font)
{
  int16_t halfWidth = 0;
  int16_t index = 0;
  while (cList[index] != '\0')
  {
    halfWidth += glyphList[(cList[index] - 32) + (font * 95)][3];
    index++;
  }

  if(halfWidth > 1)
  {
    halfWidth /= 2;
  }
  return halfWidth;
}

int16_t Display::GetHalfStringWidth(String value, int8_t font)
{
  int16_t halfWidth = 0;

  for(uint16_t index = 0; index < value.length(); index++)
  {
    halfWidth += glyphList[(value[index] - 32) + (font * 95)][3];
  }

  if(halfWidth > 1)
  {
    halfWidth /= 2;
  }
  return halfWidth;
}

int16_t Display::GetHalfFontHeight(int8_t font)
{
  return halfFontHeight[font];
}

void Display::EnableClipping(int16_t clippingLeft, int16_t clippingTop, int16_t clippingRight, int16_t clippingBottom)
{
  isClipping = true;
  this->clippingLeft = clippingLeft;
  this->clippingTop = clippingTop;
  this->clippingRight = clippingRight;
  this->clippingBottom = clippingBottom;
}

void Display::DisableClipping()
{
  isClipping = false;
}

void Display::ClearBuffer()
{
	memset(buffer, CBACKGROUND, sizeof(buffer));
}

void Display::OutputToScreen()
{
  graphicsDevice.OutputBuffer(buffer);
}
