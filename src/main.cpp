#include <iostream>
#include "linalg.h"
#include "sphere.h"
#include "imagebuffer.h"
#include <vector>
#include <cmath>

int main(){
    // Direction is assumed to be looking down positive Z axis
    const Vector3 camera_pos = Vector3(0, 0, 0);
    const unsigned int image_width = 64;
    const unsigned int image_height = 64;

    const int viewport_zero_x = -(image_width/2);
    const int viewport_zero_y = (image_height/2);

    ImageBuffer img_buf{image_width, image_height};

    float viewport_width = 1;
    float viewport_height = 1;
    float viewport_z_dist = 1;

    std::vector<Sphere> spheres;

    spheres.emplace_back(Vector3{0, -1, 3}, 1, 0xFF0000FF);

    for(auto y = 0; y < image_height; y++){
        for(auto x = 0; x < image_width; x++){
            int viewport_x = x + viewport_zero_x;
            int viewport_y = viewport_zero_y - y;

            Vector3 world_viewport_point = Vector3(viewport_width * (float)viewport_x/(float)image_width, viewport_height * (float)viewport_y/(float)image_height, viewport_z_dist);

            for(const auto &s : spheres){
                RGBA_Color computed_color = 0xFFFFFFFF;

                auto r = s.radius;
                auto co = camera_pos - s.origin;

                float a = world_viewport_point * world_viewport_point;
                float b = 2 * (co * world_viewport_point);
                float c = (co * co) - r*r;

                float disc = b*b - 4*a*c;
                // std::cout << disc << std::endl;
                if(disc < 0){
                    // std::cout << "DISC < 0" << std::endl; 
                    img_buf.SetPixel(x, y, computed_color);
                    continue;
                }

                float t_closest = 100000;
                float t_min = 1;
                float t_max = 100000;

                float t1 = (-b + sqrt(disc)) / (2 * a);
                float t2 = (-b - sqrt(disc)) / (2 * a);

                if(t1 >= t_min && t1 <= t_max && t1 <= t_closest){
                    t_closest = t1;
                }

                if(t2 >= t_min && t2 <= t_max && t2 <= t_closest){
                    t_closest = t2;
                }

                if(t_closest < t_max){
                    // auto full_vec = (t_closest * world_viewport_point);
                    // float full_vec_len_sqr = full_vec * full_vec;
                    // std::cout << x << " " << y << " " << sqrt(full_vec_len_sqr) << std::endl; 
                    // std::cout << full_vec.x << " " << full_vec.y << " " <<  full_vec.z << std::endl; 
                    // float adj_color = ((float)0xFF / full_vec_len_sqr * 2);
                    // img_buf.SetPixel(x, y, (((uint32_t)adj_color) << 24 | 0xFF));
                    img_buf.SetPixel(x, y, s.color);
                }

                // std::cout << t_closest << std::endl; 

            }
            // std::cout << world_viewport_point.x << " " << world_viewport_point.y << std::endl;
        }
    }

    img_buf.DumpBuffer("img");
    return 0;
}