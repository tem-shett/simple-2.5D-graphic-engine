#include "gameobjects.h"

void Player::change_real_speed(float time_pass) {
    double a = accel * time_pass / 3;
    if (ch_x > real_ch_x) {
        real_ch_x = std::min(real_ch_x + a, ch_x);
    } else {
        real_ch_x = std::max(real_ch_x - a, ch_x);
    }
    a *= 2;
    if (ch_y > real_ch_y) {
        real_ch_y = std::min(real_ch_y + a, ch_y);
    } else {
        real_ch_y = std::max(real_ch_y - a, ch_y);
    }
}

const double eps = 1e-4;

Floor Floor::cut(Line ln, bool is_above) {
    std::vector<Pt<double>> npts;
    int n = (int) pts.size();
    for (int i = 0; i < n; i++) {
        npts.push_back(pts[i]);
        double dist1 = ln.get_dist(pts[i]), dist2 = ln.get_dist(pts[(i + 1) % n]);
        if (std::abs(dist1) < eps || std::abs(dist2) < eps) {
            continue;
        }
        if ((dist1 < 0 && dist2 > 0) || (dist1 > 0 && dist2 < 0)) {
            auto pt_inter = ln.intersection(get_line_by_coeffs(pts[i].get_line(pts[(i + 1) % n])));
            npts.push_back(pt_inter);
//            ok.push_back(Pt<double>(round(pt_inter.x), round(pt_inter.y)));
        }
    }
    std::vector<Pt<double>> res_pts;
    for (auto pt : npts) {
        double d = ln.get_dist(pt);
        if ((d > -eps && is_above) || (d < eps && !is_above)) {
            res_pts.push_back(pt);
        }
    }
    return Floor(res_pts, h, color);
}