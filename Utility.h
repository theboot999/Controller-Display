#ifndef Utility_h
#define Utility_h
#include "Vectors.h"

#define L0000 B00000000
#define L0001 B00000001
#define L0010 B00000010
#define L0011 B00000011
#define L0100 B00000100
#define L0101 B00000101
#define L0110 B00000110
#define L0111 B00000111
#define L1000 B00001000
#define L1001 B00001001
#define L1010 B00001010
#define L1011 B00001011
#define L1100 B00001100
#define L1101 B00001101
#define L1110 B00001110
#define L1111 B00001111


/*
inline bool IsLineIntersect(Vector2 line1Start, Vector2 line1End, Vector2 line2Start, Vector2 line2End)
{
  Vector2 direction1 = line1End.Subtract(line1Start);
  Vector2 direction2 = line2End.Subtract(line2Start);

  float cross = direction1.CrossProduct(direction2);

  // Check if lines are parallel (cross product is almost zero)
  if (abs(cross) < 1e-9) 
  {
    return false; // Lines are parallel, no intersection
  }
  Vector2 startDifference = line2Start.Subtract(line1Start);
  float t = startDifference.CrossProduct(direction2) / cross;

  if (t >= 0.0 && t <= 1.0) 
  {
    float u = startDifference.CrossProduct(direction1) / cross;
    if (u >= 0.0 && u <= 1.0) 
    {
      return true;
    }
  }

  return false;
}

static inline bool IsManhattanDistance(int16_t x1, int16_t x2, int16_t y1, int16_t y2, int16_t range)
{
  Serial.print(range);
  Serial.print(" : ");
  Serial.println((abs(x2 - x1) - abs(y2 - y1)));
  return (abs(x2 - x1) - abs(y2 - y1)) <= range;
}

static inline void ClampRadian(float &radian)
{
  while (radian < 0)
  {
    radian += 2 * M_PI; // Assuming TWO_PI is equivalent to 2 * PI
  }
  while (radian >= 2 * M_PI)
  {
    radian -= 2 * M_PI;
  }
}

static inline float FindLineIntersectionDistance(Vector2 line1Start, Vector2 line1End, Vector2 line2Start, Vector2 line2End) 
{
  Vector2 direction1 = line1End.Subtract(line1Start);
  Vector2 direction2 = line2End.Subtract(line2Start);

  float cross = direction1.CrossProduct(direction2);

  if (abs(cross) < 1e-9)  
  {
    return -1.0; // Lines are parallel, no intersection
  }

  if (direction1.CrossProduct(direction2) < 0) 
  {
    return -1.0; // Lines are not pointing towards each other
  }

  Vector2 startDifference = line2Start.Subtract(line1Start);
  float t = startDifference.CrossProduct(direction2) / cross;
  Vector2 intersection  = Vector2(line1Start.x + t * direction1.x, line1Start.y + t * direction1.y);

  if (t >= 0.0 && t <= 1.0) 
  {
    float u = startDifference.CrossProduct(direction1) / cross;
    if (u >= 0.0 && u <= 1.0) 
    {
      Vector2 distanceVector = line1Start.Subtract(intersection);
      return sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
    }
  }

  return -1.0;
}
*/
#endif