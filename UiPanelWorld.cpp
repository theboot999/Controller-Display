#include "UiPanelWorld.h"


void UiPanelWorld::Init()
{
  world3D.Init(uiInput, frameTime);
   UiPanel::AddControl(*new UiButtonText(40, 20, 80, 80, CGREY, CRED, CGREEN, CBLACK, FONT12B, "Pitch +"));
   UiPanel::AddControl(*new UiButtonText(40, 110, 80, 80, CGREY, CRED, CGREEN, CBLACK, FONT12B, "Pitch - "));
}

void UiPanelWorld::Update()
{
  UiPanel::Update();
  
  if(IsControlDown(0))
  {
    world3D.addPitch = -0.1f;
  }
  else if(IsControlDown(1))
  {
    world3D.addPitch = 0.1f;
  }

  world3D.Update();
}

Vertice verticeList[4];
Vector2 vectorList[4];
void UiPanelWorld::Draw()
{ 


  world3D.Draw(display);

  UiPanel::Draw();

/*  vectorList[0].Set(200, 100);
  vectorList[1].Set(250, 150);
  vectorList[2].Set(200, 200);
  vectorList[3].Set(100, 170);
  

  //Axis aligned rectangle
  //verticeList[0].SetScreen(250, 200);
  //verticeList[1].SetScreen(100, 200);
  //verticeList[2].SetScreen(100, 100);
  //verticeList[3].SetScreen(250, 100);

  //Diamond
  verticeList[3].SetScreen(200, 100);
  verticeList[0].SetScreen(250, 150);
  verticeList[1].SetScreen(200, 200);
  verticeList[2].SetScreen(100, 170);
//ARM_DWT_CYCCNT;
  Quad quad;
  quad.Set(&verticeList[0], &verticeList[1], &verticeList[2], &verticeList[3], CGREY, L1111);
  //Serial.println(after - before);
  //  uint32_t after = ARM_DWT_CYCCNT;
  uint32_t before = micros();



  for(int i = 0; i < 100; i++)
  {
    display->DrawQuad(quad);
 // display->DrawQuadFill(vectorList[0], vectorList[1], vectorList[2], vectorList[3], CGREY);
  }

  uint32_t after = micros();
  Serial.println(after - before);

*/



 /* int16_t y = 290;
  int16_t vGap = 16;
  int16_t x1 = 400;
  int16_t x2 = 500;
  int16_t font1 = FONT8;
  int16_t font2 = FONT8;
  int16_t color1 = CBLACK;
  int16_t color2 = CBLACK;



  display->DrawString(x1,y, "Touch 1 X:", font1, color1);  
  display->DrawInt(x2,y, display->uiInput->GetTouchOneX(), font2, color2);
  y+=vGap;
  display->DrawString(x1,y, "Touch 1 Y:", font1, color1);  
  display->DrawInt(x2,y, display->uiInput->GetTouchOneY(), font2, color2);
  y+=vGap;
  display->DrawString(x1,y, "Touch 2 X:", font1, color1);  
  display->DrawInt(x2,y, display->uiInput->GetTouchTwoX(), font2, color2);
  y+=vGap;
  display->DrawString(x1,y, "Touch 2 Y:", font1, color1);  
  display->DrawInt(x2,y, display->uiInput->GetTouchTwoY(), font2, color2);
  y+=vGap;
  display->DrawString(x1,y, "Gesture:", font1, color1);  
  display->DrawInt(x2,y, display->uiInput->GetGesture(), font2, color2);*/
}