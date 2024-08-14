#include "Camera3D.h"
#include <Arduino.h>

void Camera3D::Init(Viewport *viewport)
{
  this->viewport = viewport;
  float aspect = viewport->widthF / viewport->heightF;   //800 x 480
  float fov = 0.8f;
  projectionMatrix.SetPerspective(fov, aspect, nearPlane, farPlane); 

}

void Camera3D::AddZoom(float delta) 
{
  if(delta != 0)
  {
    zoomPerStep = delta / (float)maxSteps;
    stepsLeftZoom = maxSteps;
  }
}

void Camera3D::AddYaw(float delta) 
{
  if(delta != 0)
  {
    yawPerStep = delta / (float)maxSteps;
    stepsLeftYaw = maxSteps;
  }
}

void Camera3D::AddPitch(float delta) 
{
  if(delta != 0)
  {
    pitchPerStep = delta / (float)maxSteps;
    stepsLeftPitch = maxSteps;
  }
}

void Camera3D::AddPan(float direction, float delta) 
{
  direction += yaw + PI;
  //delta *= (lookAt.z * -1);   //TODO What is this doing need to test why i put this here originally

  float addX = delta * cos(direction);
  float addY = delta * -sin(direction);

  if(stepsLeftPan > 0)
  {
    panXPerStep = ((panXPerStep * (float)stepsLeftPan) + addX) / (float)maxSteps;
    panYPerStep = ((panYPerStep * (float)stepsLeftPan) + addY) / (float)maxSteps;
    stepsLeftPan = maxSteps;
  }
  else
  {
    panXPerStep = addX / (float)maxSteps;
    panYPerStep = addY / (float)maxSteps;
    stepsLeftPan = maxSteps;
  }
}

void Camera3D::CenterOn(const Vector3 point, float distance, float yaw, float pitch) 
{
    lookAt = point;
    this->distance = distance;
    this->yaw = yaw;
    this->pitch = pitch;
}

void Camera3D::CenterOnSmooth(const Vector3 point, float yaw, float pitch)
{
  //need to tidy and finish this up from the old code
  //add pitch
  //
 /* if(stepsLeftPan == 0)
  {
    panXPerStep = (x - this->x) / (float)maxSteps;
    panYPerStep = (y - this->y) / (float)maxSteps;
    stepsLeftPan = maxSteps;
  }

  if(lookAt.z != point.z)
  {
    if(stepsLeftZoom == 0)
    {
      zoomPerStep = (z - this->z) / (float)maxStepsAuto;
      stepsLeftZoom = maxStepsAuto;
    }
  }

  if(stepsLeftSpin == 0)
  {
    yaw -= HALF_PI;
    if(yaw < 0) {yaw += TWO_PI;}
    if(yaw > TWO_PI) {yaw -= TWO_PI; }

    if(this->yaw != yaw)
    {
      double value = ShortestAngleDistance(this->yaw, yaw);
      spinPerStep = value / maxStepsSpin;
      stepsLeftSpin = maxStepsSpin;
    }
  }*/
}

Vector3 Camera3D::GetVector3Distance(float horiz, float vert, float distance)
{
  float qX = cos(vert) - sin(vert);
  float qZ = cos(vert) + cos(vert);

  float sX = cos(horiz) * qX;
  float sY = sin(horiz) * qX;

  Vector3 output;
  output.Set(sX, qZ, sY);
  output.Normalize();
  return output * distance;
}

void Camera3D::UpdateMovement()
{
  if(stepsLeftPan > 0)
  {
    lookAt.x += panXPerStep;
    lookAt.z += panYPerStep;
    stepsLeftPan --;
  }

  if(stepsLeftZoom > 0)
  {
    distance += zoomPerStep;
    stepsLeftZoom --;
    if(distance > maxZoom)
    {
      distance = maxZoom;
      stepsLeftZoom = 0;
    }
    else if(distance < minZoom)
    {
      distance = minZoom;
      stepsLeftZoom = 0;
    }
  }

  if(stepsLeftYaw > 0)
  {
    yaw += yawPerStep;
    stepsLeftYaw --;
    yaw = (yaw > TWO_PI) ? (yaw - TWO_PI) : (yaw < 0) ? (yaw + TWO_PI) : yaw;
  }

  if(stepsLeftPitch > 0)
  {
    pitch += pitchPerStep;
    stepsLeftPitch --;
    /*if(pitch > maxPitch)
    {
      pitch = maxPitch;
      stepsLeftPitch = 0;
    }
    if(pitch < minPitch)
    {
      pitch = minPitch;
      stepsLeftPitch = 0;
    }*/
  }
}

