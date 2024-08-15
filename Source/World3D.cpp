#include "World3D.h"
//Build the Walls.  AddWall.  Need to go left to right to generate correct face? Single thickness walls
//Will generate the 4 vertices and add them to main vertice list.  Will first check if they are in there
//Also pass in the height and the colour.  Will also generate the triangles (Or quads...)
//Can do shorter height ones for like a table
//AddFloor
//Adds A Floor.  It will generate the 4 vertices (And check list for them already so dont double up) pass in the colour. Add in a height
//Lets not worry about blocking floor or unblocking.  its only visual

//Pathfinding.  Select a spot to start to build nodes.  Branch out addings nodes to the list until we have every node?  have to think about this more because are we storing them in an array at run time? more to come
//Vehicel can probably take up multipel spots of the node

//Heirachy
//World3D.  contains world and walls and floor and pathfinding.  Need to be able to load a different world?  
//We also have models we add into the world

//In terms of draw order.  as its almost an isometric style drawing can we simply draw
//Ground layer
//Wall layer
//Robot layer?
//Be faster than sorting?

//I think for up and down pitching we could use a up and down slider with a circle in it
//for spin and zoom we could use 2 fingers or have a little wheel
//for movement just pan around with a finger held down i think
//thats intuitive
//i also liked the smooth movement. so double click a robot or somewhere and it automatically pans smoothly over there

//ALSO NEED TO DO SOME SCREENSPACE CULLING
//IF NO VERTICES ARE IN SCREEN ON A QUAD> DONT DRAW

void World3D::Init(UiInput *uiInput, FrameTime *frameTime)
{
  this->uiInput = uiInput;
  this->frameTime = frameTime;

 //SetTest();
  Set();
  viewport = Viewport(0, 800, 0, 480);
  camera3D.Init(&viewport);
  Vector3 newLookAtPoint(0, 100, 0); //100 above ground
  //Vector3 newLookAtPoint(-100, 0, 200); //100 above ground
//Vector3 newLookAtPoint(965, 100, 239); //100 above ground
//Vector3 newLookAtPoint(6.09872f, 0, 213.71153f); //100 above ground
//Vector3 newLookAtPoint(-80.59827f, 0, 106.97678); //100 above ground
  camera3D.CenterOn(newLookAtPoint, 1600, 3.092, -0.5);

  
}

void World3D::SetTest()
{
  verticeListCount = 6;
  quadListCount = 2;
  verticeList = new Vertice[verticeListCount];  // Array of Vertice objects
  quadList = new Quad[quadListCount];           // Array of Quad objects

  verticeList[0].Set(0, 0, 0);
  verticeList[1].Set(0, 0, 200);
  verticeList[2].Set(100, 0, 250);
  verticeList[3].Set(100, 0, 0);
  verticeList[4].Set(0, 50, 0);
  verticeList[5].Set(0, 50, 200);

  quadList[0].Set(&verticeList[0], &verticeList[1], &verticeList[2], &verticeList[3], CGREY, L1111);
  quadList[1].Set(&verticeList[4], &verticeList[5], &verticeList[1], &verticeList[0], CGREY, L1111);

  /*
  verticeList[0].Set(0, 150, 0);
  verticeList[1].Set(100, 150, 0);
  verticeList[2].Set(100, 0, 0);
  verticeList[3].Set(0, 0, 0);
  verticeList[4].Set(0, 150, 200);
  verticeList[5].Set(100, 150, 200);
  verticeList[6].Set(100, 0, 200);
  verticeList[7].Set(0, 0, 200);

  quadList[0].Set(&verticeList[0], &verticeList[1], &verticeList[2], &verticeList[3], CGREY, L1111);
  quadList[1].Set(&verticeList[1], &verticeList[5], &verticeList[6], &verticeList[2], CGREY, L1111);
  quadList[2].Set(&verticeList[5], &verticeList[4], &verticeList[7], &verticeList[6], CGREY, L1111);
  quadList[3].Set(&verticeList[4], &verticeList[0], &verticeList[3], &verticeList[7], CGREY, L1111);
  quadList[4].Set(&verticeList[3], &verticeList[2], &verticeList[6], &verticeList[7], CGREY, L1111);
  quadList[5].Set(&verticeList[0], &verticeList[4], &verticeList[5], &verticeList[1], CGREY, L1111);*/

}

