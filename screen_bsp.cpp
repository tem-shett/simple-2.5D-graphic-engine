#include "screen_bsp.h"
#include <iostream>
#include <random>
#include <algorithm>

const double eps = 1e-6;

std::mt19937 rnd_scr(179);

std::vector<Pt<double>> pts_above, pts_under;

std::pair<std::vector<Pt<double>>, std::vector<Pt<double>>> cut(std::vector<Pt<double>> &pts, Line ln) {
    int n = pts.size();
//    std::vector<Pt<double>> pts_above, pts_under;
    pts_above = {}, pts_under = {};
    for (int i = 0; i < n; i++) {
        double dist1 = ln.get_dist(pts[i]), dist2 = ln.get_dist(pts[(i + 1) % n]);
        if (dist1 > -eps) {
            pts_above.push_back(pts[i]);
        }
        if (dist1 < eps) {
            pts_under.push_back(pts[i]);
        }
        if (std::abs(dist1) < eps || std::abs(dist2) < eps) {
            continue;
        }
        if ((dist1 < 0 && dist2 > 0) || (dist1 > 0 && dist2 < 0)) {
            auto pt_inter = ln.intersection(get_line_by_coeffs(pts[i].get_line(pts[(i + 1) % n])));
            pts_above.push_back(pt_inter);
            pts_under.push_back(pt_inter);
        }
    }
    return {pts_above, pts_under};
}

int check_ln(std::vector<Pt<double>> &pts, Line ln) {
    bool flag1 = true, flag2 = true;
    for (auto &p : pts) {
        double dist = ln.get_dist(p);
        if (dist > eps) {
            flag2 = false;
        }
        if (dist < -eps) {
            flag1 = false;
        }
    }
    return (flag1 ? 1 : (flag2 ? -1 : 0));
}

std::vector<std::vector<Pt<double>>> bsp_add_poly_(std::shared_ptr<Scr_bsp_node> node, std::vector<Pt<double>> &pts, std::vector<Line> &lns) {
    if ((int) pts.size() < 3 || node->filled) {
        return {};
    }
    if (!node->above && !node->under) {
        int n = (int) pts.size();
//        std::vector<int> mda(n);
//        std::iota(mda.begin(), mda.end(), 0);
//        std::shuffle(mda.begin(), mda.end(), rnd_scr);
        for (int i = 0; i < n; i++) {
            bool flag = true;
            for (auto ln : lns) {
                double dist1 = ln.get_dist(pts[i]), dist2 = ln.get_dist(pts[(i + 1) % n]);
                if (std::abs(dist1) < eps && std::abs(dist2) < eps) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                node->ln = get_line_by_coeffs(pts[i].get_line(pts[(i + 1) % n]));
                node->above = std::make_shared<Scr_bsp_node>(), node->under = std::make_shared<Scr_bsp_node>();
                node = node->above;
//                if (node->ln.get_dist(pts[(i + 2) % n]) > 0) {
//                } else {
//                    std::cout << 112 << std::endl;
//                    std::cout << node->ln.a << node->ln.b << node->ln.c << std::endl;
//                    for (auto el : pts) {
//                        std::cout << el.x << ' ' << el.y << std::endl;
//                    }
//                    node = node->under;
//                }
            }
        }
        node->filled = true;
        return {pts};
    }
    int check = check_ln(pts, node->ln);
    if (check == 1) {
        return bsp_add_poly_(node->above, pts, lns);
    } else if (check == -1) {
        return bsp_add_poly_(node->under, pts, lns);
    }
    auto cut_ = cut(pts, node->ln);
    if (std::min(cut_.first.size(), cut_.second.size()) >= 3) {
        lns.push_back(node->ln);
    }
    auto res1 = bsp_add_poly_(node->above, cut_.first, lns);
    auto res2 = bsp_add_poly_(node->under, cut_.second, lns);
    node->sz = node->above->sz + node->under->sz + 1;
    if (node->above->filled && node->under->filled) {
        node->filled = true;
    }
    if (std::min(cut_.first.size(), cut_.second.size()) >= 3) {
        lns.pop_back();
    }
    for (auto &el : res2) {
        res1.push_back(el);
    }
    return res1;
}

std::vector<std::vector<Pt<double>>> bsp_add_poly(const std::shared_ptr<Scr_bsp_node> &root, std::vector<Pt<double>> &pts) {
    std::vector<Line> lns;
    return bsp_add_poly_(root, pts, lns);
}

void reverse_filled(const std::shared_ptr<Scr_bsp_node> &node) {
    if (!node) {
        return;
    }
    if (!node->above && !node->under) {
        node->filled = !node->filled;
    }
    reverse_filled(node->above);
    reverse_filled(node->under);
}

std::shared_ptr<Scr_bsp_node> create_scr_bsp() {
    auto root = std::make_shared<Scr_bsp_node>();
    std::vector<Pt<double>> pts = {Pt<double>(0, 0), Pt<double>(screenW - 1, 0), Pt<double>(screenW - 1, screenH - 1), Pt<double>(0, screenH - 1)};
    bsp_add_poly(root, pts);
    reverse_filled(root);
    return root;
}
