cmake_minimum_required(VERSION 3.23)

project(Gardens3D)

# set(GLFW_BUILD_EXAMPLES OFF)
# set(GLFW_BUILD_TESTS OFF)
# set(GLFW_BUILD_DOCS OFF)
# set(GLFW_INSTALL OFF)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(external/glfw)


add_executable(gardens)

add_definitions(-DGLFW_INCLUDE_NONE)

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
        src/main.cpp
        src/core/app_panel.cpp
        src/core/input.cpp
        src/render/renderer.cpp
        src/render/shader.cpp
        src/render/camera.cpp 
        src/render/mesh.cpp
        src/scene/scene.cpp
        src/scene/terrain.cpp
        src/scene/biome_manager.cpp
        src/scene/interface.cpp
        #src/*.cpp
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