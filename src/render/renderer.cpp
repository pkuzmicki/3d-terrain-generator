#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/app_panel.h"
#include "renderer.h"
#include "render/camera.h"

Renderer::Renderer() {
    main_camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
}

Camera* Renderer::get_cam() {
    return main_camera;
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
    // for (Mesh m : scene.meshes) {;
    //     m.draw(shader);
    // }

    for (auto& m : scene.get_generator()->active_chunks) {
        m.second.draw(shader);
    }
}

void Renderer::frame_begin() {
    glClearColor(0.2f, 0.80f, 0.80f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    calc_matrix();
}

void Renderer::create_framebuffer() {
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &screen_texture);
    glBindTexture(GL_TEXTURE_2D, screen_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texture, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
    else
        std::cout <<"FRAMEBUFFER::CREATION::SUCCESS\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

}

void Renderer::bind_framebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Renderer::unbind_framebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}