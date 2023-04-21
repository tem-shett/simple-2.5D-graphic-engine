#include "geom_funcs.h"

bool check_bb_in_angle(Player pl, std::pair<Pt<double>, Pt<double>> bb, Visible_angle va) {
    std::vector<Pt<double>> pts = {bb.first, Pt<double>(bb.first.x, bb.second.y), bb.second, Pt<double>(bb.second.x, bb.first.y)};
    bool f_l1 = true, f_r2 = true, f_all_less0 = true;
    for (Pt<double> p : pts) {
        p = p - pl.pos;
        if (va.vec1.cross(p) <= 0) {
            f_l1 = false;
        }
        if (va.vec2.cross(p) >= 0) {
            f_r2 = false;
        }
        if (va.vec_center.dot(p) >= 0) {
            f_all_less0 = false;
        }
    }
    return !(f_l1 || f_r2 || f_all_less0);
}

bool is_beyond_wall(Pt<double> pos, Wall wall) {
    return (wall.p1 - wall.p2).dot(pos - wall.p2) < 0 || (wall.p2 - wall.p1).dot(pos - wall.p1) < 0;
}

double get_dist_from_wall_signed(Pt<double> pos, Wall wall) {
    double sign_ = wall.ln.get_dist(pos) > 0 ? 1 : -1;
    if ((wall.p1 - wall.p2).dot(pos - wall.p2) < 0) {
        return (pos - wall.p2).dist() * sign_;
    }
    if ((wall.p2 - wall.p1).dot(pos - wall.p1) < 0) {
        return (pos - wall.p1).dist() * sign_;
    }
    return wall.ln.get_dist(pos);
}

