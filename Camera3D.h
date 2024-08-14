#ifndef Camera3D_h
#define Camera3D_h

#include "Vectors.h"

//#define VIEW_PITCH_TOP_DOWN -PI / 2 + 0.00001f
#define VIEW_PITCH_TOP_DOWN -1.5607963
#define VIEW_PITCH_45       -0.5000000
#define VIEW_STRAIGHTMIN    1.5
//#define VIEW_STRAIGHTMIN    -0.03

#define VIEW_NORTH          PI
//need to add pan based on our view angle
//could make lookAt a vector4 with the final vector being the distance to look at
//we could make a camRot for its oreintations.  yaw pitch and roll
//we could add in the maths to do roll conversiton to?  (Later)
//So we have already calculated our forward vector in our set look at
//we should save this as a vector2
//should be easy to calculate at our plans from this
//near plane is this
//far plane is the inverse of this 


class Camera3D
{
  //move to private once debug is finished
public:
  float yaw, pitch; // Rotation angles
  float distance;   // Distance from look-at point
  Vector3 lookAt;   // Look at point
  Vector3 camPos;   // Current position
  Vector3 cameraNormal;

private:
  Plane frustumPlanes[6];
  float nearPlane = 0.3f;
  float farPlane = 2000.0f;

  Matrix viewMatrix;
  Matrix projectionMatrix;
  Matrix mvpMatrix;
  Matrix inverseMvpMatrix;

  const float minZoom = 200;
  const float maxZoom = 2000;
  const float minPitch = VIEW_PITCH_TOP_DOWN;
  const float maxPitch = VIEW_STRAIGHTMIN;

  Viewport *viewport;

  float panXPerStep;
  float panYPerStep;
  float zoomPerStep;
  float yawPerStep;
  float pitchPerStep;

  int16_t stepsLeftPan = 0;
  int16_t stepsLeftZoom = 0;
  int16_t stepsLeftYaw = 0;
  int16_t stepsLeftPitch = 0;
  const int16_t maxSteps = 15;

    Vector3 GetVector3Distance(float horiz, float vert, float distance);

public:
  void Init(Viewport *viewport);
  void AddZoom(float delta);
  void AddYaw(float delta);
  void AddPitch(float delta);
  void AddPan(float direction, float delta);
  void CenterOn(const Vector3 point, float distance, float yaw, float pitch);
  void CenterOnSmooth(const Vector3 point, float yaw, float pitch);
  Matrix GetUpdatedMatrix();
  bool ScreenToWorld(Vector2 screenTouchPoint, Vector3 &worldTouchPoint);
  void TransformVerticeList(Vertice *verticeList, int16_t listCount);
  void Update();

private:
  void UpdateMovement();
  void UpdateMatrix();
  void UpdatePlanes();

void PrintMatrix(const Matrix& matrix, const char* matrixName) 
{
    Serial.print(matrixName);
    Serial.println(" Matrix:");
    for (int i = 0; i < 4; ++i) { // Iterate over rows
        for (int j = 0; j < 4; ++j) { // Iterate over columns
            Serial.print(matrix.a[j + i * 4], 6); // Print each element with 6 decimal precision
            Serial.print("\t"); // Tab space for alignment
        }
        Serial.println(); // New line after each row
    }
    Serial.println(); // Extra new line for separation
}

void PrintCameraDetails() 
{
    Serial.println("Camera Details:");
    
    Serial.print("Yaw: ");
    Serial.println(yaw, 6); // Print yaw with 6 decimal precision
    
    Serial.print("Pitch: ");
    Serial.println(pitch, 6); // Print pitch with 6 decimal precision
    
    Serial.print("Distance: ");
    Serial.println(distance, 6); // Print distance with 6 decimal precision
    
    Serial.print("LookAt: (");
    Serial.print(lookAt.x, 6);
    Serial.print(", ");
    Serial.print(lookAt.y, 6);
    Serial.print(", ");
    Serial.print(lookAt.z, 6);
    Serial.println(")"); // Print lookAt coordinates with 6 decimal precision
    
    Serial.print("Camera Position: (");
    Serial.print(camPos.x, 6);
    Serial.print(", ");
    Serial.print(camPos.y, 6);
    Serial.print(", ");
    Serial.print(camPos.z, 6);
    Serial.println(")"); // Print camera position with 6 decimal precision
    
    Serial.println(); // Extra line for spacing
}



  inline bool isOutsideFrustum(const Vector3& point) 
  {
    for (int i = 0; i < 6; i++) 
    {
      if (frustumPlanes[i].DistanceToPoint(point) < 0) 
      {
        return true;
      }
    }
    return false;
  }
};

#endif