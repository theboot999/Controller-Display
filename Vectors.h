#ifndef Vectors_h
#define Vectors_h
#include "Arduino.h"

class Matrix;

struct Vector2 
{
    float x, y;

    Vector2();
    Vector2(float x, float y);
    void Set(float x, float y);
    Vector2 Add(const Vector2& other) const;
    Vector2 Subtract(const Vector2& other) const;
    float CrossProduct(const Vector2& other) const;
    float Distance(const Vector2& other) const;
    Vector2 GetPoint(float angle, float distance) const;
    void RotateVector(const Vector2& pivot, float angle);
    void Normalize();
    float GetAngle() const;
};

struct Vector3
{
    float x, y, z;

    Vector3();
    Vector3(float x, float y, float z);
    void Set(float x, float y, float z);
    void AddVector(const Vector3& other);
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator*(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
    Vector3 MultiplyByScalar(float scalar) const; // Function to multiply by a scalar
    Vector3 Normalize() const;
    Vector3 Cross(const Vector3& other) const;
    float Dot(const Vector3& other) const;
};


struct Vector4 
{
    float x, y, z, w;

    Vector4();
    Vector4(float x, float y, float z, float w);
    Vector4 operator*(const Matrix& mat) const;
};

class Matrix
{
public:
void SetTranslation(const Vector3& eye);
void SetRotateX(float angle);
void SetRotateY(float angle);
void SetRotateZ(float angle);


  float a[16];
  void SetIdentity();
  void SetTranslate(float x, float y, float z);
  void SetPerspective(float fov, float aspect, float near, float far);
  void SetLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
  void Multiply(const Matrix &m); 
  void SetScale(float x, float y, float z);
  void SetFromQuaternion(float w, float x, float y, float z);
  Matrix GetInverse() const; // Method to invert the matrix
  float GetRoll();
  float GetPitch();
  Vector3 TransformPoint(const Vector3& point) const;
  Vector4 TransformPoint(const Vector4& point) const;
};

struct Vertice
{
  float localX;
  float localY;
  float localZ;

  int32_t screenX;
  int32_t screenY;
  float screenZ;
  bool isClipped = false;

  Vertice()
  {

  }

  Vertice(float localX, float localY)
  {
    this->localX = localX;
    this->localY = localY;
  }

  void SetScreen(int32_t screenX, int32_t screenY)
  {
    this->screenX = screenX;
    this->screenY = screenY;
  }

  void Set(float localX, float localY, float localZ)
  {
    this->localX = localX;
    this->localY = localY;
    this->localZ = localZ;
  }

  void MoveScreen(int32_t screenX, int32_t screenY)
  {
    this->screenX += screenX;
    this->screenY += screenY;
  }
};

struct Viewport
{
    int16_t left = 0;
    int16_t top = 0;
    int16_t right = 800;
    int16_t bottom = 480;
    int16_t width = right - left;
    int16_t height = bottom - top;
    int16_t centerX = (right - left) / 2 + left;
    int16_t centerY = (bottom - top) / 2 + top;

    float leftF = (float)left;
    float topF = (float)top;
    float rightF = (float)right;
    float bottomF = (float)bottom;
    float widthF = (float)width;
    float heightF = (float)height;
    float halfWidthF = widthF / 2;
    float halfHeightF = heightF / 2;
    float centerXF = (float)centerX;
    float centerYF = (float)centerY;

    Viewport() = default;

    Viewport(int16_t left, int16_t right, int16_t top, int16_t bottom)
        : left(left),
          top(top),
          right(right),
          bottom(bottom),
          width(right - left),
          height(bottom - top),
          centerX((right - left) / 2 + left),
          centerY((bottom - top) / 2 + top),

          leftF((float)left),
          topF((float)top),
          rightF((float)right),
          bottomF((float)bottom),
          widthF((float)width),        // Directly use the precomputed integer width
          heightF((float)height),      // Directly use the precomputed integer height
          halfWidthF(widthF / 2),        // Directly use the precomputed integer width
          halfHeightF(heightF / 2),      // Directly use the precomputed integer height
          centerXF((float)centerX),    // Directly use the precomputed integer centerX
          centerYF((float)centerY)     // Directly use the precomputed integer centerY
    {
    }
};

struct Quad 
{
  Vertice* verticeList[4];
  uint8_t color;
  float worldZ;
  uint8_t linesDraw;
  bool inScreenSpace = false;

  void Set(Vertice* v0, Vertice* v1, Vertice* v2, Vertice* v3, uint8_t color, uint8_t linesDraw)
  {
    verticeList[0] = v0;
    verticeList[1] = v1;
    verticeList[2] = v2;
    verticeList[3] = v3;
    this->color = color;
    this->linesDraw = linesDraw;
  //  this->color = random(30, 230);
  }

  void UpdateZ(Viewport *viewport)
  {
    worldZ = (verticeList[0]->screenZ + verticeList[1]->screenZ + verticeList[2]->screenZ + verticeList[3]->screenZ) / 4;
    inScreenSpace = !verticeList[0]->isClipped || !verticeList[1]->isClipped || !verticeList[2]->isClipped || !verticeList[3]->isClipped;
  }
};

struct Plane 
{
  Vector3 normal;
  float distance;

  Plane() : normal(Vector3()), distance(0) {}
  Plane(float a, float b, float c, float d) : normal(a, b, c), distance(d) {}

  void Normalize()
  {
    float magnitude = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    normal.x /= magnitude;
    normal.y /= magnitude;
    normal.z /= magnitude;
    distance /= magnitude;
  }

  inline float DistanceToPoint(const Vector3& point) const 
  {
    return normal.Dot(point) + distance;
  }

      // New method to define the plane from three points
    static Plane FromPoints(const Vector3& p1, const Vector3& p2, const Vector3& p3)
    {
        Plane plane;
        
        // Calculate two edge vectors
        Vector3 edge1 = p2 - p1;
        Vector3 edge2 = p3 - p1;

        // Compute the normal by taking the cross product of the edge vectors
        plane.normal = edge1.Cross(edge2).Normalize();

        // Calculate d by taking the dot product of the normal with any of the three points
        plane.distance = -plane.normal.Dot(p1);

        return plane;
    }
};





#endif