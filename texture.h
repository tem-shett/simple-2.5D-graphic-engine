#ifndef UNTITLED23_TEXTURE_H
#define UNTITLED23_TEXTURE_H

#include <vector>
#include <string>
#include "color_funcs.h"

const uint32_t banned_color = pack_color(254, 244, 233);
const double MIN_LIGHT_C = 0.6;

struct Texture {
    int w, h;
    std::vector<uint32_t> img;
    std::vector<uint32_t> img_low_light;

    Texture(const std::string filename);

    uint32_t get_color(double px, double py);

    uint32_t get_color_low_light(double px, double py);
};

void erase_color(Texture *texture, uint32_t color_to_erase);

#endif //UNTITLED23_TEXTURE_H
