#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>

#include "imagebuffer.h"
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

    // color_green = (color_green * intensity);
    // color_blue = color_blue * intensity;

    uint32_t shaded_color = (color_red << 24) | (color_green << 16) | (color_blue << 8) | 0xFF;

    return shaded_color;
}

int main(){
    // Direction is assumed to be looking down positive Z axis
    const Vector3 camera_pos = Vector3(0, 0, 0);
    const unsigned int image_width = 512;
    const unsigned int image_height = 512;
    // const unsigned int image_width = 1920;
    // const unsigned int image_height = 1080;


    const int viewport_zero_x = -(image_width/2);
    const int viewport_zero_y = (image_height/2);

    ImageBuffer img_buf{image_width, image_height};

    // float viewport_width = 1.77777777;
    float viewport_width = 1;
    float viewport_height = 1;
    float viewport_z_dist = 1;

    std::vector<Sphere> spheres;

    // Red Sphere
    spheres.emplace_back(Vector3{0, -1, 3}, 1, 0xFF0000FF);

    // Blue Sphere
    spheres.emplace_back(Vector3{2, 0, 4}, 1, 0x0000FFFF);

    // Green Sphere
    spheres.emplace_back(Vector3{-2, 0, 4}, 1, 0x00FF00FF);

    // Yellow Sphere in front of Blue and Green and occluding Red
    // spheres.emplace_back(Vector3{0, -0.25, 3.25}, 1.15, 0xFFFF00FF);

    Vector3 point_light{2, 1, 0};
    float point_light_intensity = 0.6;

    for(auto y = 0; y < image_height; y++){
        for(auto x = 0; x < image_width; x++){
            int viewport_x = x + viewport_zero_x;
            int viewport_y = viewport_zero_y - y;

            Vector3 world_viewport_point = Vector3(viewport_width * (float)viewport_x/(float)image_width, viewport_height * (float)viewport_y/(float)image_height, viewport_z_dist);

            // float t_closest = std::numeric_limits<float>::max();
            float t_min = 1;
            // float t_max = std::numeric_limits<float>::max();

            RGBA_Color computed_color = 0xFFFFFFFF;
            // computed_color = (1 << 24) | (129 << 16) | (127 << 8) | 0xFF;

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
                Vector3 light_dir_normal = (point_light - sphere_point).normalized();

                Vector3 view_vector = (camera_pos - sphere_point).normalized();
                Vector3 reflection_vector = ((2 * (sphere_normal * (sphere_normal * light_dir_normal))) - light_dir_normal).normalized();

                intensity += std::clamp((float) ((sphere_normal * light_dir_normal) * 0.6), (float)0.0, (float)1.0);

                if((view_vector * reflection_vector) > 0){
                    intensity += std::clamp((float) (std::pow(view_vector * reflection_vector, 500) * 0.6), (float)0.0, (float)1.0);
                }

                computed_color = compute_scaled_color(closest_sphere->color, intensity);
            }

            img_buf.SetPixel(x, y, computed_color);
        }
    }

    img_buf.DumpBuffer("img");
    return 0;
}