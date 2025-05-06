cmake_minimum_required(VERSION 3.28)

project(spg)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES
    main.cpp
)

add_subdirectory(fltk-1.4.1)

include_directories(
    /usr/include
    /usr/include/GL
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    GL      # OpenGL
    GLEW    # GLEW
    glut    # GLUT or freeglut
    fltk
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
