#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
#include <iostream>

#include "imagebuffer.h"
#include "light.h"
#include "linalg.h"
#include "sphere.h"

uint32_t blend_color(uint32_t color1, uint32_t color2, float blend){
    auto color1_red = (color1 >> 24) & 0xFF;
    auto color1_green = (color1 >> 16) & 0xFF;
    auto color1_blue = (color1 >> 8) & 0xFF;

    auto color2_red = (color2 >> 24) & 0xFF;
    auto color2_green = (color2 >> 16) & 0xFF;
    auto color2_blue = (color2 >> 8) & 0xFF;

    auto color_red = std::clamp((unsigned int) (color1_red * (1 - blend) + color2_red * blend), (unsigned int)0, (unsigned int)255);
    auto color_green = std::clamp((unsigned int) (color1_green * (1 - blend) + color2_green * blend), (unsigned int)0, (unsigned int)255);
    auto color_blue = std::clamp((unsigned int) (color1_blue * (1 - blend) + color2_blue * blend), (unsigned int)0, (unsigned int)255);

    uint32_t blended_color = (color_red << 24) | (color_green << 16) | (color_blue << 8) | 0xFF;

    return blended_color;
}

uint32_t compute_scaled_color(uint32_t color, float intensity){
    auto color_red = (color >> 24) & 0xFF;
    auto color_green = (color >> 16) & 0xFF;
    auto color_blue = (color >> 8) & 0xFF;

    color_red = std::clamp((unsigned int) (color_red * intensity), (unsigned int)0, (unsigned int)255);
    color_green = std::clamp((unsigned int) (color_green * intensity), (unsigned int)0, (unsigned int)255);
    color_blue = std::clamp((unsigned int) (color_blue * intensity), (unsigned int)0, (unsigned int)255);

    uint32_t shaded_color = (color_red << 24) | (color_green << 16) | (color_blue << 8) | 0xFF;

    return shaded_color;
}

std::vector<Sphere> build_sphere_vec(){
    std::vector<Sphere> spheres;

    // Red Sphere
    spheres.emplace_back(Vector3{0, -1, 3}, 1, 0xFF0000FF, 500, 0.2);

    // Blue Sphere
    spheres.emplace_back(Vector3{2, 0, 4}, 1, 0x0000FFFF, 500, 0.3);

    // Green Sphere
    spheres.emplace_back(Vector3{-2, 0, 4}, 1, 0x00FF00FF, 10, 0.4);

    // Large Yellow Sphere acting as ground
    spheres.emplace_back(Vector3{0, -5001, 0}, 5000, 0xFFFF00FF, 1000, 0.5);

    return spheres;
}

std::vector<std::unique_ptr<Light>> build_light_vec(){
    std::vector<std::unique_ptr<Light>> lights;

    lights.emplace_back(new PointLight{0.6, 0.0, {2, 1, -2}});

    lights.emplace_back(new DirectionLight{0.2, 0.0, {-1, -4, -4}});

    return lights;
}

uint32_t reflective_ray_color_blend(const int depth, const Vector3 &incoming_ray, const Vector3 &point_normal, const Vector3 &point, const uint32_t current_color, const float blend_coef, const std::vector<Sphere> &spheres, const std::vector<std::unique_ptr<Light>> &lights){
    if(depth <= 0){
        return current_color;
    }

    auto new_depth = depth - 1;

    auto negative_ray = -incoming_ray.normalized();
    Vector3 reflect_ray = ((2 * (point_normal * (point_normal * negative_ray))) - negative_ray).normalized();

    float t_closest = std::numeric_limits<float>::max();
    const Sphere *closest_sphere = nullptr;

    for(const auto &s : spheres){
        float t_intersection = s.compute_intersection(0.001, std::numeric_limits<float>::max(), point, reflect_ray);

        if(t_intersection < t_closest){
            t_closest = t_intersection;
            closest_sphere = &s;
        }
    }
    
    uint32_t blended_color = current_color;

    if(closest_sphere != nullptr){
        Vector3 sphere_point = (t_closest * reflect_ray);
        Vector3 sphere_normal = (sphere_point - closest_sphere->origin).normalized();

        blended_color = blend_color(current_color, closest_sphere->color, blend_coef);
        blended_color = reflective_ray_color_blend(new_depth, reflect_ray, sphere_normal, sphere_point, blended_color, closest_sphere->reflection, spheres, lights);
    }

    return blended_color;
}

int main(){
    // Direction is assumed to be looking down positive Z axis
    const Vector3 camera_pos = Vector3(0, 0, 0);
    const unsigned int image_width = 1920;
    const unsigned int image_height = 1080;

    const int viewport_zero_x = -(image_width/2);
    const int viewport_zero_y = (image_height/2);

    ImageBuffer img_buf{image_width, image_height};

    float viewport_width = 1.77777777;
    float viewport_height = 1;
    float viewport_z_dist = 1;

    std::vector<Sphere> spheres = build_sphere_vec();
    std::vector<std::unique_ptr<Light>> lights = build_light_vec();

    for(auto y = 0; y < image_height; y++){
        for(auto x = 0; x < image_width; x++){
            int viewport_x = x + viewport_zero_x;
            int viewport_y = viewport_zero_y - y;

            Vector3 world_viewport_point = Vector3(viewport_width * (float)viewport_x/(float)image_width, viewport_height * (float)viewport_y/(float)image_height, viewport_z_dist);

            constexpr float t_min = 1;
            constexpr float t_max = std::numeric_limits<float>::max();

            RGBA_Color computed_color = 0xFFFFFFFF;
            computed_color = (1 << 24) | (129 << 16) | (127 << 8) | 0xFF;

            float t_closest = std::numeric_limits<float>::max();
            const Sphere *closest_sphere = nullptr;

            for(const auto &s : spheres){
                float t_intersection = s.compute_intersection(t_min, t_max, camera_pos, world_viewport_point);

                if(t_intersection < t_closest){
                    t_closest = t_intersection;
                    closest_sphere = &s;
                }
            }

            float intensity = 0.2;

            if(closest_sphere != nullptr){
                Vector3 sphere_point = (t_closest * (world_viewport_point - camera_pos));
                Vector3 sphere_normal = (sphere_point - closest_sphere->origin).normalized();
                
                for(const auto &l : lights){
                    bool point_is_blocked = false;

                    for(const auto &s : spheres){

                        // Not recommended but going to do this for now
                        if(closest_sphere == &s){
                            continue;
                        }

                        point_is_blocked |= l->point_blocked(sphere_point, s);
                    }
                    
                    if(!point_is_blocked){
                        intensity += l->compute_intensity(sphere_point, sphere_normal, camera_pos, closest_sphere->specular_alpha);
                    }
                }
                
                computed_color = reflective_ray_color_blend(1, world_viewport_point, sphere_normal, sphere_point, closest_sphere->color, closest_sphere->reflection, spheres, lights);
                computed_color = compute_scaled_color(computed_color, intensity);
            }

            img_buf.SetPixel(x, y, computed_color);
        }
    }

    img_buf.DumpBuffer("img");
    return 0;
}