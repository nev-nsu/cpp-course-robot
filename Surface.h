//
// Created by nev on 11/2/16.
//

#ifndef ROBOT_SURFACE_H
#define ROBOT_SURFACE_H


#include <vector>
#include <tuple>

class BadMove : public std::exception {
};

class BadMap : public std::exception {
};

template<typename P>
class Surface {
public:
    virtual double move(P p) = 0;

    virtual std::vector<std::tuple<P, double>> lookup() = 0;

    virtual bool done() = 0;

    virtual P pos() const = 0;
};

template<typename P>
double get_dist(std::tuple<P, double> t) {
    return std::get<1>(t);
}

template<typename P>
P get_point(std::tuple<P, double> t) {
    return std::get<0>(t);
}


#endif //ROBOT_SURFACE_H
