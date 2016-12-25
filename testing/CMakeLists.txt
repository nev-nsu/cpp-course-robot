cmake_minimum_required(VERSION 3.6)
project(sort_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost 1.55 COMPONENTS log REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set(SOURCE_FILES test.cpp
        ../Plane/PlanarPoint.cpp ../Plane/PlanarSurface.cpp
        ../Language/LangPoint.cpp ../Language/LangSurface.cpp
        ../utility/boost_setup.cpp ../utility/fsort.cpp)
add_executable(robot_test ${SOURCE_FILES})

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

TARGET_LINK_LIBRARIES(robot_test ${Boost_LIBRARIES} pthread)

add_subdirectory(googletest)

target_link_libraries(robot_test gtest gtest_main)