void Camera3D::UpdateMatrix()
{
  camPos.x = lookAt.x + distance * cos(pitch) * sin(yaw);
  camPos.y = lookAt.y + distance * -sin(pitch);
  camPos.z = lookAt.z + distance * cos(pitch) * cos(yaw);
  
  
    //PrintMatrix(viewMatrix, "Projection Matrix");
  /*Serial.print("camPos.x: ");
  Serial.println(camPos.x, 6); // 6 decimal places for precision
  Serial.print("camPos.y: ");
  Serial.println(camPos.y, 6);
  Serial.print("camPos.z: ");
  Serial.println(camPos.z, 6);

  Serial.print("lookAt.x: ");
  Serial.println(lookAt.x, 6);
  Serial.print("lookAt.y: ");
  Serial.println(lookAt.y, 6);
  Serial.print("lookAt.z: ");
  Serial.println(lookAt.z, 6);*/


  viewMatrix.SetLookAt(camPos, lookAt, Vector3(0, 1, 0)); // Assuming up vector is (0, 1, 0) 

 // PrintMatrix(projectionMatrix, "View Matrix");

  mvpMatrix.SetIdentity();

 // PrintMatrix(mvpMatrix, "MVP only identity");
  mvpMatrix.Multiply(projectionMatrix);
 // PrintMatrix(mvpMatrix, "MVP Multiplied by Projection Matrix");
  mvpMatrix.Multiply(viewMatrix);
 // PrintMatrix(mvpMatrix, "MVP Multiplied by Projection And View Matrix");

 //mvpMatrix.a[0] *= -1;
 // mvpMatrix.a[1] *= -1;
  // mvpMatrix.a[2] *= -1;
 //   mvpMatrix.a[3] *= -1;
  inverseMvpMatrix = mvpMatrix.GetInverse();
}

