#ifndef UNTITLED23_SCREEN_BSP_H
#define UNTITLED23_SCREEN_BSP_H

#include <cassert>
#include <memory>
#include "framebuffer.h"
#include "line.h"

struct Scr_bsp_node {
    std::shared_ptr<Scr_bsp_node> above = nullptr, under = nullptr;
    Line ln;
    bool filled = false;
    int sz = 1;
};

std::vector<std::vector<Pt<double>>> bsp_add_poly(const std::shared_ptr<Scr_bsp_node> &root, std::vector<Pt<double>> &pts);

std::shared_ptr<Scr_bsp_node> create_scr_bsp();

#endif //UNTITLED23_SCREEN_BSP_H
