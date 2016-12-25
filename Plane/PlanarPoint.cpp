//
// Created by nev on 11/2/16.
//

#include <cmath>
#include <sstream>
#include "PlanarPoint.h"

PlanarPoint::PlanarPoint(int X, int Y) : x(X), y(Y) {}

std::vector<PlanarPoint> PlanarPoint::near() const {
    std::vector<PlanarPoint> res;
    res.push_back(PlanarPoint(x - 1, y));
    res.push_back(PlanarPoint(x + 1, y));
    res.push_back(PlanarPoint(x, y - 1));
    res.push_back(PlanarPoint(x, y + 1));
    return res;
}

bool PlanarPoint::operator==(const PlanarPoint &other) const {
    return (x == other.x && y == other.y);
}

double PlanarPoint::dist(const PlanarPoint &other, Topology top, int width, int height) const {
    if (top == Topology::tor && height < 0)
        top = Topology::cylinder;
    if (top == Topology::cylinder && width < 0)
        top = Topology::planar;
    switch (top) {
        case Topology::planar:
            return std::abs(x - other.x) + std::abs(y - other.y);
        case Topology::cylinder:
            return std::min(std::abs(x - other.x), width - std::abs(x - other.x)) +
                   std::abs(y - other.y);
        case Topology::tor:
            return std::min(std::abs(x - other.x), width - std::abs(x - other.x)) +
                   std::min(std::abs(y - other.y), height - std::abs(y - other.y));
    }
}

bool PlanarPoint::operator<(const PlanarPoint &other) const {
    return (x < other.x) || (x == other.x && y < other.y);
}

PlanarPoint::operator std::string() const {
    std::stringstream res;
    res << x << " " << y;
    return res.str();
}




