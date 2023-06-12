#include "linalg.h"

Vector3::Vector3(){}

Vector3::Vector3(const float a, const float b, const float c) : x(a), y(b), z(c) {}

Vector3 Vector3::operator=(const Vector3 &other){
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;

    return *this;
}

// Vector3 Addition
Vector3 operator+(const Vector3 &a, const Vector3 &b){
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

// Vector3 Subtraction
Vector3 operator-(const Vector3 &a, const Vector3 &b){
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

// Vector3 Dot Product
float operator*(const Vector3 &a, const Vector3 &b){
    return (a.x * b.x) + (a.y * b.y) + (a.z + b.z);
}

// Vector3 Scalar Multiplication
Vector3 operator*(const float s, const Vector3 &v){
    return Vector3(s * v.x, s * v.y, s * v.z);
}

// Vector3 Scalar Multiplication
Vector3 operator*(const Vector3 &v, const float s){
    return Vector3(s * v.x, s * v.y, s * v.z);
}