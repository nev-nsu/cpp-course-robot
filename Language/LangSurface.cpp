//
// Created by nev on 12/25/16.
//

#include <boost/log/trivial.hpp>
#include <fstream>
#include "LangSurface.h"

std::string russian_alphabet = "абвгдеёжзийклмнопрстуфхцчшщыъэьюя";

bool LangSurface::done() const {
    return position == finish;
}

std::vector<std::tuple<LangPoint, double>> LangSurface::lookup() const {
    auto tmp = position.near();
    std::vector<std::tuple<LangPoint, double>> res;
    for (auto i : tmp)
        if (passability(i))
            res.push_back(std::make_tuple(i, i.dist(finish)));
    return res;
}

double LangSurface::move(LangPoint p) {
    if (p.letters != letters) {
        BOOST_LOG_TRIVIAL(error) << "Moving error : the point is from other surface";
        throw BadMove();
    }
    auto tmp = position.near();
    tmp.push_back(position);
    for (auto i : tmp)
        if (i == p) {
            if (passability(i)) {
                if (!(position == p))
                    visited.push_back(std::string(p));
                position = p;
                return p.dist(finish);
            } else {
                BOOST_LOG_TRIVIAL(error) << "Moving error: point is occupied.";
                throw BadMove();
            }
        }
    BOOST_LOG_TRIVIAL(error) << "Moving error: point is unreachable.";
    throw BadMove();
}

LangSurface::LangSurface(std::string dictionary_filename, std::string alphabet,
                         std::string start_word, std::string finish_word) :
        letters(alphabet), position(start_word, alphabet), finish(finish_word, alphabet) {
    language.clear();
    std::ifstream in(dictionary_filename);
    if (!in) {
        BOOST_LOG_TRIVIAL(error) << "Reading error.\nFilename:" << dictionary_filename;
        throw BadMap();
    }
    std::string buf;
    std::getline(in, buf);
    while (in && !in.eof()) {
        language.insert(buf);
        std::getline(in, buf);
    }
    BOOST_LOG_TRIVIAL(info) << "Reading done (" << dictionary_filename << ").";
    if (!passability(position) || !passability(finish)) {
        BOOST_LOG_TRIVIAL(error) << "Parsing error: start(" << std::string(position)
                                 << ") or finish(" << std::string(finish) << ") word doesn't exist";
        throw BadMap();
    }
    visited.push_back(start_word);
    BOOST_LOG_TRIVIAL(info) << "Parsing done. Surface successfully created.";
}


std::ostream &operator<<(std::ostream &output, LangSurface &surface) {
    output << "Current position : " << std::string(surface.position)
           << "\nFinish word: " << std::string(surface.finish)
           << "\nVisited [" << surface.visited.size() << "]:\n";
    for (auto i : surface.visited)
        output << "\t" << i << "\n";
    return output;
}

bool LangSurface::passability(LangPoint p) const {
    bool res = (language.find(std::string(p)) == language.end());
    return !res;
}

LangPoint LangSurface::pos() const {
    return position;
}

LangSurface::LangSurface(LangSurface &other, std::string start_word, std::string finish_word) :
        letters(other.letters), language(other.language),
        position(start_word, other.letters), finish(finish_word, other.letters) {
    visited.clear();
}
