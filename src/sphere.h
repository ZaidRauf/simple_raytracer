#pragma once

#include "linalg.h"

struct Sphere {
    Sphere(const Vector3 &o, const float r);
    Vector3 origin;
    float radius;
};