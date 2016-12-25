#ifndef SORT_FSORT_H
#define SORT_FSORT_H

#include <vector>

void fgetlines(const char *filename, std::vector<std::string> &dist);

void fputlines(const char *filename, const std::vector<std::string> &src);

void fsort(const char *input_file, const char *output_file);

bool fcmp(const char *file1, const char *file2);

#endif //SORT_FSORT_H