void World3D::Set()
{
  int16_t tileSize = 100;
  int16_t width = 20;
  int16_t height = 20;

  verticeListCount = (width + 1)  * (height + 2);
  quadListCount = width * height;
  verticeList = new Vertice[verticeListCount];  // Array of Vertice objects
  quadList = new Quad[quadListCount];           // Array of Quad objects


  float y = 0;

  int16_t index = 0;

  for(int z = 0; z < height + 2; z++)
  {
    for(int x = 0; x < width + 1; x++)
    {
      float outX = x * (float)tileSize;
      float outZ = z * (float)tileSize;
      verticeList[index].Set(outX, y, outZ);
      index++;

      
    }
  }

  int8_t color = 4;
  index = 0;
  int16_t verticePerLine = width + 1;

  for(int y = 0; y < height; y++)
  {
    
    for (int x = 0; x < width; x++)
    {
      int p1 = x + verticePerLine * y;
      int p2 = x + 1 + verticePerLine * y;
      int p3 = x + 1 + verticePerLine * (y + 1);
      int p4 = x + verticePerLine * (y + 1);

      quadList[index].Set(&verticeList[p1], &verticeList[p2], &verticeList[p3], &verticeList[p4], color, L1111);
      if(color == 4)
      {
        color = 6;
      }
      else
      {
        color = 4;
      }
      index++;
      
    }
  }


}


void World3D::Cleanup()
{
  delete[] verticeList;
  delete[] quadList;
  verticeList = nullptr;
  quadList = nullptr;
}
//recurisve is bvad. we will do linked list soon
void World3D::SortQuads(int16_t low, int16_t high) 
{
  if (low < high) 
  {
    int pivotIndex = low;
    if(high > 1)
    {
      pivotIndex = (low + high) / 2;
    }
 
    float pivotValue = quadList[pivotIndex].worldZ;
        
    // Partition the array into two segments
    int i = low;
    int j = high;
        
    while (i < j) 
    {
      while (quadList[i].worldZ >= pivotValue && i < high)
      {
        i++;
      }
      while (quadList[j].worldZ < pivotValue)
      {
        j--;
      }
      if (i < j) 
      {
        // Swap quadList[i] and quadList[j]
        Quad temp = quadList[i];
        quadList[i] = quadList[j];
        quadList[j] = temp;
      }
    }
        
    // Swap the pivot element with quadList[j]
    Quad temp = quadList[pivotIndex];
    quadList[pivotIndex] = quadList[j];
    quadList[j] = temp;
        
    // Recursively sort the sub-arrays
    SortQuads(low, j - 1);
    SortQuads(j + 1, high);
  }
};

void World3D::UpdateCameraTouch()
{
  isTouchingWorld = false;

  if(uiInput->IsTouchOneInBounds(viewport.left, viewport.top, viewport.right, viewport.bottom))
  {
    if(addPitch != 0)
    {
      camera3D.AddPitch(addPitch);
    }
    addPitch = 0;
    if(uiInput->GetGesture() == GESTURE_NO)
    {
      return;
    }
    else if(uiInput->GetGesture() == GESTURE_PANONE)
    {
      float angle;
      float delta;
      uiInput->GetPanAngleOneDistance(angle, delta); 
      camera3D.AddPan(angle, delta * 1.5f);
    }
    else if(uiInput->GetGesture() == GESTURE_SPIN)
    {
      float delta = uiInput->GetSpinDistance() / 30;
      camera3D.AddYaw(-delta);
    }
    else if(uiInput->GetGesture() == GESTURE_ZOOM)
    {
      float delta = uiInput->GetZoomDistance() * 15;
      camera3D.AddZoom(-delta);
    }
  }
}

void World3D::Update()
{
  UpdateCameraTouch();

  frameTime->Start(FTIME_3DVERTICE);
  camera3D.Update();
  camera3D.TransformVerticeList(verticeList, verticeListCount);

 
  for(int i = 0; i < quadListCount; i++)
  {
    quadList[i].UpdateZ(&viewport);
  }

 // SortQuads(0, quadListCount - 1);

  frameTime->Update(FTIME_3DVERTICE);


  if(uiInput->IsTouchOneInBounds(viewport.left, viewport.top, viewport.right, viewport.bottom))
  {
      Vector2 screenPoint = uiInput->GetTouchOne();
      isTouchingWorld = camera3D.ScreenToWorld(screenPoint, worldTouchPoint);
  }

}  


