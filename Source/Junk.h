/*
void Display::DrawQuad(Quad& quad)
{
  //todo cull drawing if all 4 vertices are on the same x or y plane
  //or simply draw a single pixel line
  float v0x;
  float v0y;
  float v1x;
  float v1y;
  float v2x;
  float v2y;
  float v3x;
  float v3y;

  Vertice *vList[4];
    //Fast rotate to first Vertice in order is at the top
  if ((quad.verticeList[0]->screenY <= quad.verticeList[1]->screenY) && (quad.verticeList[0]->screenY <= quad.verticeList[2]->screenY) && (quad.verticeList[0]->screenY <= quad.verticeList[3]->screenY)) { vList[0] = quad.verticeList[0]; vList[1] = quad.verticeList[1]; vList[2] = quad.verticeList[2]; vList[3] = quad.verticeList[3]; } 
  else if ((quad.verticeList[1]->screenY <= quad.verticeList[0]->screenY) && (quad.verticeList[1]->screenY <= quad.verticeList[2]->screenY) && (quad.verticeList[1]->screenY <= quad.verticeList[3]->screenY)) { vList[0] = quad.verticeList[1]; vList[1] = quad.verticeList[2]; vList[2] = quad.verticeList[3]; vList[3] = quad.verticeList[0]; } 
  else if ((quad.verticeList[2]->screenY <= quad.verticeList[0]->screenY) && (quad.verticeList[2]->screenY <= quad.verticeList[1]->screenY) && (quad.verticeList[2]->screenY <= quad.verticeList[3]->screenY)) { vList[0] = quad.verticeList[2]; vList[1] = quad.verticeList[3]; vList[2] = quad.verticeList[0]; vList[3] = quad.verticeList[1]; } 
  else { vList[0] = quad.verticeList[3]; vList[1] = quad.verticeList[0]; vList[2] = quad.verticeList[1]; vList[3] = quad.verticeList[2];}

  //todo check if on same line
  float toAddLeft = GET_DELTAS(0, 3);
  float toAddRight = GET_DELTAS(0, 1);

  uint32_t before = ARM_DWT_CYCCNT;

  Serial.print(toAddLeft);
  Serial.print(" ");
  Serial.println(toAddRight);

  uint32_t after = ARM_DWT_CYCCNT;

  Serial.println(after - before);
}/*