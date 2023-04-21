#ifndef UNTITLED23_BSP_H
#define UNTITLED23_BSP_H

#include <random>
#include <memory>
#include "gameobjects.h"
#include "line.h"

const int INF = 100000;

struct BSP_node {
    std::vector<Wall> cur_walls;
    std::vector<Floor> cur_floors;
    Line ln;
    std::pair<Pt<double>, Pt<double>> bb = {{INF, INF}, {-INF, -INF}}; // bounding box
    std::shared_ptr<BSP_node> under, above;
    int cnt_walls;

    BSP_node(): ln(Line(1, 0, 0)), under(nullptr), above(nullptr), cnt_walls(0) {};
};

std::shared_ptr<BSP_node> bsp_algo(std::vector<Wall> &now, std::vector<Floor> &fl_now);

#endif //UNTITLED23_BSP_H