void Camera3D::UpdatePlanes()
{
  Vector3 cameraNormal = (lookAt - camPos).Normalize();

  // Left Plane
  frustumPlanes[0] = Plane(
  mvpMatrix.a[12] + mvpMatrix.a[0], mvpMatrix.a[13] + mvpMatrix.a[1], mvpMatrix.a[14] + mvpMatrix.a[2], mvpMatrix.a[15] + mvpMatrix.a[3]);
  frustumPlanes[0].Normalize();

  // Right Plane
  frustumPlanes[1] = Plane(mvpMatrix.a[12] - mvpMatrix.a[0], mvpMatrix.a[13] - mvpMatrix.a[1], mvpMatrix.a[14] - mvpMatrix.a[2], mvpMatrix.a[15] - mvpMatrix.a[3]);
  frustumPlanes[1].Normalize();

  // Top Plane
  frustumPlanes[2] = Plane(
  mvpMatrix.a[12] - mvpMatrix.a[4], mvpMatrix.a[13] - mvpMatrix.a[5], mvpMatrix.a[14] - mvpMatrix.a[6], mvpMatrix.a[15] - mvpMatrix.a[7]);
  frustumPlanes[2].Normalize();

  // Bottom Plane
  frustumPlanes[3] = Plane(
  mvpMatrix.a[12] + mvpMatrix.a[4], mvpMatrix.a[13] + mvpMatrix.a[5], mvpMatrix.a[14] + mvpMatrix.a[6], mvpMatrix.a[15] + mvpMatrix.a[7]);
  frustumPlanes[3].Normalize();

   /* // Near Plane
    frustumPlanes[4] = Plane(
        mvpMatrix.a[12] + mvpMatrix.a[8],  // mvpMatrix.a[3][0] + mvpMatrix.a[2][0]
        mvpMatrix.a[13] + mvpMatrix.a[9],  // mvpMatrix.a[3][1] + mvpMatrix.a[2][1]
        mvpMatrix.a[14] + mvpMatrix.a[10], // mvpMatrix.a[3][2] + mvpMatrix.a[2][2]
        mvpMatrix.a[15] + mvpMatrix.a[11]  // mvpMatrix.a[3][3] + mvpMatrix.a[2][3]
    );
    frustumPlanes[4].Normalize();

    // Far Plane
    frustumPlanes[5] = Plane(
        mvpMatrix.a[12] - mvpMatrix.a[8],  // mvpMatrix.a[3][0] - mvpMatrix.a[2][0]
        mvpMatrix.a[13] - mvpMatrix.a[9],  // mvpMatrix.a[3][1] - mvpMatrix.a[2][1]
        mvpMatrix.a[14] - mvpMatrix.a[10], // mvpMatrix.a[3][2] - mvpMatrix.a[2][2]
        mvpMatrix.a[15] - mvpMatrix.a[11]  // mvpMatrix.a[3][3] - mvpMatrix.a[2][3]
    );
    frustumPlanes[5].Normalize();

*/





  //Near Plane
  Vector3 nearPlanePoint = camPos + cameraNormal * nearPlane;
  frustumPlanes[4] = Plane(cameraNormal.x, cameraNormal.y, cameraNormal.z, -cameraNormal.Dot(nearPlanePoint));
  frustumPlanes[4].Normalize();
  
  //Far Plane
  Vector3 farPlanePoint = camPos + cameraNormal * farPlane;
  frustumPlanes[5] = Plane(-cameraNormal.x, -cameraNormal.y, -cameraNormal.z, cameraNormal.Dot(farPlanePoint));
  frustumPlanes[5].Normalize();
}

void Camera3D::Update()
{
  UpdateMovement();
  UpdateMatrix();
  UpdatePlanes();
}

//Test if its quicker accessing teh verticeList directly
//Calculate how many clock cycles per vertice
//To save memory see if we can do vertice screenX as int16_t
void Camera3D::TransformVerticeList(Vertice *verticeList, int16_t listCount)
{
  for (int i = 0; i < listCount; i++) 
  {
    float x = verticeList[i].localX;
    float y = verticeList[i].localY;
    float z = verticeList[i].localZ;

  //todo need to speed this up
    Vector3 point(x, y, z);
    verticeList[i].isClipped = isOutsideFrustum(point);

    float transformedX = mvpMatrix.a[0] * x + mvpMatrix.a[1] * y + mvpMatrix.a[2] * z + mvpMatrix.a[3];
    float transformedY = mvpMatrix.a[4] * x + mvpMatrix.a[5] * y + mvpMatrix.a[6] * z + mvpMatrix.a[7];
    float transformedZ = mvpMatrix.a[8] * x + mvpMatrix.a[9] * y + mvpMatrix.a[10] * z + mvpMatrix.a[11];
    float transformedW = mvpMatrix.a[12] * x + mvpMatrix.a[13] * y + mvpMatrix.a[14] * z + mvpMatrix.a[15];

    if (transformedW != 0.0f) 
    {
      transformedX /= transformedW;
      transformedY /= transformedW;
      transformedZ /= transformedW;
    }

    verticeList[i].screenX = viewport->centerX + (transformedX * viewport->halfWidthF);
    verticeList[i].screenY = viewport->centerY - (transformedY * viewport->halfHeightF);
    verticeList[i].screenZ = transformedZ;
  }
}

