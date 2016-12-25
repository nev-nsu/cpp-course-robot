//
// Created by nev on 11/2/16.
//

#include <fstream>
#include <iostream>
#include <boost/log/trivial.hpp>
#include "PlanarSurface.h"

PlanarPoint PlanarSurface::default_point(-1, -1);

double PlanarSurface::move(PlanarPoint p) {
    if (!checkfix_point(p)) {
        BOOST_LOG_TRIVIAL(error) << "Moving error: point doesn't exist.";
        throw BadMove();
    }
    std::vector<PlanarPoint> cur = position.near();
    for (auto i : cur) {
        if (!checkfix_point(i)) continue;
        if (i == p) {
            if (passability[p.x + p.y * width]) {
                position = p;
                visited[p.x + width * p.y] = true;
                return p.dist(finish);
            } else {
                BOOST_LOG_TRIVIAL(error) << "Moving error: point is occupied.";
                throw BadMove();
            }
        }
    }
    BOOST_LOG_TRIVIAL(error) << "Moving error: point is unreachable.";
    throw BadMove();
}

std::vector<std::tuple<PlanarPoint, double>> PlanarSurface::lookup() const {
    std::vector<std::tuple<PlanarPoint, double>> res;
    auto cur = position.near();
    for (auto i : cur)
        if (checkfix_point(i))
            if (passability[i.x + i.y * width])
                res.push_back(std::make_tuple(i, i.dist(finish, top, (int) width, (int) height)));
    return res;
}

PlanarSurface::PlanarSurface(std::string filename, Topology topology)
        : finish(default_point), position(default_point), top(topology) {
    std::ifstream input(filename);
    if (!input) {
        BOOST_LOG_TRIVIAL(error) << "Reading error.\nFilename:" << filename;
        throw BadMap();
    }
    std::vector<std::string> field;
    std::string str;
    input >> str;
    while (input && !input.eof()) {
        field.push_back(str);
        input >> str;
    }
    input.close();
    height = field.size();
    width = str.size();
    BOOST_LOG_TRIVIAL(info) << "Reading done. Map [" << width << ", " << height << "]";
    passability = new bool[width * height];
    visited = new bool[width * height];
    for (std::size_t i = 0; i < height; i++)
        for (std::size_t j = 0; j < width; j++) {
            if (j == field[i].size()) {
                BOOST_LOG_TRIVIAL(error) << "Parsing error: too few symbols in line " << i + 1;
                throw BadMap();
            }
            visited[i * width + j] = (field[i][j] == 'S');
            switch (field[i][j]) {
                case 'S':
                    if (position != PlanarPoint(-1, -1)) {
                        BOOST_LOG_TRIVIAL(error) << "Parsing error: doubled start point.";
                        throw BadMap();
                    }
                    position = PlanarPoint((int) j, (int) i);
                case '.':
                    passability[i * width + j] = true;
                    break;
                case 'F':
                    if (finish != PlanarPoint(-1, -1)) {
                        BOOST_LOG_TRIVIAL(error) << "Parsing error: doubled finish point.";
                        throw BadMap();
                    }
                    passability[i * width + j] = true;
                    finish = PlanarPoint((int) j, (int) i);
                    break;
                case '#':
                    passability[i * width + j] = false;
                    break;
                default:
                    BOOST_LOG_TRIVIAL(error) << "Parsing error: unexpected symbol in the input file.";
                    throw BadMap();
            }
        }
    if (position == default_point || finish == default_point) {
        BOOST_LOG_TRIVIAL(error) << "Parsing error: start/finish point didn't found.";
        throw BadMap();
    } else
        BOOST_LOG_TRIVIAL(info) << "Parsing done. Surface successfully created.";
}

bool PlanarSurface::checkfix_point(PlanarPoint &p) const {
    switch (top) {
        case Topology::planar:
            if (p.x >= width || p.x < 0 || p.y >= height || p.y < 0)
                return false;
            return true;
        case Topology::cylinder:
            if (p.y >= height || p.y < 0)
                return false;
            p.x = (int) ((p.x + width) % width);
            return true;
        case Topology::tor:
            p.x = (int) ((p.x + width) % width);
            p.y = (int) ((p.y + height) % height);
            return true;
    }
}

PlanarSurface::~PlanarSurface() {
    delete[] visited;
    delete[] passability;
    BOOST_LOG_TRIVIAL(info) << "Surface successfully deleted.";
}

bool PlanarSurface::done() const {
    return position == finish;
}

std::ostream &operator<<(std::ostream &output, PlanarSurface &surface) {
    for (std::size_t i = 0; i < surface.height; i++) {
        for (std::size_t j = 0; j < surface.width; j++)
            if (surface.visited[i * surface.width + j])
                output << '*';
            else if (surface.passability[i * surface.width + j])
                output << '.';
            else output << '#';
        output << "\n";
    }
    return output;
}

PlanarPoint PlanarSurface::pos() const {
    return position;
}
