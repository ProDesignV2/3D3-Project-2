cmake_minimum_required(VERSION 3.13)
project(3D3Proj2master)

set(CMAKE_CXX_STANDARD 11)

add_executable(3D3Proj2master helper.cpp helper.h router.cpp FileTopology message.cpp message.hpp bellman.hpp bellman.cpp)
