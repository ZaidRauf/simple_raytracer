#include <iostream>
#include "linalg.h"
#include "sphere.h"
#include "imagebuffer.h"

int main(){
    // Direction is assumed to be looking down positive Z axis
    const Vector3 camera_pos = Vector3(0, 0, 0);

    ImageBuffer img_buf{512, 512};

    img_buf.DumpBuffer("img");
    return 0;
}