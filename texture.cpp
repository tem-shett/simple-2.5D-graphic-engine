#include <iostream>
#include <cassert>
#include "texture.h"
#include "SDL.h"
#include "color_funcs.h"

Texture::Texture(const std::string filename) {
    SDL_Surface *tmp = SDL_LoadBMP(filename.c_str());
    if (!tmp) {
        std::cerr << "Error in SDL_LoadBMP: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(tmp);
    if (!surface) {
        std::cerr << "Error in SDL_ConvertSurfaceFormat: " << SDL_GetError() << std::endl;
        return;
    }
    w = surface->w, h = surface->h;
    if (w * 4 != surface->pitch) {
        std::cerr << "Error: the texture must be a 32 bit image" << std::endl;
        SDL_FreeSurface(surface);
        return;
    }
    auto *pixmap = reinterpret_cast<uint8_t *>(surface->pixels);
    img.resize(w * h);
    img_low_light.resize(w * h);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            auto r = pixmap[(i + j * w) * 4];
            auto g = pixmap[(i + j * w) * 4 + 1];
            auto b = pixmap[(i + j * w) * 4 + 2];
            auto a = pixmap[(i + j * w) * 4 + 3];
            int ind = i * h + (h - j - 1);
            img[ind] = pack_color(r, g, b);
            img_low_light[ind] = ch_light(img[ind], MIN_LIGHT_C);
//            if (img[i + (h - j - 1) * w] == 4294967295) {
//                img[i + (h - j - 1) * w] = banned_color;
//            }
        }
    }
}

uint32_t Texture::get_color(double px, double py) {
    px = std::max(0.0, std::min(1.0, px));
    py = std::max(0.0, std::min(1.0, py));
//    assert(0 <= px && px <= 1 && 0 <= py && py <= 1);
    int x = px * (w - 1), y = py * (h - 1);
    return img[x * h + y];
}

uint32_t Texture::get_color_low_light(double px, double py) {
    px = std::max(0.0, std::min(1.0, px));
    py = std::max(0.0, std::min(1.0, py));
//    assert(0 <= px && px <= 1 && 0 <= py && py <= 1);
    int x = px * (w - 1), y = py * (h - 1);
    return img_low_light[x * h + y];
}

void erase_color(Texture *texture, uint32_t color_to_erase) {
    for (int x = 0; x < texture->w; x++) {
        for (int y = 0; y < texture->h; y++) {
            int ind = x * texture->h + y;
            if (texture->img[ind] == color_to_erase) {
                texture->img[ind] = banned_color;
            }
        }
    }
}