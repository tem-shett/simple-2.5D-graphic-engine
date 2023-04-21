#include <cassert>
#include "framebuffer.h"

void FrameBuffer::set_pixel(int x, int y, uint32_t color) {
//    assert(x < w && y < h);
//    assert(x >= 0 && y >= 0);
    if (color == banned_color) {
        return;
    }
    int ind = x + (h - y - 1) * w;
//    if (frame[ind] == clear_color) {
//        next[ind] = ind - w;
        frame[ind] = color;
//    }
}

void FrameBuffer::clear() {
    std::fill(frame.begin(), frame.end(), clear_color);
//    std::iota(next.begin(), next.end(), -w);
//    std::fill(frame, frame + w * h, clear_color);
}

/*
int FrameBuffer::get_nexty(int x, int y) {
    int ind = x + (h - y - 1) * w;
    if (ind - w < 0 || frame[ind - w] == clear_color) {
        return y + 1;
    }
    ind = next[ind];
    int steps = 0;
    while (ind >= 0 && frame[ind] != clear_color) {
        ind = next[ind];
        steps++;
    }
    if (steps > 3) {
        int ind2 = x + (h - y - 1) * w;
        int ind3;
        while (ind2 >= 0 && frame[ind2] != clear_color) {
            ind3 = next[ind2];
            next[ind2] = ind;
            ind2 = ind3;
        }
    }
    return -((ind - x) / w - (h - 1));
}
 */
/*
 * #include <cassert>
#include "framebuffer.h"

void FrameBuffer::set_pixel(int x, int y, uint32_t color) {
    assert(x < w && y < h && x >= 0 && y >= 0);
    int ind = x + (h - y - 1) * w;
    if (frame[ind] == clear_color) {
        next[ind] = ind - w;
        frame[ind] = color;
    }
}

void FrameBuffer::clear() {
    std::fill(frame.begin(), frame.end(), clear_color);
//    std::fill(frame, frame + w * h, clear_color);
}

int FrameBuffer::get_nexty(int x, int y) {
    int ind = x + (h - y - 1) * w;
    if (ind - w < 0 || frame[ind - w] == clear_color) {
        return y + 1;
    }
    ind = next[ind];
    int steps = 0;
    while (ind >= 0 && frame[ind] != clear_color) {
        ind = next[ind];
        steps++;
    }
    if (steps > 3) {
        int ind2 = x + (h - y - 1) * w;
        int ind3;
        while (ind2 >= 0 && frame[ind2] != clear_color) {
            ind3 = next[ind2];
            next[ind2] = ind;
            ind2 = ind3;
        }
    }
    return -((ind - x) / w - (h - 1));
}
 */