//
// Created by nev on 12/25/16.
//

#ifndef ROBOT_LANGPOINT_H
#define ROBOT_LANGPOINT_H

#include <vector>
#include <string>


class LangPoint {
protected:
    std::string word;
    std::string letters;
public:
    LangPoint(std::string Word = std::string(), std::string alphabet = std::string());

    virtual std::vector<LangPoint> near() const;

    virtual bool operator==(const LangPoint &other) const;

    virtual bool operator<(const LangPoint &other) const;

    virtual double dist(const LangPoint &other) const;

    operator std::string() const;

    friend class LangSurface;
};


#endif //ROBOT_LANGPOINT_H
