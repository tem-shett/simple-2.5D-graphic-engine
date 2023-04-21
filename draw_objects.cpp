#include <algorithm>
#include "draw_objects.h"
#include "iostream"
#include "line.h"
#include "geom_funcs.h"
#include "screen_bsp.h"

Pt<double> get_coords_on_screen(Pt<double> p, double h) {
    return {(half_scW + (p.y / p.x) * half_scW), (half_scH + (h / p.x) * half_scH)};
}

template<typename Type>
Type get_y(Pt<Type> p1, Pt<Type> p2, Type x) {
    return (x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;
}

void draw_wall(Wall wall, Player pl, FrameBuffer &fb, const std::shared_ptr<Scr_bsp_node> &scr_root, bool light_dep = true) {
    wall.p1 = center_point(wall.p1, pl.pos, pl.eye_angle);
    wall.p2 = center_point(wall.p2, pl.pos, pl.eye_angle);
    wall.h1 -= pl.h, wall.h2 -= pl.h;
    if (wall.p1.y < wall.p2.y) {
        std::swap(wall.p1, wall.p2);
        std::swap(wall.left_cut, wall.right_cut);
    }
    {
        Pt<double> vec1(1, 1), vec2(1, -1);
        Line side1(1, -1, 0), side2(1, 1, 0);
        Line ln_wall = get_line_by_coeffs(wall.p1.get_line(wall.p2));
        auto pt_side1 = side1.intersection(ln_wall), pt_side2 = side2.intersection(ln_wall);
        bool f1 = pt_in_angle(wall.p1, vec1, vec2);
        bool f2 = pt_in_angle(wall.p2, vec1, vec2);
        if (f1 && f2) {
        } else if (!f1 && !f2) {
            if (pt_side1.x > 0 && pt_on_seg(pt_side1, wall.p1, wall.p2)) {
                double left_cut = (pt_side1.x - wall.p1.x) / (wall.p2.x - wall.p1.x);
                double prev_p = (1 - wall.left_cut - wall.right_cut);
                wall.left_cut += prev_p * left_cut;
                double right_cut = (wall.p2.x - pt_side2.x) / (wall.p2.x - wall.p1.x);
                wall.right_cut += prev_p * right_cut;
                wall.p1 = pt_side1;
                wall.p2 = pt_side2;
            } else {
                return;
            }
        } else if (f1 && !f2) {
            if (pt_side2.x < 0) {
                std::swap(pt_side1, pt_side2);
            }
            double right_cut = (wall.p2.x - pt_side2.x) / (wall.p2.x - wall.p1.x);
            wall.right_cut += (1 - wall.left_cut - wall.right_cut) * right_cut;
            wall.p2 = pt_side2;
        } else {
            if (pt_side1.x < 0) {
                std::swap(pt_side1, pt_side2);
            }
            double left_cut = (pt_side1.x - wall.p1.x) / (wall.p2.x - wall.p1.x);
            wall.left_cut += (1 - wall.left_cut - wall.right_cut) * left_cut;
            wall.p1 = pt_side1;
        }
    }
//    std::cout << wall.left_cut << ' ' << wall.right_cut << std::endl;
    auto p1 = get_coords_on_screen(wall.p1, wall.h1), p2 = get_coords_on_screen(wall.p1, wall.h2);
    auto p3 = get_coords_on_screen(wall.p2, wall.h1), p4 = get_coords_on_screen(wall.p2, wall.h2);
    if (p1.x == p3.x) {
        return;
    }
    if (p1.x > p3.x) {
        std::swap(p1, p3);
        std::swap(p2, p4);
        std::swap(wall.p1, wall.p2);
        std::swap(wall.left_cut, wall.right_cut);
    }
    auto xx1 = p1.x, yy1_b = p1.y, yy1_t = p2.y;
    auto xx2 = p3.x, yy2_b = p3.y, yy2_t = p4.y;
    std::vector<Pt<double>> poly = {p2.dbl(), p1.dbl(), p3.dbl(), p4.dbl()};
    auto vec_pts = bsp_add_poly(scr_root, poly);
//    for (auto el : vec_pts) {
//        for (auto el2 : el) {
//            std::cout << el2.x << ' ' << el2.y << ' ';
//        }
//        std::cout << std::endl;
//    }
//    std::cout << std::endl;
//    std::cout << std::endl;
    for (auto &pts : vec_pts) {
        int n = pts.size();
        int ii = 0;
        int mnx = screenW - 1, mxx = 0;
        for (auto &pt : pts) {
            pt.x = round(pt.x);
        }
        for (int i = 0; i < n; i++) {
            if (pts[i].x < pts[ii].x) {
                ii = i;
            }
            mnx = std::min(mnx, (int) ceil(pts[i].x));
            mxx = std::max(mxx, (int) pts[i].x);
        }
        mxx = std::min(mxx, screenW - 1);
        mnx = std::max(mnx, 0);
        int bi = ii, ti = ii;
        for (int x = mnx; x <= mxx; x++) {
            while (pts[bi].x < x && pts[(bi + 1) % n].x >= pts[bi].x) {
                bi = (bi + 1) % n;
            }
            while (pts[ti].x < x && pts[(ti + n - 1) % n].x >= pts[ti].x) {
                ti = (ti + n - 1) % n;
            }
            int ty_poly = get_y(pts[ti], pts[(ti + 1) % n], (double) x);
            int by_poly = get_y(pts[bi], pts[(bi + n - 1) % n], (double) x);
            if (pts[bi].x == x && pts[ti].x == x) {
                by_poly = pts[bi].y;
                ty_poly = pts[ti].y;
            }
            {
                int y_b = (x - xx1 + 1) * (yy2_b - yy1_b) / (xx2 - xx1 + 1) + yy1_b;
                int y_t = (x - xx1 + 1) * (yy2_t - yy1_t) / (xx2 - xx1 + 1) + yy1_t;
                double c_light_beta = get_y(Pt<double>(xx1, 1.0 / wall.p1.x), Pt<double>(xx2, 1.0 / wall.p2.x), (double) x);
                double realx = 1.0 / c_light_beta;
                double xp = get_y(Pt<double>(wall.p1.x, 0), Pt<double>(wall.p2.x, 1), realx);
                xp = std::min(1.0, std::max(xp, 0.0));
                xp = xp * (1 - wall.left_cut - wall.right_cut) + wall.left_cut;
                double c_light = std::max(MIN_LIGHT_C, std::min(1.0, 40 * c_light_beta));
//                double c_light = 1;
                for (int y = by_poly; y <= ty_poly; y++) {
                    double yp = (y - y_b) / (double) (y_t - y_b);
                    yp = std::min(1.0, std::max(yp, 0.0));
                    uint32_t color = 0;
                    if (!light_dep || c_light == 1.0) {
                        color = wall.texture->get_color(xp, yp);
                    } else if (c_light == MIN_LIGHT_C) {
                        color = wall.texture->get_color_low_light(xp, yp);
                    } else {
                        color = ch_light(wall.texture->get_color(xp, yp), c_light);
                    }
                    fb.set_pixel(x, y, color);
//                    fb.set_pixel(x, y, wall.texture->get_color(xp, yp));
                }
            }

        }
    }
    /*
    for (int x = std::max(xx1, 0); x <= std::min(xx2, screenW - 1); x++) {
        int y_b = (x - xx1 + 1) * (yy2_b - yy1_b) / (xx2 - xx1 + 1) + yy1_b;
        int y_t = (x - xx1 + 1) * (yy2_t - yy1_t) / (xx2 - xx1 + 1) + yy1_t;
        double realx = 1.0 / get_y(Pt<double>(xx1, 1.0 / wall.p1.x), Pt<double>(xx2, 1.0 / wall.p2.x), (double) x);
        double xp = get_y(Pt<double>(wall.p1.x, 0), Pt<double>(wall.p2.x, 1), realx);
        xp = std::min(1.0, std::max(xp, 0.0));
        xp = xp * (1 - wall.left_cut - wall.right_cut) + wall.left_cut;
        for (int y = std::max(y_b, 0); y <= std::min(y_t, screenH - 1); y = fb.get_nexty(x, y)) {
            double yp = (y - y_b) / (double) (y_t - y_b);
            yp = std::min(1.0, std::max(yp, 0.0));
            fb.set_pixel(x, y, wall.texture->get_color(xp, yp));
        }
    }
    */
}

void sort_vec_pts(std::vector<Pt<double>> &pts) {
    std::sort(pts.begin(), pts.end());
    Pt<double> mn = pts[0];
    std::sort(pts.begin() + 1, pts.end(), [mn](Pt<double> a, Pt<double> b) { return (a - mn).cross(b - mn) > 0;});

    std::vector<Pt<double>> s = {pts[0], pts[1]};
    for (int i = 2; i < (int) pts.size(); i++) {
        int k = (int) s.size();
        while (k > 1 && (s[k - 1] - s[k - 2]).cross(pts[i] - s[k - 1]) <= 0) {
            s.pop_back();
            k = (int) s.size();
        }
        s.push_back(pts[i]);
    }
    pts = s;

}

void draw_floor(Floor fl, Player pl, FrameBuffer &fb, std::shared_ptr<Scr_bsp_node> scr_root) {
    {
        for (auto &pt : fl.pts) {
            pt = center_point(pt, pl.pos, pl.eye_angle);
        }
        fl.h -= pl.h;
        Line side1(1, -1, 0), side2(1, 1, 0);
        fl = fl.cut(side1, true).cut(side2, true).cut(Line(1, 0, -0.1), true);
        if ((int) fl.pts.size() < 3) {
            return;
        }
    }
//    sort_vec_pts(fl.pts);
//    for (auto el : fl.pts) {
//        std::cout << el.x << ' ' << el.y << ' ';
//    }
//    std::cout << std::endl;
    std::vector<Pt<double>> poly = {};
    for (auto &pt : fl.pts) {
        poly.push_back(get_coords_on_screen(pt, fl.h).dbl());
    }
    sort_vec_pts(poly);
    auto vec_pts = bsp_add_poly(scr_root, poly);
//    for (auto el : vec_pts) {
//        for (auto el2 : el) {
//            std::cout << el2.x << ' ' << el2.y << ' ';
//        }
//        std::cout << std::endl;
//    }
//    std::cout << std::endl;
    for (auto &pts : vec_pts) {
        int n = pts.size();
        int ii = 0;
        int mnx = screenW - 1, mxx = 0;
        for (auto &pt : pts) {
            pt.x = round(pt.x);
        }
        for (int i = 0; i < n; i++) {
            if (pts[i].x < pts[ii].x) {
                ii = i;
            }
            mnx = std::min(mnx, (int) ceil(pts[i].x));
            mxx = std::max(mxx, (int) pts[i].x);
        }
        mxx = std::min(mxx, screenW - 1);
        mnx = std::max(mnx, 0);
        int bi = ii, ti = ii;
        for (int x = mnx; x <= mxx; x++) {
            while (pts[bi].x < x && pts[(bi + 1) % n].x >= pts[bi].x) {
                bi = (bi + 1) % n;
            }
            while (pts[ti].x < x && pts[(ti + n - 1) % n].x >= pts[ti].x) {
                ti = (ti + n - 1) % n;
            }
            int ty_poly = get_y(pts[ti], pts[(ti + 1) % n], (double) x);
            int by_poly = get_y(pts[bi], pts[(bi + n - 1) % n], (double) x);
//            by_poly = std::max(0, by_poly);
            if (pts[bi].x == x && pts[ti].x == x) {
                by_poly = pts[bi].y;
                ty_poly = pts[ti].y;
            }
            for (int y = by_poly; y <= ty_poly; y++) {
                fb.set_pixel(x, y, fl.color);
            }
        }
    }
}

void draw_npc(NPC npc, Player pl, FrameBuffer &fb, std::shared_ptr<Scr_bsp_node> scr_root) {
    npc.pos = center_point(npc.pos, pl.pos, pl.eye_angle);
    npc.h -= pl.h;
    if (npc.pos.x <= 0) {
        return;
    }
    auto pt_on_screen1 = get_coords_on_screen(npc.pos, npc.h), pt_on_screen2 = get_coords_on_screen(npc.pos,
                                                                                                    npc.h + npc.height);
    auto kek = get_coords_on_screen(npc.pos + Pt<double>(0, npc.width / 2), npc.h);
    auto kek2 = get_coords_on_screen(npc.pos - Pt<double>(0, npc.width / 2), npc.h);
    int x1 = kek2.x, x2 = kek.x;
    int y1 = pt_on_screen1.y, y2 = pt_on_screen2.y;
    std::vector<Pt<double>> poly = {Pt<double>(x1, y1), Pt<double>(x2, y1), Pt<double>(x2, y2), Pt<double>(x1, y2)};
    auto vec_pts = bsp_add_poly(scr_root, poly);
    for (auto &pts : vec_pts) {
        int n = pts.size();
        int ii = 0;
        int mnx = screenW - 1, mxx = 0;
        for (auto &pt : pts) {
            pt.x = round(pt.x);
        }
        for (int i = 0; i < n; i++) {
            if (pts[i].x < pts[ii].x) {
                ii = i;
            }
            mnx = std::min(mnx, (int) ceil(pts[i].x));
            mxx = std::max(mxx, (int) pts[i].x);
        }
        mxx = std::min(mxx, screenW - 1);
        mnx = std::max(mnx, 0);
        int bi = ii, ti = ii;
        for (int x = mnx; x <= mxx; x++) {
            while (pts[bi].x < x && pts[(bi + 1) % n].x >= pts[bi].x) {
                bi = (bi + 1) % n;
            }
            while (pts[ti].x < x && pts[(ti + n - 1) % n].x >= pts[ti].x) {
                ti = (ti + n - 1) % n;
            }
            int ty_poly = get_y(pts[ti], pts[(ti + 1) % n], (double) x);
            int by_poly = get_y(pts[bi], pts[(bi + n - 1) % n], (double) x);
            if (pts[bi].x == x && pts[ti].x == x) {
                by_poly = pts[bi].y;
                ty_poly = pts[ti].y;
            }
            double xp = (x - x1) / (double) (x2 - x1);
            xp = xp * (1 - npc.left_cut - npc.right_cut) + npc.left_cut;
            for (int y = by_poly; y <= ty_poly; y++) {
                double yp = (y - y1) / (double) (y2 - y1);
                fb.set_pixel(x, y, npc.texture->get_color(xp, yp));
            }
        }
    }

    for (int x = std::max(x1, 0); x <= std::min(x2, screenW - 1); x++) {

    }
}

void dfs_bsp(const std::shared_ptr<BSP_node> &node, Player &pl, std::vector<NPC> &npcs, FrameBuffer &fb, Visible_angle &v_a, std::shared_ptr<Scr_bsp_node> scr_root) {
    if (node->cur_walls.empty()) {
        for (auto npc : npcs) {
            draw_npc(npc, pl, fb, scr_root);
        }
        std::sort(node->cur_floors.begin(), node->cur_floors.end(), [pl](const Floor &a, const Floor &b){return std::abs(a.h - pl.h) < std::abs(b.h - pl.h);});
        for (auto &fl : node->cur_floors) {
            draw_floor(fl, pl, fb, scr_root);
        }
        return;
    }
    // divide npcs in two groups
    std::vector<NPC> npc_above, npc_under;
    for (auto npc : npcs) {
        auto p1 = npc.pos + v_a.norm1 * (npc.width / 2), p2 = npc.pos - v_a.norm1 * (npc.width / 2);
        double val1 = node->ln.get_dist(p1), val2 = node->ln.get_dist(p2);
        if ((val1 < 0 && val2 > 0) || (val1 > 0 && val2 < 0)) {
            Pt<double> p_int = node->ln.intersection(get_line_by_coeffs(p1.get_line(p2)));
            NPC npc1 = npc, npc2 = npc;
            npc1.pos = (p_int + p1) / 2, npc2.pos = (p_int + p2) / 2;
            double right_cut = (p_int - p1).dist() / (p1 - p2).dist();
            double left_cut = (p_int - p2).dist() / (p1 - p2).dist();
            npc2.right_cut += (1 - npc.right_cut - npc.left_cut) * right_cut;
            npc2.width = npc.width * (1 - right_cut);
            npc1.left_cut += (1 - npc.left_cut - npc.right_cut) * left_cut;
            npc1.width = npc.width * (1 - left_cut);
            if (val1 > 0) {
                std::swap(npc1, npc2);
            }
            npc_above.push_back(npc1);
            npc_under.push_back(npc2);
        } else if (node->ln.get_dist(npc.pos) < 0) {
            npc_above.push_back(npc);
        } else {
            npc_under.push_back(npc);
        }
    }
    if (node->cnt_walls > 20 && npcs.empty()) {
        if (!check_bb_in_angle(pl, node->bb, v_a)) {
            return;
        }
    }
    bool flag = node->ln.get_dist(pl.pos) < 0;
    dfs_bsp(flag ? node->under : node->above, pl, flag ? npc_above : npc_under, fb, v_a, scr_root);
    for (auto wall : node->cur_walls) {
        draw_wall(wall, pl, fb, scr_root);
    }
    dfs_bsp(!flag ? node->under : node->above, pl, !flag ? npc_above : npc_under, fb, v_a, scr_root);
}

void draw_frame(GameHandler &gh, FrameBuffer &fb) {
    fb.clear();
    Pt<double> v1(1, 1), v2(1, -1), vc(1, 0);
    double ang = gh.player.eye_angle;
    Visible_angle v_a(v1.rotate(ang), v2.rotate(ang), vc.rotate(ang));
    std::sort(gh.enemies.begin(), gh.enemies.end(),
              [gh](NPC a, NPC b) { return (a.pos - gh.player.pos).dist() < (b.pos - gh.player.pos).dist(); });
    auto scr_root = create_scr_bsp();
    dfs_bsp(gh.root, gh.player, gh.enemies, fb, v_a, scr_root);
//    std::cout << "scr_root size: " << scr_root->sz << std::endl;
    for (auto &wall : gh.bg_walls) {
        draw_wall(wall, gh.player, fb, scr_root, false);
    }
}

/*
void draw_npc(NPC npc, Player pl, FrameBuffer &fb) {
    npc.pos = center_point(npc.pos, pl.pos, pl.eye_angle);
    npc.h -= pl.h;
    auto pt_on_screen1 = get_coords_on_screen(npc.pos, npc.h), pt_on_screen2 = get_coords_on_screen(npc.pos,
                                                                                                    npc.h + npc.height);
    auto kek = get_coords_on_screen(npc.pos + Pt<double>(0, npc.width / 2), npc.h);
    auto kek2 = get_coords_on_screen(npc.pos - Pt<double>(0, npc.width / 2), npc.h);
    int x1 = kek2.x, x2 = kek.x;
    int y1 = pt_on_screen1.y, y2 = pt_on_screen2.y;
    for (int x = std::max(x1, 0); x <= std::min(x2, screenW - 1); x++) {
        double xp = (x - x1) / (double) (x2 - x1);
        xp = xp * (1 - npc.left_cut - npc.right_cut) + npc.left_cut;
        for (int y = std::max(y1, 0); y <= std::min(y2, screenH - 1); y++) {
            double yp = (y - y1) / (double) (y2 - y1);
            fb.set_pixel(x, y, npc.texture->get_color(xp, yp));
        }
    }
}
 */