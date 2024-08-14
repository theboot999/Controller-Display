#ifndef UiPanelWorld_h
#define UiPanelWorld_h

#include "UiPanel.h"
#include "Display.h"
#include "World3D.h"



class UiPanelWorld : public UiPanel
{
  World3D world3D;

public:
  UiPanelWorld(UiInput *uiInput, Display *display, FrameTime *frameTime) : UiPanel(uiInput, display, frameTime) {}
  void Init();
  void ResetZoom();
  void Update();
  void DrawCalibration();
  void Draw(); 
};

#endif