void World3D::Draw(Display *display)
{
  int16_t quadsDrawn = 0;

  frameTime->Start(FTIME_3DDRAW);
  display->EnableClipping(viewport.left, viewport.top, viewport.right, viewport.bottom);

  for(int i = 0; i < quadListCount; i++)
  {  
    if(!quadList[i].inScreenSpace) { continue; }
    Vertice *vertice0 = quadList[i].verticeList[0];
    Vertice *vertice1 = quadList[i].verticeList[1];
    Vertice *vertice2 = quadList[i].verticeList[2];
    Vertice *vertice3 = quadList[i].verticeList[3];

    int32_t abx = vertice1->screenX - vertice0->screenX;
    int32_t aby = vertice1->screenY - vertice0->screenY;
    int32_t acx = vertice2->screenX - vertice0->screenX;
    int32_t acy = vertice2->screenY - vertice0->screenY;
  
   if(abx * acy - acx * aby > 0)  //Reverse culling
    {
      quadsDrawn++;
    //  display->DrawQuad(quadList[i]);

     // display->DrawQuadTest(vertice0->screenX, vertice0->screenY, vertice1->screenX, vertice1->screenY, vertice2->screenX, vertice2->screenY, vertice3->screenX, vertice3->screenY, quadList[i].color);

      display->DrawTriangleFill(vertice1->screenX, vertice1->screenY, vertice3->screenX, vertice3->screenY, vertice0->screenX, vertice0->screenY, quadList[i].color);
      display->DrawTriangleFill(vertice1->screenX, vertice1->screenY, vertice2->screenX, vertice2->screenY, vertice3->screenX, vertice3->screenY, quadList[i].color);

    }

          if (quadList[i].linesDraw & (1 << 3)) {display->DrawLine(vertice0->screenX, vertice0->screenY, vertice1->screenX, vertice1->screenY, CBLACK);}
      if (quadList[i].linesDraw & (1 << 2)) {display->DrawLine(vertice1->screenX, vertice1->screenY, vertice2->screenX, vertice2->screenY, CBLACK);}
      if (quadList[i].linesDraw & (1 << 1)) {display->DrawLine(vertice2->screenX, vertice2->screenY, vertice3->screenX, vertice3->screenY, CBLACK);}
      if (quadList[i].linesDraw & (1 << 0)) {display->DrawLine(vertice3->screenX, vertice3->screenY, vertice0->screenX, vertice0->screenY, CBLACK);}
  }

  frameTime->Update(FTIME_3DDRAW);

  display->DrawString(600,40, "Look X:", FONT10, CBLACK);  
  display->DrawFloat(680,40, camera3D.lookAt.x, FONT10, CBLACK, 5);
  display->DrawString(600,60, "Look Y:", FONT10, CBLACK);  
  display->DrawFloat(680,60, camera3D.lookAt.y, FONT10, CBLACK, 5);
  display->DrawString(600,80, "Look Z:", FONT10, CBLACK);  
  display->DrawFloat(680,80, camera3D.lookAt.z, FONT10, CBLACK, 5);

  display->DrawString(600,100, "Dist:", FONT10, CBLACK);  
  display->DrawFloat(680,100, camera3D.distance, FONT10, CBLACK, 5);

  display->DrawString(600,120, "Yaw:", FONT8, CBLACK);  
  display->DrawFloat(680,120, camera3D.yaw, FONT8, CBLACK, 5);

  display->DrawString(600,140, "Pitch:", FONT8, CBLACK);  
  display->DrawFloat(680,140, camera3D.pitch, FONT8, CBLACK, 5);

  display->DrawString(600,160, "Cam X:", FONT8, CBLACK);  
  display->DrawFloat(680,160, camera3D.camPos.x, FONT8, CBLACK, 5);

  display->DrawString(600,180, "Cam Y:", FONT8, CBLACK);  
  display->DrawFloat(680,180, camera3D.camPos.y, FONT8, CBLACK, 5);

  display->DrawString(600,200, "Cam Z:", FONT8, CBLACK);  
  display->DrawFloat(680,200, camera3D.camPos.z, FONT8, CBLACK, 5);

  display->DrawString(600,200, "Cam Z:", FONT8, CBLACK);  
  display->DrawFloat(680,200, camera3D.camPos.z, FONT8, CBLACK, 5);

  display->DrawString(600,220, "Quads:", FONT8, CBLACK);  
  display->DrawFloat(680,220, quadsDrawn, FONT8, CBLACK, 5);


  
 /* display->DrawString(250,400, "Touch W:", FONT8, CBLACK);  
  if(isTouchingWorld)
  {
    display->DrawString(330,400, "YES", FONT8, CBLACK);
  }
  else
  {
    display->DrawString(330,400, "NO", FONT8, CBLACK);
  }

  display->DrawFloat(370,400, worldTouchPoint.x, FONT8, CBLACK, 2);
  display->DrawFloat(420,400, worldTouchPoint.y, FONT8, CBLACK, 2);
  display->DrawFloat(470,400, worldTouchPoint.z, FONT8, CBLACK, 2);*/



}


