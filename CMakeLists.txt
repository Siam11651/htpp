cmake_minimum_required(VERSION 3.5)
project(htpp VERSION 0.1.0 LANGUAGES C CXX)

add_library(htpp
    "src/htpp.cpp"
    "src/client.cpp")
target_include_directories(htpp PUBLIC "include")
