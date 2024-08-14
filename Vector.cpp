#include "Vectors.h"

Vector2::Vector2() : x(0), y(0) {}

Vector2::Vector2(float x, float y) : x(x), y(y) {}

void Vector2::Set(float x, float y) {
    this->x = x;
    this->y = y;
}

Vector2 Vector2::Add(const Vector2& other) const 
{
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::Subtract(const Vector2& other) const 
{
    return Vector2(x - other.x, y - other.y);
}

float Vector2::CrossProduct(const Vector2& other) const 
{
    return x * other.y - y * other.x;
}

float Vector2::Distance(const Vector2& other) const 
{
    float dx = x - other.x;
    float dy = y - other.y;
    return sqrt(dx * dx + dy * dy);
}

Vector2 Vector2::GetPoint(float angle, float distance) const 
{
    float newX = x + distance * cos(angle);
    float newY = y + distance * sin(angle);
    return Vector2(newX, newY);
}

void Vector2::RotateVector(const Vector2& pivot, float angle) 
{
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    // Translation to the pivot
    float translatedX = x - pivot.x;
    float translatedY = y - pivot.y;

    // Rotation using a 2D rotation matrix
    x = translatedX * cosAngle - translatedY * sinAngle;
    y = translatedX * sinAngle + translatedY * cosAngle;

    // Translate back to the original position
    x += pivot.x;
    y += pivot.y;
}

void Vector2::Normalize() 
{
    float length = sqrt(x * x + y * y);

    // Avoid division by zero
    if (length != 0.0f) {
        x /= length;
        y /= length;
    }
}

float Vector2::GetAngle() const {
    return atan2(y, x);
}

//////////////VECTOR 3////////////////////////

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

void Vector3::Set(float x, float y, float z) 
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector3::AddVector(const Vector3& other) 
{
    x += other.x;
    y += other.y;
    z += other.z;
}

Vector3 Vector3::operator-(const Vector3& other) const 
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator+(const Vector3& other) const 
{
  return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator*(const Vector3& other) const 
{
    return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
}


Vector3 Vector3::operator*(float scalar) const 
{
  return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::MultiplyByScalar(float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::Normalize() const 
{
  float length = sqrt(x * x + y * y + z * z);
  if (length != 0) 
  {
    return Vector3(x / length, y / length, z / length);
  }
  return Vector3(0, 0, 0); // Return zero vector if length is zero
}

Vector3 Vector3::Cross(const Vector3& other) const 
{
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

float Vector3::Dot(const Vector3& other) const 
{
    return x * other.x + y * other.y + z * other.z;
}



//////////////VECTOR 4////////////////////////


Vector4::Vector4() : x(0), y(0), z(0), w(1) {}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vector4 Vector4::operator*(const Matrix& mat) const 
{
    return Vector4(
        x * mat.a[0] + y * mat.a[4] + z * mat.a[8]  + w * mat.a[12],  // x * mat.a[0][0] + y * mat.a[1][0] + z * mat.a[2][0] + w * mat.a[3][0]
        x * mat.a[1] + y * mat.a[5] + z * mat.a[9]  + w * mat.a[13],  // x * mat.a[0][1] + y * mat.a[1][1] + z * mat.a[2][1] + w * mat.a[3][1]
        x * mat.a[2] + y * mat.a[6] + z * mat.a[10] + w * mat.a[14],  // x * mat.a[0][2] + y * mat.a[1][2] + z * mat.a[2][2] + w * mat.a[3][2]
        x * mat.a[3] + y * mat.a[7] + z * mat.a[11] + w * mat.a[15]   // x * mat.a[0][3] + y * mat.a[1][3] + z * mat.a[2][3] + w * mat.a[3][3]
    );
}


//////////////MATRIX////////////////////////

float Matrix::GetRoll()
{
    float roll = atan2(a[8], a[10]);  // Direct access using the linear array
    if (roll < 0)
    {
        roll = PI + (PI - (-roll));
    }
    return roll;
}

float Matrix::GetPitch()
{
    float pitch = -atan2(a[9], a[10]);  // Direct access using the linear array
    if (pitch < 0)
    {
        pitch = PI + (PI - (-pitch));
    }
    return pitch;
}

void Matrix::SetIdentity()
{
    // Set all elements to 0.0f
    memset(a, 0, sizeof(float) * 16);

    // Set the diagonal elements to 1.0f
    a[0]  = 1.0f;  // a[0][0]
    a[5]  = 1.0f;  // a[1][1]
    a[10] = 1.0f;  // a[2][2]
    a[15] = 1.0f;  // a[3][3]
}

void Matrix::SetTranslate(float x, float y, float z)
{
    SetIdentity();
    a[3]  = x;  // a[0][3]
    a[7]  = y;  // a[1][3]
    a[11] = z;  // a[2][3]
}

void Matrix::SetScale(float x, float y, float z)
{
    SetIdentity();
    a[0]  = x;  // a[0][0]
    a[5]  = y;  // a[1][1]
    a[10] = z;  // a[2][2]
}

void Matrix::SetFromQuaternion(float w, float x, float y, float z) 
{
    x = -x;
    y = -y;
    SetIdentity();
    float xx = x * x;
    float yy = y * y;
    float zz = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    a[0]  = 1 - 2 * (yy + zz);  // a[0][0]
    a[1]  = 2 * (xy - wz);      // a[0][1]
    a[2]  = 2 * (xz + wy);      // a[0][2]

    a[4]  = 2 * (xy + wz);      // a[1][0]
    a[5]  = 1 - 2 * (xx + zz);  // a[1][1]
    a[6]  = 2 * (yz - wx);      // a[1][2]

    a[8]  = 2 * (xz - wy);      // a[2][0]
    a[9]  = 2 * (yz + wx);      // a[2][1]
    a[10] = 1 - 2 * (xx + yy);  // a[2][2]
}

//this may be why i had to manually calculate my near and far planes
/*float range = far - near;
a[10] = -(far + near) / range;   // Notice the negative sign
a[14] = -(2.0f * far * near) / range; // Also negative*/
//float range = far - near;

void Matrix::SetPerspective(float fov, float aspect, float near, float far) 
{
    SetIdentity();

    float tanHalfFOV = tan(fov / 2.0f);
    float range = far - near;

    a[0]  = 1.0f / (aspect * tanHalfFOV);  // X-axis scaling
    a[5]  = 1.0f / tanHalfFOV;             // Y-axis scaling
    a[10] = -(far + near) / range;   // Notice the negative sign  //CHECK THIS LATER
    a[11] = -1.0f;                         // Perspective division term
    a[14] = -(2.0f * far * near) / range; // Also negative*/
    a[15] = 0.0f;                          // Homogeneous coordinate
}

void Matrix::Multiply(const Matrix &m)
{
    float result[16];

    // Row 0
    result[0] = a[0] * m.a[0] + a[1] * m.a[4] + a[2] * m.a[8] + a[3] * m.a[12];
    result[1] = a[0] * m.a[1] + a[1] * m.a[5] + a[2] * m.a[9] + a[3] * m.a[13];
    result[2] = a[0] * m.a[2] + a[1] * m.a[6] + a[2] * m.a[10] + a[3] * m.a[14];
    result[3] = a[0] * m.a[3] + a[1] * m.a[7] + a[2] * m.a[11] + a[3] * m.a[15];

    // Row 1
    result[4] = a[4] * m.a[0] + a[5] * m.a[4] + a[6] * m.a[8] + a[7] * m.a[12];
    result[5] = a[4] * m.a[1] + a[5] * m.a[5] + a[6] * m.a[9] + a[7] * m.a[13];
    result[6] = a[4] * m.a[2] + a[5] * m.a[6] + a[6] * m.a[10] + a[7] * m.a[14];
    result[7] = a[4] * m.a[3] + a[5] * m.a[7] + a[6] * m.a[11] + a[7] * m.a[15];

    // Row 2
    result[8] = a[8] * m.a[0] + a[9] * m.a[4] + a[10] * m.a[8] + a[11] * m.a[12];
    result[9] = a[8] * m.a[1] + a[9] * m.a[5] + a[10] * m.a[9] + a[11] * m.a[13];
    result[10] = a[8] * m.a[2] + a[9] * m.a[6] + a[10] * m.a[10] + a[11] * m.a[14];
    result[11] = a[8] * m.a[3] + a[9] * m.a[7] + a[10] * m.a[11] + a[11] * m.a[15];

    // Row 3
    result[12] = a[12] * m.a[0] + a[13] * m.a[4] + a[14] * m.a[8] + a[15] * m.a[12];
    result[13] = a[12] * m.a[1] + a[13] * m.a[5] + a[14] * m.a[9] + a[15] * m.a[13];
    result[14] = a[12] * m.a[2] + a[13] * m.a[6] + a[14] * m.a[10] + a[15] * m.a[14];
    result[15] = a[12] * m.a[3] + a[13] * m.a[7] + a[14] * m.a[11] + a[15] * m.a[15];

    // Copy the result back into the original matrix
    memcpy(a, result, sizeof(float) * 16);

}

void Matrix::SetRotateX(float angle) 
{
    SetIdentity();
    
    float c = cos(angle);
    float s = sin(angle);
    
    a[0]  = 1;  a[1]  = 0;  a[2]  = 0;  a[3]  = 0;  // a[0][0], a[0][1], a[0][2], a[0][3]
    a[4]  = 0;  a[5]  = c;  a[6]  = -s; a[7]  = 0;  // a[1][0], a[1][1], a[1][2], a[1][3]
    a[8]  = 0;  a[9]  = s;  a[10] = c;  a[11] = 0;  // a[2][0], a[2][1], a[2][2], a[2][3]
    a[12] = 0;  a[13] = 0;  a[14] = 0;  a[15] = 1;  // a[3][0], a[3][1], a[3][2], a[3][3]
}

void Matrix::SetRotateY(float angle) 
{
    SetIdentity();
    
    float c = cos(angle);
    float s = sin(angle);
    
    a[0]  = c;  a[1]  = 0;  a[2]  = s;  a[3]  = 0;  // a[0][0], a[0][1], a[0][2], a[0][3]
    a[4]  = 0;  a[5]  = 1;  a[6]  = 0;  a[7]  = 0;  // a[1][0], a[1][1], a[1][2], a[1][3]
    a[8]  = -s; a[9]  = 0;  a[10] = c;  a[11] = 0;  // a[2][0], a[2][1], a[2][2], a[2][3]
    a[12] = 0;  a[13] = 0;  a[14] = 0;  a[15] = 1;  // a[3][0], a[3][1], a[3][2], a[3][3]
}

void Matrix::SetRotateZ(float angle) 
{
    SetIdentity();
    
    float c = cos(angle);
    float s = sin(angle);
    
    a[0]  = c;  a[1]  = -s; a[2]  = 0;  a[3]  = 0;  // a[0][0], a[0][1], a[0][2], a[0][3]
    a[4]  = s;  a[5]  = c;  a[6]  = 0;  a[7]  = 0;  // a[1][0], a[1][1], a[1][2], a[1][3]
    a[8]  = 0;  a[9]  = 0;  a[10] = 1;  a[11] = 0;  // a[2][0], a[2][1], a[2][2], a[2][3]
    a[12] = 0;  a[13] = 0;  a[14] = 0;  a[15] = 1;  // a[3][0], a[3][1], a[3][2], a[3][3]
}

void Matrix::SetTranslation(const Vector3& eye) 
{
    SetIdentity(); // Initialize to identity matrix

    a[12] = -eye.x;  // a[3][0]
    a[13] = -eye.y;  // a[3][1]
    a[14] = -eye.z;  // a[3][2]
}

Vector3 Matrix::TransformPoint(const Vector3& point) const
{
    Vector3 result;
    float w = a[3] * point.x + a[7] * point.y + a[11] * point.z + a[15];

    result.x = (a[0] * point.x + a[4] * point.y + a[8] * point.z + a[12]) / w;
    result.y = (a[1] * point.x + a[5] * point.y + a[9] * point.z + a[13]) / w;
    result.z = (a[2] * point.x + a[6] * point.y + a[10] * point.z + a[14]) / w;

    return result;
}

Vector4 Matrix::TransformPoint(const Vector4& point) const
{
    return Vector4(
        a[0] * point.x + a[4] * point.y + a[8] * point.z + a[12] * point.w,
        a[1] * point.x + a[5] * point.y + a[9] * point.z + a[13] * point.w,
        a[2] * point.x + a[6] * point.y + a[10] * point.z + a[14] * point.w,
        a[3] * point.x + a[7] * point.y + a[11] * point.z + a[15] * point.w
    );
}

/*void Matrix::SetLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    Vector3 zAxis = (eye - target).Normalize();   // Forward
    Vector3 xAxis = up.Cross(zAxis).Normalize();  // Right
    Vector3 yAxis = zAxis.Cross(xAxis);           // Up

    a[0]  = xAxis.x;
    a[1]  = yAxis.x;
    a[2]  = zAxis.x;
    a[3]  = 0.0f;

    a[4]  = xAxis.y;
    a[5]  = yAxis.y;
    a[6]  = zAxis.y;
    a[7]  = 0.0f;

    a[8]  = xAxis.z;
    a[9]  = yAxis.z;
    a[10] = zAxis.z;
    a[11] = 0.0f;

    a[12] = -xAxis.Dot(eye);
    a[13] = -yAxis.Dot(eye);
    a[14] = -zAxis.Dot(eye);
    a[15] = 1.0f;
}*/
/////////////////
//Try this
//a[8]  =  -cameraNormal.x;  // Flip X if needed
//a[9]  =  cameraNormal.y;    // Remove Y flip
//a[10] =  cameraNormal.z;    // Remove Z flip

//a[9]  =  cameraNormal.y;

//a[0]  =  s.x;  
//a[1]  =  s.y; 
//a[2]  =  s.z;

//we are close but upside down
void Matrix::SetLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) 
{
    Vector3 cameraNormal = (target - eye).Normalize();  // Forward vector
    Vector3 u = up.Normalize();              // Up vector
    Vector3 s = cameraNormal.Cross(u).Normalize() * -1;      // Right vector *1
    u = s.Cross(cameraNormal);

    a[0]  =  s.x;  
    a[1]  =  s.y; 
    a[2]  =  s.z;  
    a[3]  = -s.Dot(eye);
    a[4]  =  u.x;  
    a[5]  =  u.y; 
    a[6]  =  u.z;  
    a[7]  = -u.Dot(eye);
    a[8]  = -cameraNormal.x;  
    a[9]  = -cameraNormal.y; 
    a[10] = -cameraNormal.z;  
    a[11] =  cameraNormal.Dot(eye);
    a[12] =  0.0f;  
    a[13] = 0.0f; 
    a[14] = 0.0f;  
    a[15] =  1.0f;
}
//showing corrct but z is up and down now
/*
void Matrix::SetLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) 
{
    Vector3 cameraNormal = (target - eye).Normalize();  // Forward vector
    Vector3 u = up.Normalize();              // Up vector
    Vector3 s = cameraNormal.Cross(u).Normalize() * -1;      // Right vector
    u = s.Cross(cameraNormal);

    a[0]  =  s.x;  
    a[1]  =  s.y; 
    a[2]  =  -s.z;  
    a[3]  = -s.Dot(eye);
    a[4]  =  u.x;  
    a[5]  =  u.y; 
    a[6]  =  -u.z;  
    a[7]  = -u.Dot(eye);
    a[8]  = cameraNormal.x;  
    a[9]  = cameraNormal.y; 
    a[10] = cameraNormal.z;  
    a[11] =  cameraNormal.Dot(eye);
    a[12] =  0.0f;  
    a[13] = 0.0f; 
    a[14] = 0.0f;  
    a[15] =  1.0f;
}*/

Matrix Matrix::GetInverse() const
{
    Matrix result;
    float det;

    result.a[0] = a[5]  * a[10] * a[15] - 
                 a[5]  * a[11] * a[14] - 
                 a[9]  * a[6]  * a[15] + 
                 a[9]  * a[7]  * a[14] +
                 a[13] * a[6]  * a[11] - 
                 a[13] * a[7]  * a[10];

    result.a[4] = -a[4]  * a[10] * a[15] + 
                  a[4]  * a[11] * a[14] + 
                  a[8]  * a[6]  * a[15] - 
                  a[8]  * a[7]  * a[14] - 
                  a[12] * a[6]  * a[11] + 
                  a[12] * a[7]  * a[10];

    result.a[8] = a[4]  * a[9] * a[15] - 
                 a[4]  * a[11] * a[13] - 
                 a[8]  * a[5] * a[15] + 
                 a[8]  * a[7] * a[13] + 
                 a[12] * a[5] * a[11] - 
                 a[12] * a[7] * a[9];

    result.a[12] = -a[4]  * a[9] * a[14] + 
                   a[4]  * a[10] * a[13] +
                   a[8]  * a[5] * a[14] - 
                   a[8]  * a[6] * a[13] - 
                   a[12] * a[5] * a[10] + 
                   a[12] * a[6] * a[9];

    result.a[1] = -a[1]  * a[10] * a[15] + 
                  a[1]  * a[11] * a[14] + 
                  a[9]  * a[2] * a[15] - 
                  a[9]  * a[3] * a[14] - 
                  a[13] * a[2] * a[11] + 
                  a[13] * a[3] * a[10];

    result.a[5] = a[0]  * a[10] * a[15] - 
                 a[0]  * a[11] * a[14] - 
                 a[8]  * a[2] * a[15] + 
                 a[8]  * a[3] * a[14] + 
                 a[12] * a[2] * a[11] - 
                 a[12] * a[3] * a[10];

    result.a[9] = -a[0]  * a[9] * a[15] + 
                  a[0]  * a[11] * a[13] + 
                  a[8]  * a[1] * a[15] - 
                  a[8]  * a[3] * a[13] - 
                  a[12] * a[1] * a[11] + 
                  a[12] * a[3] * a[9];

    result.a[13] = a[0]  * a[9] * a[14] - 
                  a[0]  * a[10] * a[13] - 
                  a[8]  * a[1] * a[14] + 
                  a[8]  * a[2] * a[13] + 
                  a[12] * a[1] * a[10] - 
                  a[12] * a[2] * a[9];

    result.a[2] = a[1]  * a[6] * a[15] - 
                 a[1]  * a[7] * a[14] - 
                 a[5]  * a[2] * a[15] + 
                 a[5]  * a[3] * a[14] + 
                 a[13] * a[2] * a[7] - 
                 a[13] * a[3] * a[6];

    result.a[6] = -a[0]  * a[6] * a[15] + 
                  a[0]  * a[7] * a[14] + 
                  a[4]  * a[2] * a[15] - 
                  a[4]  * a[3] * a[14] - 
                  a[12] * a[2] * a[7] + 
                  a[12] * a[3] * a[6];

    result.a[10] = a[0]  * a[5] * a[15] - 
                  a[0]  * a[7] * a[13] - 
                  a[4]  * a[1] * a[15] + 
                  a[4]  * a[3] * a[13] + 
                  a[12] * a[1] * a[7] - 
                  a[12] * a[3] * a[5];

    result.a[14] = -a[0]  * a[5] * a[14] + 
                   a[0]  * a[6] * a[13] + 
                   a[4]  * a[1] * a[14] - 
                   a[4]  * a[2] * a[13] - 
                   a[12] * a[1] * a[6] + 
                   a[12] * a[2] * a[5];

    result.a[3] = -a[1] * a[6] * a[11] + 
                  a[1] * a[7] * a[10] + 
                  a[5] * a[2] * a[11] - 
                  a[5] * a[3] * a[10] - 
                  a[9] * a[2] * a[7] + 
                  a[9] * a[3] * a[6];

    result.a[7] = a[0] * a[6] * a[11] - 
                 a[0] * a[7] * a[10] - 
                 a[4] * a[2] * a[11] + 
                 a[4] * a[3] * a[10] + 
                 a[8] * a[2] * a[7] - 
                 a[8] * a[3] * a[6];

    result.a[11] = -a[0] * a[5] * a[11] + 
                   a[0] * a[7] * a[9] + 
                   a[4] * a[1] * a[11] - 
                   a[4] * a[3] * a[9] - 
                   a[8] * a[1] * a[7] + 
                   a[8] * a[3] * a[5];

    result.a[15] = a[0] * a[5] * a[10] - 
                  a[0] * a[6] * a[9] - 
                  a[4] * a[1] * a[10] + 
                  a[4] * a[2] * a[9] + 
                  a[8] * a[1] * a[6] - 
                  a[8] * a[2] * a[5];

    det = a[0] * result.a[0] + a[1] * result.a[4] + a[2] * result.a[8] + a[3] * result.a[12];

    if (det == 0)
        return Matrix(); // Return identity matrix if determinant is zero, indicating a non-invertible matrix

    det = 1.0 / det;

    for (int i = 0; i < 16; i++) {
        result.a[i] *= det;
    }

    return result;
}
