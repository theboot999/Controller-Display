#include "UiInput.h"
#include "Wire.h"

void UiInput::Init()
{
  //Setting up FT5206
  Wire2.begin();
  Wire2.setClock(400000UL); // Set I2C frequency to 400kHz
  Wire2.beginTransmission(FT5206_I2C_ADDRESS);
  Wire2.write(FT5206_DEVICE_MODE);
  Wire2.write(0);
  Wire2.endTransmission(FT5206_I2C_ADDRESS);
}

void UiInput::UpdateTouch()
{
  Wire2.beginTransmission(FT5206_I2C_ADDRESS);

  Wire2.requestFrom(FT5206_I2C_ADDRESS, FT5206_REGISTERS); 
  

  for(int i = 0; i < FT5206_REGISTERS; i++)
  {
    registers[i] = Wire2.read();
  }

 // Wire2.endTransmission(FT5206_I2C_ADDRESS);

  oldTouches = currentTouches;
	currentTouches = registers[FT5206_TD_STATUS] & 0xF;

  if(currentTouches > 1) {currentTouches = 2;}

  if(currentTouches > 0)
  {
    touch1.SetCurrent(800 - (((registers[0x03] & 0x0f) << 8) | registers[0x04]), 480 - (((registers[0x05] & 0x0f) << 8) | registers[0x06]));
  }
  else
  {
    if(touch1.isTouching)
    {
      touch1.MakeClicked();
    }
    else
    {
      touch1.Reset();
    }
  }
  
  if(currentTouches > 1)
  {
    touch2.SetCurrent(800 - (((registers[0x09] & 0x0f) << 8) | registers[0x0A]), 480 - (((registers[0x0B] & 0x0f) << 8) | registers[0x0C]));
  }
  else
  {
    if(touch2.isTouching)
    {
      touch2.MakeClicked();
    }
    else
    {
      touch2.Reset();
    }
  }
}

//TODO: Still one bug
//BOTH FINGERS DOWN. ZOOM OR SPIN.  LEFT LAST FINGER UP AND PUT IT DOWN AGAIN AND WE REVERSE OUR ZOOM/SPIN
//OKAY SO WHEN WE LIFT OUR FINGER IT REVERSE TOUCH NUMBER SO OUR DISTANCE CALCULATION GETS REVERSED
void UiInput::UpdateGesture()
{
  if(oldTouches == 2 && currentTouches == 1) //We lifted a finger off and touch2 could become touch1
  {
    gesture = GESTURE_NO;
  }
  else if(currentTouches == 0)
  {
    gesture = GESTURE_NO;
  }
  else if(currentTouches == 1)
  {
    if(!touch2.isClicked && touch1.IsPan())
    {
      gesture = GESTURE_PANONE;
    }
  }
  else if(currentTouches == 2)
  {
    if(!touch1.firstTouch && !touch2.firstTouch)
    {
      if(touch1.hasMoved || touch2.hasMoved)
      {
        int16_t xChange = abs(touch1.xChange) + abs(touch2.xChange);
        int16_t yChange = abs(touch1.yChange) + abs(touch2.yChange);

        if(xChange > yChange)
        {
          gesture = GESTURE_ZOOM;
        }
        else
        {
          gesture = GESTURE_SPIN;
        }
      }
    }
  }
}

int8_t UiInput::GetGesture()
{
  return gesture;
}

int16_t UiInput::GetTouchOneX()
{
  return touch1.currentX;
}

int16_t UiInput::GetTouchOneY()
{
  return touch1.currentY;
}

bool UiInput::IsTouchingOne()
{
  return touch1.isTouching;
}

bool UiInput::IsClickedTwo()
{
    return touch2.isClicked;
}

bool UiInput::IsClickedOne()
{
    return touch1.isClicked;
}

bool UiInput::IsTouchOneInBounds(int16_t left, int16_t top, int16_t right, int16_t bottom)
{
  if(!IsTouchingOne()) {return false;}
  if(touch1.currentX < left) {return false;}
  if(touch1.currentY < top) {return false;}
  if(touch1.currentX > right) {return false;}
  if(touch1.currentY > bottom) {return true;}
  return true;
}

Vector2 UiInput::GetTouchOne()
{
  return Vector2(touch1.currentX, touch1.currentY);
}

int16_t UiInput::GetTouchTwoX()
{
 return touch2.currentX;
}

int16_t UiInput::GetTouchTwoY()
{
  return touch2.currentY;
}

bool UiInput::IsTouchingTwo()
{
  return touch2.isTouching;
}

void UiInput::GetPanAngleOneDistance(float &angle, float &distance)
{
  touch1.GetPanAngleDistance(angle, distance);
}

void UiInput::GetPanAngleTwoDistance(float &angle, float &distance)
{
  touch2.GetPanAngleDistance(angle, distance);
}

void UiInput::GetPanXTwoDistance(float &distance)
{
  touch2.GetPanXDistance(distance);
}

double UiInput::GetZoomDistance()
{
  int16_t leftChange;
  int16_t rightChange;

  if(touch1.currentX < touch2.currentX)
  {
    leftChange = touch1.oldX - touch1.currentX;
    rightChange = touch2.currentX - touch2.oldX;
  }
  else
  {
    leftChange = touch2.oldX - touch2.currentX;
    rightChange = touch1.currentX - touch1.oldX;
  }
  return leftChange + rightChange;
}

double UiInput::GetSpinDistance()
{
  //need to get left y first
  int16_t leftChange;
  int16_t rightChange;

  if(touch1.currentX < touch2.currentX)
  {
    leftChange = touch1.currentY - touch1.oldY;
    rightChange = touch2.oldY - touch2.currentY;

  }
  else
  {
    leftChange = touch2.currentY - touch2.oldY;
    rightChange = touch1.oldY - touch1.currentY;
  }

  return leftChange + rightChange;
}

void UiInput::Update()
{  
  UpdateTouch();

  UpdateGesture();
}