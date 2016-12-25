#include <iostream>
#include <fstream>

#include "utility/boost_setup.h"
#include "Plane/PlanarSurface.h"
#include "Language/LangSurface.h"
#include "Solver.h"

int main(int argc, const char *argv[]) {
    try {
        log_init("logs/robot.log");
    }
    catch (...) {
        std::cerr << "Log init failed. Aborted()" << std::endl;
        exit(EXIT_FAILURE);
    }
    BOOST_LOG_TRIVIAL(info) << "Program started.";

    try {
        boost::program_options::variables_map vm = po_init(argc, argv);

        std::string input_file = vm["in"].as<std::string>();
        std::string output_file = vm["out"].as<std::string>();
        int limit = vm["limit"].as<int>();

        if (vm["type"].as<std::string>() == "planar") {
            Topology top = Topology::planar;
            auto str = vm["topology"].as<std::string>();
            if (str == "cylinder")
                top = Topology::cylinder;
            else if (str == "tor")
                top = Topology::tor;
            PlanarSurface surface(input_file, top);
            Solver<PlanarPoint> solver(&surface);
            solver.solve(limit);
            std::ofstream out(output_file);
            if (!out)
                BOOST_LOG_TRIVIAL(error) << "Output file didn't open.";
            out << surface;
        } else if (vm["type"].as<std::string>() == "language") {
            std::string dict = vm["dict"].as<std::string>();

            std::ifstream input(input_file);
            if (!input)
                BOOST_LOG_TRIVIAL(error) << "Reading error: file doesn't exist";
            std::vector<std::string> in;
            std::string buf;
            std::getline(input, buf);
            int j = 3;
            while (!input.eof() && input && j--) {
                in.push_back(buf);
                std::getline(input, buf);
            }
            if (j--)
                in.push_back(russian_alphabet);
            if (j) {
                BOOST_LOG_TRIVIAL(error) << "Parsing error : wrong count of attributes in " << input_file;
                throw BadMap();
            }
            LangSurface surface(dict, in[2], in[0], in[1]);
            Solver<LangPoint> solver(&surface);
            solver.solve(limit);
            std::ofstream out(output_file);
            auto path = solver.get_path();
            for (auto i : path)
                out << std::string(i) << std::endl;
        }
        return 0;
    }
    catch (...) {
        BOOST_LOG_TRIVIAL(error) << "Critical error, abort.";
        std::cout << "Sorry, there is some critical error. Program finished." << std::endl;
        exit(EXIT_FAILURE);
    }
}