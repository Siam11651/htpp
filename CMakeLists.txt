cmake_minimum_required(VERSION 3.5)
project(htpp VERSION 0.1.0 LANGUAGES C CXX)
include(FetchContent)
FetchContent_Declare(llhttp
  URL "https://github.com/nodejs/llhttp/archive/refs/tags/release/v9.2.1.tar.gz")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS ON CACHE INTERNAL "")
FetchContent_MakeAvailable(llhttp)
add_library(htpp
    "src/htpp.cpp"
    "src/client.cpp")
target_include_directories(htpp PUBLIC "include")
target_link_libraries(htpp llhttp_static)
set_property(TARGET htpp PROPERTY CXX_STANDARD 20)