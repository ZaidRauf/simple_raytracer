#pragma once

#include <ostream>

// Vector3 Definitions
struct Vector3 {
    Vector3();
    Vector3(const float a, const float b, const float c);
    float x = 0;
    float y = 0;
    float z = 0;
    Vector3 operator=(const Vector3 &other);
};

// Vector3 Addition
Vector3 operator+(const Vector3 &a, const Vector3 &b);

// Vector3 Subtraction
Vector3 operator-(const Vector3 &a, const Vector3 &b);

// Vector3 Dot Product
float operator*(const Vector3 &a, const Vector3 &b);

// Vector3 Scalar Multiplication
Vector3 operator*(const float s, const Vector3 &v);

// Vector3 Scalar Multiplication
Vector3 operator*(const Vector3 &v, const float s);

// Vector3 Unary Subtraction
Vector3 operator-(const Vector3 &v);

// Print Vector3
std::ostream& operator<<(std::ostream &os, const Vector3 &v);