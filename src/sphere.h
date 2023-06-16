#pragma once

#include "linalg.h"
#include <cstdint>
#include <limits>

struct Sphere {
    Sphere(const Vector3 &o, const float r, const uint32_t c);
    Vector3 origin;
    uint32_t color;
    float radius;
    float compute_intersection(const float t_min, const Vector3 &camera_pos, const Vector3 &world_viewport_point) const;
};