//
// Created by nev on 12/24/16.
//

#ifndef ROBOT_LOGGING_H
#define ROBOT_LOGGING_H

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/program_options.hpp>


void log_init(std::string filename);

boost::program_options::variables_map po_init(int argc, const char *argv[]);


#endif //ROBOT_LOGGING_H
