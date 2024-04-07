cmake_minimum_required(VERSION 3.5)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

add_subdirectory(lib/spdlog)
add_subdirectory(lib/glm)
add_subdirectory(lib/sigslot)
add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)
add_subdirectory(lib/assimp)

FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
        GIT_TAG "v5.0.0"
		SOURCE_SUBDIR "Build"
)
FetchContent_MakeAvailable(JoltPhysics)

set_target_properties(Jolt PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

project(Octogine 
    VERSION 0.4
    DESCRIPTION "Light-weight game engine"
)

file(GLOB SRC_OCTOGINE  
    "src/*.h"
    "src/*.cpp"
    "src/*/*.h"
    "src/*/*.cpp"

    "vendor/*/*.cpp"
    "vendor/*/*.h"

    "lib/imgui/*.cpp"
	"lib/imgui/*.h"
	"lib/imgui/backends/imgui_impl_glfw.h"
    "lib/imgui/backends/imgui_impl_glfw.cpp"
    "lib/imgui/backends/imgui_impl_opengl3.h"
    "lib/imgui/backends/imgui_impl_opengl3.cpp"
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
    assimp
    glad
    Jolt 
)

target_include_directories(Octogine
    PUBLIC
        "lib/imgui"
        ${JoltPhysics_SOURCE_DIR}/..
)

add_subdirectory(example)