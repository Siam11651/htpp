cmake_minimum_required(VERSION 3.5)
project(htpp VERSION 0.1.0 LANGUAGES C CXX)
add_library(htpp
    "src/client.cpp"
    "src/htpp.cpp"
    "src/request.cpp"
    "src/route.cpp")
target_include_directories(htpp PUBLIC "include")
set_property(TARGET htpp PROPERTY CXX_STANDARD 20)