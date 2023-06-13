#pragma once

#include "linalg.h"
#include <cstdint>

struct Sphere {
    Sphere(const Vector3 &o, const float r, const uint32_t c);
    Vector3 origin;
    uint32_t color;
    float radius;
};