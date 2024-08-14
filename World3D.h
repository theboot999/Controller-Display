#ifndef World_h3D
#define World_h

#include "Utility.h"
#include "Display.h"
#include "UiInput.h"
#include "Camera3D.h"
#include "FrameTime.h"

class World3D
{
private:
  UiInput *uiInput;
  FrameTime *frameTime;
  Camera3D camera3D;
  int16_t verticeListCount = 0;
  int16_t quadListCount = 0;
  Vertice *verticeList = nullptr;
  Quad *quadList = nullptr;
  Viewport viewport;
  bool isTouchingWorld = false;
  Vector3 worldTouchPoint;

public:
  float addPitch = 0;

private:
  void Set();
  void SetTest();
  void SortQuads(int16_t low, int16_t high);
  void UpdateCameraTouch();

public:
  void Init(UiInput *uiInput, FrameTime *frameTime);
  void Cleanup();

  void Draw(Display *display); 
  void Update();

};


#endif