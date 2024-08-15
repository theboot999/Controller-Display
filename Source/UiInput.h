#ifndef UiInput_h
#define UiInput_h

#include "Arduino.h"
#include "Vectors.h"

#define GESTURE_NO        0
#define GESTURE_ZOOM      1
#define GESTURE_PANONE    2
#define GESTURE_SPIN      3

#define FT5206_I2C_ADDRESS 			0x38
#define FT5206_DEVICE_MODE 			0x00
#define FT5206_REGISTERS 	      13
#define FT5206_TD_STATUS 		  	0x02
#define FT5206_GEST_ID 				  0x01

struct TouchPoint
{
  int16_t currentX;
  int16_t currentY;
  int16_t oldX;
  int16_t oldY;
  bool isTouching;
  bool isClicked;
  bool firstTouch;
  bool hasMoved;
  int16_t xChange;
  int16_t yChange;


  void SetCurrent(int16_t x, int16_t y)
  {
    if(x > -1 && x < 800 && y > -1 && y < 480)
    {
      oldX = currentX;
      currentX = x;
      oldY = currentY;
      currentY = y;
      if(!isTouching)
      {
        firstTouch = true;
      }
      else
      {
        firstTouch = false;
      }

      isTouching = true;
    }

    if(oldX != currentX || oldY != currentY)
    {
      hasMoved = true;
    }
    else
    {
      hasMoved = false;
    }

    xChange = currentX - oldX;
    yChange = currentY - oldY;
  }

  void MakeClicked()
  {
    currentX = 0;
    currentY = 0;
    oldX = 0;
    oldY = 0;
    isTouching = false;
    firstTouch = false;
    isClicked = true;
  }

  void Reset()
  {
    currentX = 0;
    currentY = 0;
    oldX = 0;
    oldY = 0;
    isTouching = false;
    isClicked = false;
    firstTouch = false;
  }

  bool HasVerticalChanged()
  {
     return currentY != oldY;
  }

  bool IsPan()
  {
    if(isTouching && !firstTouch)
    {
      if(currentX != oldX || currentY != oldY)
      {
        return true;
      }
    }
    return false;
  }
  
  void GetPanXDistance(float &distance)
  {
    if(oldX == 0)
    {
      distance = 0;
      return;
    }
    distance = currentX - oldX;
  }

  void GetPanYDistance(float &distance)
  {
    if(oldY == 0)
    {
      distance = 0;
      return;
    }
    distance = currentY - oldY;
  }


  void GetPanAngleDistance(float &angle, float &distance)
  {
    if(oldX == 0 || oldY == 0)
    {
      distance = 0;
      return;
    }

    int16_t deltaX = currentX - oldX;
    int16_t deltaY = currentY - oldY;
    distance = sqrt((float)(deltaX * deltaX + deltaY * deltaY));

    angle = atan2((float)(-deltaY), (float)(deltaX));
  }
};

class UiInput
{
private:
//touch
  TouchPoint touch1;
  TouchPoint touch2;

  uint8_t registers[FT5206_REGISTERS];
  int8_t gesture = 0;

public:
  uint8_t currentTouches = 0;
  uint8_t oldTouches = 0;
  uint16_t coordinates[5][2]; //TODO: Make this private with methods later
  void Init();
  void UpdateTouch();
  void UpdateGesture();
  void Update();
  int16_t GetTouchOneX();
  int16_t GetTouchOneY();
  bool IsTouchingOne();
  bool IsClickedOne();
  bool IsTouchOneInBounds(int16_t left, int16_t top, int16_t right, int16_t bottom);
  Vector2 GetTouchOne();
  int16_t GetTouchTwoX();
  int16_t GetTouchTwoY();
  bool IsTouchingTwo();
  bool IsClickedTwo();
  int8_t GetGesture();
  void GetPanAngleOneDistance(float &angle, float &distance);
  void GetPanAngleTwoDistance(float &angle, float &distance);
  void GetPanXTwoDistance(float &distance);
  double GetZoomDistance();
  double GetSpinDistance();
};

#endif