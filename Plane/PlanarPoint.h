//
// Created by nev on 11/2/16.
//

#ifndef ROBOT_PLANARPOINT_H
#define ROBOT_PLANARPOINT_H

enum class Topology {
    planar,
    cylinder,
    tor
};

#include <vector>
#include <string>

class PlanarPoint {
protected:
    int x;
    int y;
public:
    PlanarPoint(int X = -1, int Y = -1);

    virtual std::vector<PlanarPoint> near() const;
    virtual double dist(const PlanarPoint &other, Topology top = Topology::planar, int width = 0, int height = 0) const;

    virtual bool operator==(const PlanarPoint &other) const;
    virtual bool operator!=(const PlanarPoint &other) const;
    virtual bool operator<(const PlanarPoint &other) const;
    virtual bool operator>(const PlanarPoint &other) const;

    operator std::string() const;

    friend class PlanarSurface;
};


#endif //ROBOT_PLANARPOINT_H
