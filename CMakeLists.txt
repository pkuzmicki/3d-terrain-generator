cmake_minimum_required(VERSION 3.23)

project(Gardens3D)

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(external/glfw)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(gardens ${SOURCES})

target_sources(gardens
    PRIVATE
        external/glad/src/glad.c
        external/imgui/imgui.cpp
        external/imgui/imgui_impl_opengl3.cpp
        external/imgui/imgui_impl_glfw.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui_demo.cpp
)

target_include_directories(gardens PRIVATE
    src    
    external
    external/glm
    external/glad
    external/imgui
)

# find_package(OpenGL REQUIRED)

# target_link_libraries(gardens PRIVATE glfw OpenGL::GL)


if (WIN32)
    target_link_libraries(gardens PRIVATE glfw opengl32)
elseif (UNIX)
    target_link_libraries(gardens PRIVATE glfw GL)
endif()