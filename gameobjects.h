#ifndef UNTITLED23_GAMEOBJECTS_H
#define UNTITLED23_GAMEOBJECTS_H

#include <cmath>
#include "line.h"
#include "texture.h"

const double pi = acos(-1);

struct Wall {
    Pt<double> p1, p2;
    double h1, h2;
    Texture *texture;
    double left_cut = 0, right_cut = 0;
    Line ln;
    int ind = -1;

    Wall(Pt<double> p1_, Pt<double> p2_, double h1_, double h2_, Texture *texture1) : p1(p1_), p2(p2_), h1(h1_),
                                                                                      h2(h2_), texture(texture1) {
        ln = get_line_by_coeffs(p1.get_line(p2));
    }
};

struct Floor {
    std::vector<Pt<double>> pts;
    double h;
    uint32_t color;

    Floor(std::vector<Pt<double>> pts_, double h_, uint32_t color_): pts(pts_), h(h_), color(color_) {}

    Floor cut(Line ln, bool is_above);
};

struct Player {
    Pt<double> pos;
    double eye_angle;
    double h;
    double ch_x = 0;
    double ch_y = 0;
    double real_ch_x = 0, real_ch_y = 0;
    double ch_h = 0;
    double ch_ang = 0;
    double h_under_eye = 3;
    double h_above_eye = 1;
    double speed = 3;
    double accel = 50;
    double min_dist_from_wall = 0.5;

    Player(Pt<double> pos_, double eye_angle_ = 0.1, double h_ = 5) : pos(pos_), eye_angle(eye_angle_), h(h_) {}

    void change_real_speed(float time_pass);
};

struct NPC {
    Pt<double> pos;
    double h;
    double height, width;
    Texture *texture;
    double left_cut = 0, right_cut = 0;
    double speed = 10;

    NPC(Pt<double> pos_, double h_, double height_, double width_, Texture *texture1) : pos(pos_), h(h_),
                                                                                        height(height_), width(width_),
                                                                                        texture(texture1) {}
};

#endif //UNTITLED23_GAMEOBJECTS_H
