#ifndef UNTITLED23_FRAMEBUFFER_H
#define UNTITLED23_FRAMEBUFFER_H

#include <cstdint>
#include <vector>
#include <numeric>
#include "color_funcs.h"
#include "texture.h"

//const int screenH = 1080 / 1.7, screenW = 1920 / 1.7;
const int screenH = 720, screenW = 1280;
const uint32_t clear_color = 29935816;

struct FrameBuffer {
    int w, h;
    std::vector<uint32_t> frame;
//    std::vector<int> next;
//    uint32_t frame[screenH * screenW];
//    int next[screenH * screenW];

    FrameBuffer(int w_, int h_): w(w_), h(h_) {
        frame.resize(w * h);
//        next.resize(w * h);
//        std::fill(frame, frame + w * h, clear_color);
//        std::iota(next, next + w * h, -w);
    }

    void set_pixel(int x, int y, uint32_t color);

//    int get_nexty(int x, int y);

    void clear();
};

#endif //UNTITLED23_FRAMEBUFFER_H