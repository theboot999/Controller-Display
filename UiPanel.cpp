#include "UiPanel.h"

void UiPanel::AddControl(UiControl &uiControl)
{
  uiControlList[controlCount] = &uiControl;
  controlCount++;
}

bool UiPanel::IsControlClickOrTick(int index)
{
  if(uiControlList[index]->isClicked)
  {
    return true;   
  }
  if(uiControlList[index]->isHoldTick)
  {
    return true;   
  }

  return false;
}

void UiPanel::SetText(int index, const String &text)
{
  uiControlList[index]->SetText(text);
}

bool UiPanel::IsControlClick(int index)
{
  return uiControlList[index]->isClicked;
}

void UiPanel::Update()
{
  for (int i = 0; i < controlCount; i++)
  {
    uiControlList[i]->Update(uiInput);
  }
}
   
void UiPanel::Draw()
{
  for (int i = 0; i < controlCount; i++)
  {
    uiControlList[i]->Draw(display);
  }
}