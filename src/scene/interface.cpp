#include "interface.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "core/app_panel.h"

#include <string>

void GUI::init_gui(GLFWwindow* window, AppPanel* ap) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    minimap = ap->minimap;
}

void GUI::draw_gui(AppPanel* ap) {
    GeneratorSettings* settings = ap->s_manager->current_scene->get_generator_settings();
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
    coords.append(std::to_string((int)ap->renderer->get_cam()->chunk.x));
    coords.append(", ");
    coords.append(std::to_string((int)ap->renderer->get_cam()->chunk.y));
    ImGui::Text(coords.c_str());

    coords.clear();

    ImGui::Text("Player Coords: ");
    coords.append(std::to_string(ap->renderer->get_cam()->position.x));
    coords.append(", ");
    coords.append(std::to_string(ap->renderer->get_cam()->position.y));
    coords.append(", ");
    coords.append(std::to_string(ap->renderer->get_cam()->position.z));
    ImGui::Text(coords.c_str());

    ImGui::End();

    //draw_screen(ap->renderer->screen_texture);
    draw_minimap(ap);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::draw_minimap(AppPanel* ap) {
    ImVec2 size(map_window_size, map_window_size);
    //ImGui::SetNextWindowSizeConstraints(size, size);
    //ImGui::SetNextWindowSize(size);
    ImGui::Begin("Map", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    

    ImVec2 pos = ImGui::GetCursorScreenPos();
    // auto i = minimap->get_minimap().find(std::make_pair(ap->renderer->get_cam()->chunk.x, ap->renderer->get_cam()->chunk.y));
    // if (i == minimap->get_minimap().end()) {
    //     std::cout<<"0";
    // } else {
    //     unsigned int txt = minimap->get_minimap().at(std::make_pair(ap->renderer->get_cam()->chunk.x, ap->renderer->get_cam()->chunk.y));
    //     ImVec2 map_pos(pos.x + pixels_per_chunk * ap->renderer->get_cam()->chunk.x, pos.y + pixels_per_chunk * ap->renderer->get_cam()->chunk.y);
    //     ImGui::GetWindowDrawList()->AddImage((ImTextureID)txt, map_pos, ImVec2(map_pos.x + pixels_per_chunk, map_pos.y + pixels_per_chunk), ImVec2(0, 1), ImVec2(1, 0));
    // }

    if (ImGui::IsWindowHovered()) {
        float scroll = ImGui::GetIO().MouseWheel;
        if (scroll != 0) {
            pixels_per_chunk += (scroll*2);
            if (pixels_per_chunk < 16) pixels_per_chunk = 16;
            if (pixels_per_chunk > 128) pixels_per_chunk = 128;
        }
    }

    ImGui::InvisibleButton("drag", size);
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
        ImVec2 delta = ImGui::GetIO().MouseDelta;
        offset.x += delta.x;
        offset.y += delta.y;
    }

    for (auto [coords, txt] : minimap->get_minimap()) {
        ImVec2 map_pos(pos.x + offset.x + pixels_per_chunk * coords.first, pos.y + offset.y + pixels_per_chunk * coords.second);
        //ImGui::GetWindowDrawList()->AddImage((ImTextureID)txt, map_pos, ImVec2(map_pos.x + pixels_per_chunk, map_pos.y + pixels_per_chunk), ImVec2(1, 1), ImVec2(0, 0));
        ImGui::GetWindowDrawList()->AddImageQuad(
            (ImTextureID)txt, 
            map_pos, 
            ImVec2(map_pos.x + pixels_per_chunk, map_pos.y),
            ImVec2(map_pos.x + pixels_per_chunk, map_pos.y + pixels_per_chunk),
            ImVec2(map_pos.x, map_pos.y + pixels_per_chunk),
            ImVec2(0, 0), ImVec2(0, 1), ImVec2(1, 1), ImVec2(1, 0)
        );
    }

    ImVec2(ap->renderer->get_cam()->position.x, ap->renderer->get_cam()->position.y);
    ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(pos.x + offset.x + ap->renderer->get_cam()->position.x, pos.y + offset.y + ap->renderer->get_cam()->position.z), pixels_per_chunk/8.0f, ImColor(255, 255, 255));

    ImGui::End();
}

void GUI::draw_screen(unsigned int texture_id) {
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(500, 500));
    ImGui::SetNextWindowSize(ImVec2(500, 500));
    ImGui::Begin("TEST");
    
    ImVec2 pos = ImGui::GetWindowPos();
    ImGui::GetWindowDrawList()->AddImage((ImTextureID)texture_id, pos, ImVec2(pos.x + 500, pos.y + 500), ImVec2(0, 0), ImVec2(1, 1));
    // ImGui::Image((void*)texture_id, ImVec2(500, 500));
    // ImGui::Image((ImTextureID)texture_id, ImVec2(499, 499));
    // ImGui::Image((void*)(intptr_t)texture_id, ImVec2(500, 500));

    ImGui::End();
}

void GUI::free_gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}