cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/spdlog)
add_subdirectory(lib/glm)
add_subdirectory(lib/sigslot)
add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)


project(Octogine 
    VERSION 0.1
    DESCRIPTION "Light-weight game engine"
)

file(GLOB SRC_OCTOGINE  
    "src/*.h"
    "src/*.cpp"
    "src/*/*.h"
    "src/*/*.cpp"
)

add_library(Octogine
    ${SRC_OCTOGINE}
)

set_target_properties(Octogine PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(Octogine PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries(Octogine
    spdlog
    glm
    sigslot
    glfw
    glad
)

add_subdirectory(example)