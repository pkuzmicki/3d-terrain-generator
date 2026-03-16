#include "core/app_panel.h"
#include "interface.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <string>

AppPanel* ap = &AppPanel::getInstance();
GeneratorSettings* settings;

void init_ui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    settings = ap->s_manager->current_scene->get_generator_settings();
}

void draw_ui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();

    ImGui::Begin("Edit Window");

    unsigned int min = 1, max_size = 2500, max_numocataves = 17, max_altitude = 500, max_render_distance = 100;
    // ImGui::SliderScalar("WIDTH", ImGuiDataType_U32, &settings->width, &min, &max_size);
    // ImGui::SliderScalar("LENGTH", ImGuiDataType_U32, &settings->length, &min, &max_size);
    //ImGui::SliderScalar("SIZE", ImGuiDataType_U32, &settings->size, &min, &max_size);
    ImGui::SliderScalar("NUMOCTAVES", ImGuiDataType_U32, &settings->numoctaves, &min, &max_numocataves);
    ImGui::SliderScalar("ALTITUDE", ImGuiDataType_U32, &settings->altitude, &min, &max_altitude);

    ImGui::SliderScalar("distance", ImGuiDataType_U32, &settings->render_distance, &min, &max_render_distance);

    

    if (ImGui::Button("reset")) {
        ap->s_manager->current_scene->meshes.clear();
        ap->s_manager->current_scene->get_generator()->active_chunks.clear();
    }

    std::string coords;
    ImGui::Text("Chunk: ");
    coords.append(std::to_string((int)ap->renderer->main_camera->chunk.x));
    coords.append(", ");
    coords.append(std::to_string((int)ap->renderer->main_camera->chunk.y));
    ImGui::Text(coords.c_str());

    coords.clear();

    ImGui::Text("Player Coords: ");
    coords.append(std::to_string(ap->renderer->main_camera->position.x));
    coords.append(", ");
    coords.append(std::to_string(ap->renderer->main_camera->position.y));
    coords.append(", ");
    coords.append(std::to_string(ap->renderer->main_camera->position.z));
    ImGui::Text(coords.c_str());

    ImGui::End();

    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(500, 500));
    ImGui::SetNextWindowSize(ImVec2(500, 500));
    ImGui::Begin("Map");

    //ImGui::Image()
    

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void free_ui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}