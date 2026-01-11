#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/app_panel.h"
#include "renderer.h"
#include "render/mesh.h"
#include "render/camera.h"

Renderer::Renderer() {
    main_camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
}

void Renderer::calc_matrix() {
    projection = glm::perspective(glm::radians(80.0f), static_cast<float>(AppPanel::getInstance().SCREEN_WIDTH) / static_cast<float>(AppPanel::getInstance().SCREEN_HEIGHT), 0.1f, 500.0f);

    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);

    unsigned int view_loc = glGetUniformLocation(shader.ID, "view");
    unsigned int proj_loc = glGetUniformLocation(shader.ID, "projection");

    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(main_camera->get_view_matrix()));
}

void Renderer::render_scene(Scene &scene) {
    for (Mesh m : scene.meshes) {;
        m.draw(shader);
    }
}

void Renderer::frame_begin() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    calc_matrix();
}

