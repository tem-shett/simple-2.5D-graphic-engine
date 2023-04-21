#include <algorithm>
#include <cassert>
#include "bsp.h"

std::mt19937 rnd_(23);

std::shared_ptr<BSP_node> bsp_algo(std::vector<Wall> &now, std::vector<Floor> &fl_now) {
    if (now.empty()) {
        std::shared_ptr<BSP_node> node = std::make_shared<BSP_node>();
        node->cur_floors = fl_now;
        if (!fl_now.empty()) {
            node->bb = get_bounding_box(fl_now[0].pts);
            for (int i = 1; i < (int) fl_now.size(); i++) {
                node->bb = get_bounding_box(node->bb, get_bounding_box(fl_now[i].pts));
            }
        }
        return node;
    }
    Wall main_wall = now[rnd_() % (int) now.size()];
    std::vector<Wall> vert, under, above;
    std::vector<Floor> fl_under, fl_above;
    std::vector<Pt<double>> pts_on_ln;
    for (auto wall : now) {
        double dist1 = main_wall.ln.get_dist(wall.p1), dist2 = main_wall.ln.get_dist(wall.p2);
        if (std::abs(dist1) < epsilon && std::abs(dist2) < epsilon) {
            vert.push_back(wall);
            pts_on_ln.push_back(wall.p1), pts_on_ln.push_back(wall.p2);
        } else if (dist1 >= -epsilon && dist2 >= -epsilon) {
            above.push_back(wall);
        } else if (dist1 <= epsilon && dist2 <= epsilon) {
            under.push_back(wall);
        } else {
            Pt<double> p = main_wall.ln.intersection(get_line_by_coeffs(wall.p1.get_line(wall.p2)));
            Wall wall1(wall.p1, p, wall.h1, wall.h2, wall.texture);
            Wall wall2(p, wall.p2, wall.h1, wall.h2, wall.texture);
            double right_cut = (p - wall.p2).dist() / (wall.p1 - wall.p2).dist();
            double left_cut = (p - wall.p1).dist() / (wall.p1 - wall.p2).dist();
            wall1.left_cut = wall.left_cut, wall1.right_cut = wall.right_cut;
            wall2.left_cut = wall.left_cut, wall2.right_cut = wall.right_cut;
            wall1.right_cut += (1 - wall.right_cut - wall.left_cut) * right_cut;
            wall2.left_cut += (1 - wall.left_cut - wall.right_cut) * left_cut;
            wall1.ind = wall2.ind = wall.ind;
            if (dist1 > 0) {
                std::swap(wall1, wall2);
            }
            under.push_back(wall1);
            above.push_back(wall2);
        }
    }
    for (auto floor : fl_now) {
        auto fla = floor.cut(main_wall.ln, true), flu = floor.cut(main_wall.ln, false);
        if (!fla.pts.empty()) {
            fl_above.push_back(fla);
        }
        if (!flu.pts.empty()) {
            fl_under.push_back(flu);
        }
    }
    auto node = std::make_shared<BSP_node>();
    node->ln = main_wall.ln;
    node->cur_walls = vert;
    node->bb = get_bounding_box(pts_on_ln);
    node->cnt_walls = (int) node->cur_walls.size();
//    assert((int) node->cur_walls.size() == 1);
    {
        node->under = bsp_algo(under, fl_under);
        node->bb = get_bounding_box(node->bb, node->under->bb);
        node->cnt_walls += node->under->cnt_walls;
    }
    {
        node->above = bsp_algo(above, fl_above);
        node->bb = get_bounding_box(node->bb, node->above->bb);
        node->cnt_walls += node->above->cnt_walls;
    }
    return node;
}
