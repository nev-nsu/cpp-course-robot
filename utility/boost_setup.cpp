//
// Created by nev on 12/24/16.
//

#include "boost_setup.h"

void log_init(std::string filename) {
    boost::log::add_common_attributes();
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    boost::log::add_file_log(filename,
                             boost::log::keywords::auto_flush = true,
                             boost::log::keywords::format = "%TimeStamp% (%LineID%) <%Severity%>: %Message%");
}

boost::program_options::variables_map po_init(int argc, const char *argv[]) {
    using namespace boost::program_options;
    options_description desc("General options");
    desc.add_options()
            // First parameter describes option name/short name
            // The second is parameter to option
            // The third is description
            ("help,h", "print usage message")
            ("in,i", value<std::string>()->default_value("input.txt"), "pathname for input")
            ("out,o", value<std::string>()->default_value("output.txt"), "pathname for output")
            ("type,w", value<std::string>(), "type of robot: planar/language")
            ("limit,l", value<int>()->default_value(1000), "limit for a way length");
    options_description planar_desc("Options for a planar robot");
    planar_desc.add_options()
            ("topology,t", value<std::string>()->default_value("planar"), "topology of planar surface");
    options_description lang_desc("Options for a language solver");
    lang_desc.add_options()
            ("dict,d", value<std::string>()->default_value("dict.txt"), "pathname for a russian dictionary");
    variables_map vm;
    parsed_options parsed = command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    store(parsed, vm);
    notify(vm);
    std::string task_type = "";
    if (vm.count("type") && !vm.count("help"))
        task_type = vm["type"].as<std::string>();
    if (task_type == "planar") {
        desc.add(planar_desc);
        store(parse_command_line(argc, argv, desc), vm);
        return vm;
    } else if (task_type == "language") {
        desc.add(lang_desc);
        store(parse_command_line(argc, argv, desc), vm);
        return vm;
    } else {
        desc.add(planar_desc).add(lang_desc);
        std::cout << desc << "\nLog is in the file 'robot.log'\n\n"
                  << "Input files format: \nFor a planar robot: map in the input file\n"
                  << "\t# - barrier\n\t. - road\n\tS - start point\n\tF - finish point\n"
                  << "For a language solver: in the input file are three words:\n"
                  << "\t1. Start word\n\t2. Finish word\n\t3. Alphabet (=russian alpahbet)" << std::endl;
        exit(0);
    }
}
