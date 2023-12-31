#include "light.h"

Light::Light(float l_intensity, float a_intensity) : light_intensity(l_intensity), ambient_intensity(a_intensity)  {}

PointLight::PointLight(float l_intensity, float a_intensity, const Vector3 &light_position) : Light(l_intensity, a_intensity), position(light_position){}

float PointLight::compute_intensity(const Vector3 &point, const Vector3 &point_normal, const Vector3 &camera_pos, const float specular_alpha) const {
    float intensity = 0.0;

    intensity += this->ambient_intensity;

    // Diffuse Term Calculation
    Vector3 light_dir_normal = (this->position - point).normalized();
    intensity += std::max(static_cast<float>((point_normal * light_dir_normal) * this->light_intensity), static_cast<float>(0.0));

    // Specular Term Calculation
    Vector3 view_vector = (camera_pos - point).normalized();
    Vector3 reflection_vector = ((2 * (point_normal * (point_normal * light_dir_normal))) - light_dir_normal).normalized();

    if((view_vector * reflection_vector) > 0){
        intensity += std::max(static_cast<float>(std::pow(view_vector * reflection_vector, specular_alpha) * this->light_intensity), static_cast<float>(0.0));
    }

    auto light_distance_vec = point-this->position;
    // Less intense falloff then inverse square law
    intensity /= std::sqrt(light_distance_vec.length());

    return intensity;
}

bool PointLight::point_blocked(const Vector3 &point, const Sphere &sphere) const {
    bool point_blocked = false;

    auto test_t_blocked = sphere.compute_intersection(0.001, 1.0, point, this->position);
    if(test_t_blocked < std::numeric_limits<float>::max()){
        point_blocked = true;
    }

    return point_blocked;
}


DirectionLight::DirectionLight(float l_intensity, float a_intensity, const Vector3 &light_direction) : Light(l_intensity, a_intensity), direction(light_direction.normalized()){}

float DirectionLight::compute_intensity(const Vector3 &point, const Vector3 &point_normal, const Vector3 &camera_pos, const float specular_alpha) const {
    float intensity = 0.0;

    intensity += this->ambient_intensity;

    // Diffuse Term Calculation
    Vector3 light_dir_normal = -this->direction;
    intensity += std::max(static_cast<float>((point_normal * light_dir_normal) * this->light_intensity), static_cast<float>(0.0));

    // Specular Term Calculation
    Vector3 view_vector = (camera_pos - point).normalized();
    Vector3 reflection_vector = ((2 * (point_normal * (point_normal * light_dir_normal))) - light_dir_normal).normalized();

    if((view_vector * reflection_vector) > 0){
        intensity += std::max(static_cast<float>(std::pow(view_vector * reflection_vector, specular_alpha) * this->light_intensity), static_cast<float>(0.0));
    }

    // Light assumed to be at infinity so no need to attenuate by distance as majority light fall off has ocurred

    return intensity;
}

bool DirectionLight::point_blocked(const Vector3 &point, const Sphere &sphere) const {
    bool point_blocked = false;
    
    auto test_t_blocked = sphere.compute_intersection(0.001, std::numeric_limits<float>::max(), point, -this->direction);

    if(test_t_blocked < std::numeric_limits<float>::max()){
        point_blocked = true;
    }

    return point_blocked;
}