bool Camera3D::ScreenToWorld(Vector2 screenTouchPoint, Vector3 &worldTouchPoint) 
{
    float ndcX = (2.0f * (screenTouchPoint.x - viewport->centerXF)) / viewport->widthF - 1.0f;
    float ndcY = 1.0f - (2.0f * (screenTouchPoint.y - viewport->centerYF)) / viewport->heightF;

   /* Serial.print("ndcX: ");
    Serial.println(ndcX);
    Serial.print("ndcY: ");
    Serial.println(ndcY);*/

    // Step 2: Create the 4D point in clip space at both near and far plane
    Vector4 nearPoint(ndcX, ndcY, -1.0f, 1.0f); // Near plane (z = -1 in clip space)
    Vector4 farPoint(ndcX, ndcY, 1.0f, 1.0f);   // Far plane (z = 1 in clip space)

 /*   Serial.print("Near Point: ");
    Serial.print(nearPoint.x); Serial.print(", ");
    Serial.print(nearPoint.y); Serial.print(", ");
    Serial.print(nearPoint.z); Serial.print(", ");
    Serial.println(nearPoint.w);

    Serial.print("Far Point: ");
    Serial.print(farPoint.x); Serial.print(", ");
    Serial.print(farPoint.y); Serial.print(", ");
    Serial.print(farPoint.z); Serial.print(", ");
    Serial.println(farPoint.w);*/

    // Step 3: Transform the clip space points to world space
    Vector4 nearWorldPoint = inverseMvpMatrix.TransformPoint(nearPoint);
    Vector4 farWorldPoint = inverseMvpMatrix.TransformPoint(farPoint);

   /* Serial.print("Near World Point: ");
    Serial.print(nearWorldPoint.x); Serial.print(", ");
    Serial.print(nearWorldPoint.y); Serial.print(", ");
    Serial.print(nearWorldPoint.z); Serial.print(", ");
    Serial.println(nearWorldPoint.w);

    Serial.print("Far World Point: ");
    Serial.print(farWorldPoint.x); Serial.print(", ");
    Serial.print(farWorldPoint.y); Serial.print(", ");
    Serial.print(farWorldPoint.z); Serial.print(", ");
    Serial.println(farWorldPoint.w);*/

    // Convert from homogeneous to 3D coordinates
    float nearWInv = (fabs(nearWorldPoint.w) > 1e-6) ? 1.0f / nearWorldPoint.w : 0.0f;
    float farWInv = (fabs(farWorldPoint.w) > 1e-6) ? 1.0f / farWorldPoint.w : 0.0f;

    Vector3 nearPoint3D(nearWorldPoint.x * nearWInv, nearWorldPoint.y * nearWInv, nearWorldPoint.z * nearWInv);
    Vector3 farPoint3D(farWorldPoint.x * farWInv, farWorldPoint.y * farWInv, farWorldPoint.z * farWInv);

   /* Serial.print("Near Point 3D: ");
    Serial.print(nearPoint3D.x); Serial.print(", ");
    Serial.print(nearPoint3D.y); Serial.print(", ");
    Serial.println(nearPoint3D.z);

    Serial.print("Far Point 3D: ");
    Serial.print(farPoint3D.x); Serial.print(", ");
    Serial.print(farPoint3D.y); Serial.print(", ");
    Serial.println(farPoint3D.z);*/

    // Step 4: Compute the direction vector of the ray from near to far plane
    Vector3 rayDir = (farPoint3D - nearPoint3D).Normalize();

   /* Serial.print("Ray Direction: ");
    Serial.print(rayDir.x); Serial.print(", ");
    Serial.print(rayDir.y); Serial.print(", ");
    Serial.println(rayDir.z);*/

    // Step 5: Calculate the intersection with the Y = 0 plane
    const float EPSILON = 1e-6;
    if (fabs(rayDir.y) > EPSILON) {
        float t = -nearPoint3D.y / rayDir.y; // Calculate t where y = 0

    /*    Serial.print("t: ");
        Serial.println(t);*/

        worldTouchPoint = nearPoint3D + rayDir.MultiplyByScalar(t);

    /*    Serial.print("World Touch Point: ");
        Serial.print(worldTouchPoint.x); Serial.print(", ");
        Serial.print(worldTouchPoint.y); Serial.print(", ");
        Serial.println(worldTouchPoint.z);*/

        return true;
    }

    // If the ray is parallel to the Y = 0 plane and doesn't intersect, return false
    return false;
}

