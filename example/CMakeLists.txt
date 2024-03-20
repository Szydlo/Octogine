cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

project(ExampleProject 
    VERSION 0.1
    DESCRIPTION "Example project for Octogine"
)

add_executable(
    ExampleProject

    main.cpp
)

target_link_libraries(ExampleProject Octogine glm)
target_include_directories(ExampleProject
    PUBLIC
        "src"
)