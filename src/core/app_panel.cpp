#include "app_panel.h"

#include <glfw/glfw3.h>
#include <glad/glad.h>
#include <iostream>

#include "render/renderer.h"
#include "scene/scene_manager.h"
#include "input.h"
#include "scene/interface.h"

AppPanel& AppPanel::getInstance() {
    static AppPanel ap_;
    return ap_;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
} 

void AppPanel::init() {
    std::cout<<"APP_INIT\n";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TerrainGenerator3D", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }   

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    renderer = new Renderer();
    s_manager = new SceneManager();
    //to do oblsugi na przyciski
    s_manager->current_scene = new TerrainScene();

    init_input(window);
    init_ui(window);

    std::cout<<"Initizalization Succesfull\n";
}

void AppPanel::run() {
    while (!glfwWindowShouldClose(window)) {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        key_callback(window, delta_time);

        update_mode();
        renderer->frame_begin();
        renderer->render_scene(*s_manager->current_scene);
        draw_ui();


        glfwPollEvents();    
        glfwSwapBuffers(window);
    }

    free_ui();
    glfwTerminate(); 
}

void AppPanel::update_mode() {
    if (is_mouse_hidden)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else 
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);    
}