//
// Created by nev on 12/25/16.
//

#include <iostream>
#include "../utility/lev_distance.h"
#include "LangPoint.h"

LangPoint::LangPoint(std::string Word, std::string alphabet) : word(Word), letters(alphabet) {}

std::vector<LangPoint> LangPoint::near() const {
    std::vector<LangPoint> res;
    // replace
    for (std::size_t i = 0; i < word.size(); i += 2)
        for (std::size_t j = 0; j < letters.size(); j += 2)
            if (word[i] != j) {
                std::string tmp(word);
                tmp.replace(i, 2, letters.substr(j, 2));
                res.push_back(LangPoint(tmp, letters));
            }
    // insert
    for (int i = 0; i <= word.size(); i++)
        for (int j = 0; j < letters.size(); j++) {
            std::string tmp(word);
            tmp.insert((unsigned long) i, 1, letters[j]);
            res.push_back(LangPoint(tmp, letters));
        }
    // delete
    for (int i = 0; i < word.size(); i++) {
        std::string tmp(word);
        tmp.erase((unsigned long) i, 1);
        res.push_back(LangPoint(tmp, letters));
    }
    return res;
}

bool LangPoint::operator==(const LangPoint &other) const {
    return word == other.word;
}

bool LangPoint::operator<(const LangPoint &other) const {
    return word < other.word;
}

double LangPoint::dist(const LangPoint &other) const {
    std::vector<uint16_t> s, t;
    for (int i = 0; i < word.size(); i += 2)
        s.push_back(*reinterpret_cast<const uint16_t *>(word.c_str() + i));
    for (int i = 0; i < other.word.size(); i += 2)
        t.push_back(*reinterpret_cast<const uint16_t *>(other.word.c_str() + i));
    return LevenshteinDistance<uint16_t>(s, t);
}

LangPoint::operator std::string() const {
    return word;
}

