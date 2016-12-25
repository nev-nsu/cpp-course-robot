//
// Created by nev on 12/24/16.
//

#ifndef ROBOT_SOLVER_H
#define ROBOT_SOLVER_H


#include <set>
#include <boost/log/trivial.hpp>
#include <algorithm>

#include "Surface.h"

template<class P>
class Solver {
private:
    Surface<P> *surface;
    P position;
    std::set<P> visited;
    std::vector<P> path;

    bool dfs(P p, int &deep);
    void sort(std::vector<std::tuple<P, double>> &array);
public:
    Solver(Surface<P> *used_surface);

    bool solve(int limit = 1000);
    std::vector<P> get_path();
};

template<class P>
bool Solver<P>::solve(int limit) {
    if (surface->done())
        return true;
    position = surface->pos();
    BOOST_LOG_TRIVIAL(info) << "Solving started. Using method : DFS.";
    bool result = dfs(position, limit);
    BOOST_LOG_TRIVIAL(info) << "Solving ended.";
    return result;
}

template<class P>
Solver<P>::Solver(Surface<P> *used_surface)
        : surface(used_surface) {
    if (surface == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Solver error : bad surface";
        throw BadMap();
    }
}

template<class P>
bool Solver<P>::dfs(P p, int &deep) {
    visited.insert(p);
    path.push_back(p);
    if (surface->done())
        return true;
    if (!deep) return false;
    auto pos = surface->lookup();
    this->sort(pos);
    for (auto j : pos) {
        P i = get_point(j);
        if (visited.find(i) == visited.end()) {
            surface->move(i);
            deep--;
            if (dfs(i, deep))
                return true;
            if (deep < 2)
                return false;
            surface->move(p);
            deep--;
        }
    }
    path.pop_back();
    return false;
}

template<class P>
void Solver<P>::sort(std::vector<std::tuple<P, double>> &array) {
    auto cmp = [](std::tuple<P, double> a, std::tuple<P, double> b) {
        return get_dist(a) < get_dist(b);
    };
    std::sort(array.begin(), array.end(), cmp);
}

template<class P>
std::vector<P> Solver<P>::get_path() {
    return path;
}

#endif //ROBOT_SOLVER_H
