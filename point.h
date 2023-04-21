#ifndef UNTITLED23_POINT_H
#define UNTITLED23_POINT_H

#include <cmath>
#include <vector>
#include <tuple>

template<typename Type>
struct Pt {
    Type x, y;

    Pt(Type x_ = 0, Type y_ = 0) : x(x_), y(y_) {}

    Pt operator-(Pt other) const {
        return {x - other.x, y - other.y};
    }

    Pt operator+(Pt other) const {
        return {x + other.x, y + other.y};
    }

    Pt operator*(double c) const {
        return {x * c, y * c};
    }

    Pt operator/(double c) const {
        return {x / c, y / c};
    }

    bool operator==(Pt other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(Pt other) const {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }

    Type dot(Pt other) {
        return x * other.x + y * other.y;
    }

    Type cross(Pt other) {
        return x * other.y - y * other.x;
    }

    Pt normal() {
        return {-y, x};
    }

    Pt len1() {
        double len = dist();
        return {x / len, y / len};
    }

    Pt<double> dbl() {
        return {(double) x, (double) y};
    }

    Pt rotate(double ang) {
        double sin_ = sin(ang);
        double cos_ = cos(ang);
        return Pt<double>(x * cos_ - y * sin_, x * sin_ + y * cos_);
    }

    double dist() {
        return hypot(x, y);
    }

    std::vector<double> get_line(Pt p2) {
        double a = y - p2.y;
        double b = p2.x - x;
        double c = cross(p2);
        return {a, b, c};
    }
};

bool pt_in_angle(Pt<double> p, Pt<double> p_a1, Pt<double> p_a2);

bool pt_on_seg(Pt<double> p, Pt<double> seg_pt1, Pt<double> seg_pt2);

std::pair<Pt<double>, Pt<double>> get_bounding_box(std::vector<Pt<double>> pts);

std::pair<Pt<double>, Pt<double>> get_bounding_box(std::pair<Pt<double>, Pt<double>> p1, std::pair<Pt<double>, Pt<double>> p2);

Pt<double> center_point(Pt<double> p, Pt<double> start, double ang);

double get_x_centering(Pt<double> p, Pt<double> center, double ang);

double get_y_centering(Pt<double> p, Pt<double> center, double ang);

#endif //UNTITLED23_POINT_H
