cmake_minimum_required(VERSION 3.5.0)
project(raspberry_github VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(git miniGit/main_1.cpp miniGit/miniGit.cpp)
add_executable(client client/main_client.cpp)
add_executable(server server/main_server.cpp)

include(CTest)
enable_testing()