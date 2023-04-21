#include "point.h"

bool pt_in_angle(Pt<double> p, Pt<double> p_a1, Pt<double> p_a2) {
    return p_a1.cross(p) <= 0 && p_a2.cross(p) >= 0;
}

bool pt_on_seg(Pt<double> p, Pt<double> seg_pt1, Pt<double> seg_pt2) {
    if (seg_pt1.x > seg_pt2.x) {
        std::swap(seg_pt1, seg_pt2);
    }
    return seg_pt1.x <= p.x && p.x <= seg_pt2.x;
}

std::pair<Pt<double>, Pt<double>> get_bounding_box(std::vector<Pt<double>> pts) {
    Pt<double> p1 = pts[0], p2 = pts[0];
    for (auto p : pts) {
        p1.x = std::min(p1.x, p.x), p1.y = std::min(p1.y, p.y);
        p2.x = std::max(p2.x, p.x), p2.y = std::max(p2.y, p.y);
    }
    return {p1, p2};
}

std::pair<Pt<double>, Pt<double>> get_bounding_box(std::pair<Pt<double>, Pt<double>> bb1, std::pair<Pt<double>, Pt<double>> bb2) {
    return get_bounding_box({bb1.first, bb1.second, bb2.first, bb2.second});
}

Pt<double> center_point(Pt<double> p, Pt<double> start, double ang) {
    return (p - start).rotate(-ang);
}

double get_x_centering(Pt<double> p, Pt<double> center, double ang) {
    return center_point(p, center, ang).x;
}

double get_y_centering(Pt<double> p, Pt<double> center, double ang) {
    return center_point(p, center, ang).y;
}
