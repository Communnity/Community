cmake_minimum_required(VERSION 3.10)
project(Community)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2  ")
#include_directories(.)

add_executable(Community
    clique.cpp
    clique.h
    condition.cpp
    condition.h
    core.cpp
    core.h
    ego_community.cpp
    ego_community.h
    file_manage.cpp
    file_manage.h
    graph.cpp
    graph.h
    include.cpp
    include.h
    ks_core.cpp
    ks_core.h
    main.cpp
    README.md
        truss.cpp
    truss.h tree.cpp tree.h graph_algorithm.cpp graph_algorithm.h)
