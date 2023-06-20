#pragma once

#include <cstdint>
#include <limits>

#include "linalg.h"

struct Sphere {
    Sphere(const Vector3 &o, const float r, const uint32_t c, const float spec_alpha, const float sphere_reflection);
    Vector3 origin;
    uint32_t color;
    float radius;
    float specular_alpha;
    float reflection;
    float compute_intersection(const float t_min, const float t_max, const Vector3 &camera_pos, const Vector3 &world_viewport_point) const;
};