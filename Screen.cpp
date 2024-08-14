#include "Screen.h"
#include "UiPanelWorld.h"

#define UIPWORLD 0


void Screen::Init(UiInput *uiInput, Display *display, FrameTime *frameTime)
{
  this->display = display;
  this->frameTime = frameTime;
  uiPanelList[UIPWORLD] = new UiPanelWorld(uiInput, display, frameTime);

  for(int i = 0; i < uiPanelCount; i++)
  {
    uiPanelList[i]->Init();
  }
  uiPanelList[UIPWORLD]->isActive = true;

}

void Screen::Update()
{
  for(int i = 0; i < uiPanelCount; i++)
  {
    if(uiPanelList[i]->isActive)
    {
      uiPanelList[i]->Update();
    }
  }

}


void Screen::Draw()
{
  for(int i = 0; i < uiPanelCount; i++)
  {
    if(uiPanelList[i]->isActive)
    {
      display->DisableClipping();
      uiPanelList[i]->Draw();
    }
  }

  int x = 300;
  int xx = 360;
  int y = 20;
  display->DrawString(x, y, "Total", FONT7, CBLACK);
  display->DrawInt(xx, y, frameTime->GetFrameTimeMicros(FTIME_TOTAL), FONT7, CBLACK);
  y += 17;
  display->DrawString(x, y, "Vertice U", FONT7, CBLACK);
  display->DrawInt(xx, y, frameTime->GetFrameTimeMicros(FTIME_3DVERTICE), FONT7, CBLACK);
  y += 17;
  display->DrawString(x, y, "Vertice D", FONT7, CBLACK);
  display->DrawInt(xx, y, frameTime->GetFrameTimeMicros(FTIME_3DDRAW), FONT7, CBLACK);
  y += 17;
  display->DrawString(x, y, "Test", FONT7, CBLACK);
  display->DrawInt(xx, y, frameTime->GetFrameTimeMicros(FTIME_TEST), FONT7, CBLACK);
  y += 17;
  display->DrawString(x, y, "Out Scr", FONT7, CBLACK);
  display->DrawInt(xx, y, frameTime->GetFrameTimeMicros(FTIME_OUTPUTSCREEN), FONT7, CBLACK);


}