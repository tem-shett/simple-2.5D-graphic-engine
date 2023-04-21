#ifndef UNTITLED23_LINE_H
#define UNTITLED23_LINE_H

#include "point.h"

const double epsilon = 1e-3;

struct Line {
    double a, b, c;

    Line(double a_ = 1, double b_ = 0, double c_ = 0) : a(a_), b(b_), c(c_) {
        double now = hypot(a, b);
        a /= now;
        b /= now;
        c /= now;
    }

    double get_dist(Pt<double> p);

    Pt<double> intersection(Line ln2);
};

Line get_line_by_coeffs(std::vector<double> coeffs);

#endif //UNTITLED23_LINE_H
