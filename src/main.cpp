#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>

#include "imagebuffer.h"
#include "light.h"
#include "linalg.h"
#include "sphere.h"

//TODO: Turn C style casts into static_cast(s)
//TODO: Move material values into own struct

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
    spheres.emplace_back(Vector3{0, -1, 3}, 1, 0xFF0000FF, 500);

    // Blue Sphere
    spheres.emplace_back(Vector3{2, 0, 4}, 1, 0x0000FFFF, 500);

    // Green Sphere
    spheres.emplace_back(Vector3{-2, 0, 4}, 1, 0x00FF00FF, 10);

    // Large Yellow Sphere acting as ground
    spheres.emplace_back(Vector3{0, -5001, 0}, 5000, 0xFFFF00FF, 1000);

    return spheres;
}

std::vector<std::unique_ptr<Light>> build_light_vec(){
    std::vector<std::unique_ptr<Light>> lights;

    lights.emplace_back(new PointLight{0.7, 0.05, {2, 1, 0}});

    lights.emplace_back(new DirectionLight{0.15, 0.0, {0, -1, 0}});

    return lights;
}

int main(){
    // Direction is assumed to be looking down positive Z axis
    const Vector3 camera_pos = Vector3(0, 0, 0);
    const unsigned int image_width = 512;
    const unsigned int image_height = 512;

    const int viewport_zero_x = -(image_width/2);
    const int viewport_zero_y = (image_height/2);

    ImageBuffer img_buf{image_width, image_height};

    float viewport_width = 1;
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
            RGBA_Color computed_color = 0xFFFFFFFF;
            computed_color = (1 << 24) | (129 << 16) | (127 << 8) | 0xFF;

            float intensity = 0.0;

            float t_closest = std::numeric_limits<float>::max();
            const Sphere *closest_sphere = nullptr;

            for(const auto &s : spheres){
                float t_intersection = s.compute_intersection(t_min, camera_pos, world_viewport_point);

                if(t_intersection < t_closest){
                    t_closest = t_intersection;
                    closest_sphere = &s;
                }
            }

            if(closest_sphere != nullptr){
                Vector3 sphere_point = (t_closest * world_viewport_point);
                Vector3 sphere_normal = (sphere_point - closest_sphere->origin).normalized();
                
                for(const auto &l : lights){
                    intensity += l->compute_intensity(sphere_point, sphere_normal, camera_pos, closest_sphere->specular_alpha);
                }

                computed_color = compute_scaled_color(closest_sphere->color, intensity);
            }

            img_buf.SetPixel(x, y, computed_color);
        }
    }

    img_buf.DumpBuffer("img");
    return 0;
}