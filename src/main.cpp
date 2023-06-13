#include <iostream>
#include "linalg.h"
#include "sphere.h"
#include "imagebuffer.h"
#include <vector>
#include <cmath>

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

    std::vector<Sphere> spheres;

    // Red Sphere
    spheres.emplace_back(Vector3{0, -1, 3}, 1, 0xFF0000FF);

    // Blue Sphere
    spheres.emplace_back(Vector3{2, 0, 4}, 1, 0x0000FFFF);

    // Green Sphere
    spheres.emplace_back(Vector3{-2, 0, 4}, 1, 0x00FF00FF);

    spheres.emplace_back(Vector3{0, -0.25, 3.25}, 1, 0xFFFF00FF);

    for(auto y = 0; y < image_height; y++){
        for(auto x = 0; x < image_width; x++){
            int viewport_x = x + viewport_zero_x;
            int viewport_y = viewport_zero_y - y;

            Vector3 world_viewport_point = Vector3(viewport_width * (float)viewport_x/(float)image_width, viewport_height * (float)viewport_y/(float)image_height, viewport_z_dist);

            float t_closest = 100000;
            float t_min = 1;
            float t_max = 100000;
            RGBA_Color computed_color = 0xFFFFFFFF;

            for(const auto &s : spheres){
                bool closest_updated = false;

                auto r = s.radius;
                auto co = camera_pos - s.origin;

                float a = world_viewport_point * world_viewport_point;
                float b = 2 * (co * world_viewport_point);
                float c = (co * co) - r*r;

                float disc = b*b - 4*a*c;

                if(disc < 0){
                    continue;
                }

                float t1 = (-b + sqrt(disc)) / (2 * a);
                float t2 = (-b - sqrt(disc)) / (2 * a);

                if(t1 >= t_min && t1 <= t_max && t1 <= t_closest){
                    t_closest = t1;
                    closest_updated = true;
                }

                if(t2 >= t_min && t2 <= t_max && t2 <= t_closest){
                    t_closest = t2;
                    closest_updated = true;
                }

                if(closest_updated){
                    computed_color = s.color;
                }

            }

            img_buf.SetPixel(x, y, computed_color);
        }
    }

    img_buf.DumpBuffer("img");
    return 0;
}