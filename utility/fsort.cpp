#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include "fsort.h"

void halt(const char *str) {
    std::cerr << str << std::endl;
    exit(EXIT_FAILURE);
}

void fgetlines(const char *filename, std::vector<std::string> &dist) {
    std::ifstream in(filename);
    std::string s;
    if (in.fail())
        halt("Reading error\nPlease, check the filename");
    while (!in.eof()) {
        getline(in, s);
        dist.push_back(s);
    }
    while (!dist.empty() && dist.back().empty())
        dist.pop_back();
}

void fputlines(const char *filename, const std::vector<std::string> &src) {
    std::ofstream out(filename);
    if (out.fail())
        halt("Writing error\nPlease, check your permissions");
    for (auto i : src)
        out << i << std::endl;
}

void fsort(const char *input_file, const char *output_file) {
    std::vector<std::string> list;
    fgetlines(input_file, list);
    sort(list.begin(), list.end());
    fputlines(output_file, list);
}

bool fcmp(const char *file1, const char *file2) {
    std::vector<std::string> v1, v2;
    fgetlines(file1, v1);
    fgetlines(file2, v2);
    return v1 == v2;
}