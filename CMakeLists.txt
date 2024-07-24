cmake_minimum_required(VERSION 3.5)
project(htpp VERSION 0.1.0 LANGUAGES C CXX)
add_library(htpp
    "src/client.cpp"
    "src/cookies.cpp"
    "src/handler.cpp"
    "src/htpp.cpp"
    "src/request.cpp"
    "src/response.cpp"
    "src/route.cpp"
    "src/route-segment-tree.cpp"
    "src/string-view.cpp")
target_include_directories(htpp PUBLIC "include")
set_property(TARGET htpp PROPERTY CXX_STANDARD 20)

if(BUILD_TESTS)
    add_executable(htpp-test-0
        "tests/test-0.cpp")
    target_include_directories(htpp-test-0 PUBLIC "include")
    target_link_libraries(htpp-test-0 htpp)
    set_property(TARGET htpp-test-0 PROPERTY CXX_STANDARD 20)
endif()