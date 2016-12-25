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
    size_t height, width;
    bool *visited;
    bool *passability;

    bool checkfix_point(PlanarPoint &p);

public:
    PlanarSurface(std::string filename, Topology topology = Topology::planar);

    virtual double move(PlanarPoint p) override;

    virtual std::vector<std::tuple<PlanarPoint, double>> lookup() override;

    virtual PlanarPoint pos() const;

    virtual bool done() override;

    ~PlanarSurface();

    friend std::ostream &operator<<(std::ostream &output, PlanarSurface &surface);
};


#endif //ROBOT_PLANARSURFACE_H
