#include "color_funcs.h"

uint32_t pack_color(uint32_t r, uint32_t g, uint32_t b) {
    return (0 << 24) + (b << 16) + (g << 8) + r;
}

uint32_t ch_light(uint32_t color, double c) {
//    uint32_t r = ;
//    uint32_t g = ((color >>  8) & 255);
//    uint32_t b = ((color >> 16) & 255);
//    uint32_t a = ((color >> 24) & 255);
//    return color * c;
    return pack_color((color & 255) * c, ((color >> 8) & 255) * c, (color >> 16) * c);
}