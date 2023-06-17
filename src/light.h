#pragma once

#include <algorithm>

#include "linalg.h"
#include "sphere.h"

struct Light {
    float light_intensity = 0.0;
    float ambient_intensity = 0.0;
    Light(float l_intensity, float a_intensity);
    virtual ~Light() = default;
    virtual float compute_intensity(const Vector3 &point, const Vector3 &point_normal, const Vector3 &camera_pos, const float specular_alpha) const = 0;
    virtual bool point_blocked(const Vector3 &point, const Sphere &sphere) const = 0;
};

struct PointLight : public Light {
    Vector3 position;
    PointLight(float l_intensity, float a_intensity, const Vector3 &light_position);
    ~PointLight() = default;
    float compute_intensity(const Vector3 &point, const Vector3 &point_normal, const Vector3 &camera_pos, const float specular_alpha) const override;
    bool point_blocked(const Vector3 &point, const Sphere &sphere) const override;
};

struct DirectionLight : public Light {
    Vector3 direction;
    DirectionLight(float l_intensity, float a_intensity, const Vector3 &light_direction);
    ~DirectionLight() = default;
    float compute_intensity(const Vector3 &point, const Vector3 &point_normal, const Vector3 &camera_pos, const float specular_alpha) const override;
    bool point_blocked(const Vector3 &point, const Sphere &sphere) const override;
};