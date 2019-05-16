cmake_minimum_required(VERSION 3.13)
project(Communtity)

set(CMAKE_CXX_STANDARD 11)


find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
add_executable(MyTests test.cpp test.h)
target_link_libraries(MyTests ${GTEST_BOTH_LIBRARIES})
target_link_libraries(MyTests ${CMAKE_THREAD_LIBS_INIT})
add_test(Test MyTests)
enable_testing()

add_executable(Communtity main.cpp cores.cpp cores.h include.cpp include.h graph.cpp graph.h condition.cpp condition.h file_manage.cpp file_manage.h truss.cpp truss.h ks_core.cpp ks_core.h)