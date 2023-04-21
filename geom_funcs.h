#ifndef UNTITLED23_GEOM_FUNCS_H
#define UNTITLED23_GEOM_FUNCS_H

#include "line.h"
#include "gameobjects.h"

struct Visible_angle {
    Pt<double> vec1, vec2;
    Pt<double> vec_center;
    Pt<double> norm1;

    Visible_angle(Pt<double> v1, Pt<double> v2, Pt<double> vc): vec1(v1), vec2(v2), vec_center(vc) {
        norm1 = vec_center.normal().len1();
    }
};

bool check_bb_in_angle(Player pl, std::pair<Pt<double>, Pt<double>> bb, Visible_angle va);

bool is_beyond_wall(Pt<double> pos, Wall wall);

double get_dist_from_wall_signed(Pt<double> pos, Wall wall);

template<typename Type>
bool in_triangle(Pt<Type> p1, Pt<Type> p2, Pt<Type> p3, Pt<Type> p) {
    bool f1 = true, f2 = true;
    std::vector<Pt<Type>> ps = {p1, p2, p3};
    for (int i = 0; i < 3; i++) {
        Type now = (ps[(i + 1) % 3] - ps[i]).cross(p - ps[i]);
        if (now > 0) {
            f1 = false;
        }
        if (now < 0) {
            f2 = false;
        }
    }
    return f1 || f2;
}

#endif //UNTITLED23_GEOM_FUNCS_H
