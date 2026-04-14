#include "app_panel.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "render/renderer.h"
#include "scene/scene_manager.h"
#include "input.h"

AppPanel& AppPanel::getInstance() {
    static AppPanel ap_;
    return ap_;
}

AppPanel::~AppPanel() {
    delete renderer, s_manager, s_manager->current_scene, gui;
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
    minimap = new MiniMap();
    s_manager->current_scene = new TerrainScene(this);
    gui = new GUI();

    init_input(window);
    gui->init_gui(window, this);

    //renderer->create_framebuffer();

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
        s_manager->current_scene->update_scene();
        renderer->render_scene(*s_manager->current_scene);

        // renderer->bind_framebuffer();
        // renderer->frame_begin();
        // s_manager->current_scene->update_scene();
        // renderer->render_scene(*s_manager->current_scene);
        // renderer->unbind_framebuffer();
        
        gui->draw_gui(&getInstance());

        glfwPollEvents();    
        glfwSwapBuffers(window);
    }

    gui->free_gui();
    glfwTerminate(); 
}

void AppPanel::update_mode() {
    if (is_mouse_hidden)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else 
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);    
}