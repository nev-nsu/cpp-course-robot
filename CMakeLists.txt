cmake_minimum_required(VERSION 3.6)
project(robot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lboost_program_options")

find_package(Boost 1.55 COMPONENTS log program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set(SOURCE_FILES main.cpp Surface.h Solver.h
        Plane/PlanarSurface.cpp Plane/PlanarSurface.h Plane/PlanarPoint.cpp Plane/PlanarPoint.h
        utility/boost_setup.cpp
        Language/LangSurface.cpp Language/LangSurface.h Language/LangPoint.cpp Language/LangPoint.h)
add_executable(robot ${SOURCE_FILES})

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

TARGET_LINK_LIBRARIES(robot ${Boost_LIBRARIES} pthread)

add_subdirectory(testing)