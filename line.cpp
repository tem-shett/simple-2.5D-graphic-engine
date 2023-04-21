#include "line.h"

double Line::get_dist(Pt<double> p) {
    return a * p.x + b * p.y + c;
}

Pt<double> Line::intersection(Line ln2) {
    double d = Pt<double>(a, b).cross(Pt<double>(ln2.a, ln2.b));
    double dx = Pt<double>(c, b).cross(Pt<double>(ln2.c, ln2.b));
    double dy = Pt<double>(a, c).cross(Pt<double>(ln2.a, ln2.c));
    return {-dx / d, -dy / d};
}

Line get_line_by_coeffs(std::vector<double> coeffs) {
    return {coeffs[0], coeffs[1], coeffs[2]};
}