//
// Created by nev on 11/2/16.
//

#ifndef ROBOT_PLANARSURFACE_H
#define ROBOT_PLANARSURFACE_H


#include "../Surface.h"
#include "PlanarPoint.h"

class PlanarSurface;

std::ostream &operator<<(std::ostream &output, PlanarSurface &surface);

class PlanarSurface : public Surface<PlanarPoint> {
private:
    PlanarPoint position;
    PlanarPoint finish;
    Topology top;
    std::size_t height, width;
    bool *visited;
    bool *passability;
    static PlanarPoint default_point;
    bool checkfix_point(PlanarPoint &p) const;

public:
    PlanarSurface(std::string filename, Topology topology = Topology::planar);
    ~PlanarSurface();


    virtual double move(PlanarPoint p) override;
    virtual std::vector<std::tuple<PlanarPoint, double>> lookup() const override;
    virtual PlanarPoint pos() const override;
    virtual bool done() const override;

    friend std::ostream &operator<<(std::ostream &output, PlanarSurface &surface);
};


#endif //ROBOT_PLANARSURFACE_H
