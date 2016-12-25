//
// Created by nev on 12/25/16.
//

#ifndef ROBOT_LANGSURFACE_H
#define ROBOT_LANGSURFACE_H


#include <string>
#include <vector>
#include <tuple>
#include <set>
#include "LangSurface.h"
#include "LangPoint.h"
#include "../Surface.h"

extern std::string russian_alphabet;

class LangSurface : public Surface<LangPoint> {
private:
    LangPoint position;
    LangPoint finish;
    std::set<std::string> language;

    bool passability(LangPoint);

    std::vector<std::string> visited;
    std::string letters;
public:
    LangSurface(std::string dictionary_filename, std::string alphabet,
                std::string start_word, std::string finish_word);

    LangSurface(LangSurface &other, std::string start_word, std::string finish_word);

    virtual double move(LangPoint p) override;

    virtual std::vector<std::tuple<LangPoint, double>> lookup() override;

    virtual bool done() override;

    virtual LangPoint pos() const;

    friend std::ostream &operator<<(std::ostream &output, LangSurface &surface);
};


#endif //ROBOT_LANGSURFACE_H
