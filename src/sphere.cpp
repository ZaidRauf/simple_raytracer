#include "sphere.h"

Sphere::Sphere(const Vector3 &o, const float r, const uint32_t c, const float spec_alpha) : origin(o), radius(r), color(c), specular_alpha(spec_alpha){};

float Sphere::compute_intersection(const float t_min, const Vector3 &camera_pos, const Vector3 &world_viewport_point) const {
    constexpr float t_max = std::numeric_limits<float>::max();
    float t_closest = std::numeric_limits<float>::max();

    auto r = this->radius;
    auto co = camera_pos - this->origin;

    float a = world_viewport_point * world_viewport_point;
    float b = 2 * (co * world_viewport_point);
    float c = (co * co) - r*r;

    float disc = b*b - 4*a*c;

    if(disc < 0){
        return t_closest;
    }

    float t1 = (-b + std::sqrt(disc)) / (2 * a);
    float t2 = (-b - std::sqrt(disc)) / (2 * a);

    if(t1 >= t_min && t1 <= t_max && t1 <= t_closest){
        t_closest = t1;
    }

    if(t2 >= t_min && t2 <= t_max && t2 <= t_closest){
        t_closest = t2;
    }

    return t_closest;
}