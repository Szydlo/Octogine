cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

project(Editor
        VERSION 0.1
        DESCRIPTION "Editor for Octogine"
)

add_executable(
        Editor

        main.cpp
        inspector.h
        baseUI.h
        viewport.h
        assetBrowser.h
        log.h
        hierarchy.h
)

target_link_libraries(Editor
        Octogine
        glm
)
target_include_directories(Editor
        PUBLIC
        "src"
        "lib/"
)