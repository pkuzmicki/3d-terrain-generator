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
    GeneratorSettings* s = ap->s_manager->current_scene->get_generator_settings();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow();

    ImGui::Begin("Edit Window");

    unsigned int min = 1, max_size = 2500, max_numocataves = 17, max_altitude = 100, max_render_distance = 100;
    // ImGui::SliderScalar("NUMOCTAVES", ImGuiDataType_U32, &settings->numoctaves, &min, &max_numocataves);
    // ImGui::SliderScalar("ALTITUDE", ImGuiDataType_U32, &settings->altitude, &min, &max_altitude);

    ImGui::SliderScalar("distance", ImGuiDataType_U32, &s->render_distance, &min, &max_render_distance);
    
    if (ImGui::Button("temperature")) s->mode = MAP_MODE::TEMPERATURE;
    ImGui::SameLine();
    if (ImGui::Button("height")) s->mode = MAP_MODE::HEIGHT;
    ImGui::SameLine();
    if (ImGui::Button("biomes")) s->mode = MAP_MODE::BIOME;

    std::string coords;
    ImGui::Text("Chunk: ");
    coords.append(std::to_string((int)ap->renderer->get_cam()->chunk.x));
    coords.append(", ");
    coords.append(std::to_string((int)ap->renderer->get_cam()->chunk.y));
    ImGui::SameLine();
    ImGui::Text(coords.c_str());

    coords.clear();

    ImGui::Text("Player Coords: ");
    coords.append(std::to_string(ap->renderer->get_cam()->position.x));
    coords.append(", ");
    coords.append(std::to_string(ap->renderer->get_cam()->position.y));
    coords.append(", ");
    coords.append(std::to_string(ap->renderer->get_cam()->position.z));
    ImGui::Text(coords.c_str());

    ImGui::Text("Biome: ");
    ImGui::SameLine();
    ImGui::Text(biome_names[ap->s_manager->current_scene->get_generator()->get_current_biome(ap->renderer->get_cam()->position.x, ap->renderer->get_cam()->position.z)]);

    ImGui::SliderScalar("t alt", ImGuiDataType_U32, &s->temp_alt, &min, &max_altitude);
    ImGui::SliderScalar("h alt", ImGuiDataType_U32, &s->height_alt, &min, &max_altitude);


    if (ImGui::Button("reset")) {
        ap->s_manager->current_scene->meshes.clear();
        ap->s_manager->current_scene->get_generator()->active_chunks.clear();\
        minimap->get_minimap().clear();
    }


    ImGui::End();

    //draw_screen(ap->renderer->screen_texture);
    draw_minimap(ap);
    draw_diagram(ap);




    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::draw_minimap(AppPanel* ap) {
    ImVec2 size(map_window_size, map_window_size);
    ImGui::Begin("Map", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    ImVec2 pos = ImGui::GetCursorScreenPos();

    if (ImGui::IsWindowHovered()) {
        float scroll = ImGui::GetIO().MouseWheel;
        if (scroll != 0) {
            pixels_per_chunk += (scroll*2);
            if (pixels_per_chunk < 8) pixels_per_chunk = 8;
            if (pixels_per_chunk > 256) pixels_per_chunk = 256;
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

    float scale = (float)pixels_per_chunk / 32;
    ImGui::GetWindowDrawList()->AddCircleFilled(
        ImVec2(
            pos.x + offset.x + ap->renderer->get_cam()->position.x * scale, 
            pos.y + offset.y + ap->renderer->get_cam()->position.z * scale), 
        pixels_per_chunk/8.0f, 
        ImColor(255, 255, 255));

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

void GUI::draw_diagram(AppPanel* ap) {

    // std::vector<float> points;
    // int CELLSIZE = 500;
    // const int NUMOCTAVES = 12;
    // const int ALTITUDE = 8000;
    // float x = (float)rand();
    // float z = (float)rand();

    // ImGui::Begin("Temperature");
    // ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    // ImVec2 canvas_size = ImGui::GetContentRegionAvail();


    // int minVal = *std::min_element(points.begin(), points.end());
    // int maxVal = *std::max_element(points.begin(), points.end());
    // int d = maxVal - minVal;


    // //CELLSIZE = 512;
    // for (int i = 0; i < CELLSIZE; i++) {
    //     for (int j = 0; j < CELLSIZE; j++) {
    //         ImU32 col;
    //         // if (points[i * CELLSIZE + j] < minVal + d/5) {
    //         //     col = IM_COL32(51, 51, 51, 255);
    //         // } else if (points[i * CELLSIZE + j] > minVal + d/5 && points[i * CELLSIZE + j] < minVal + (2*(d/5))) {
    //         //     col = IM_COL32(102, 102, 102, 255);
    //         // } else if (points[i * CELLSIZE + j] > minVal + (2*(d/5)) && points[i * CELLSIZE + j] < minVal +  (3*(d/5))) {
    //         //     col = IM_COL32(153, 153, 153, 255);
    //         // } else if (points[i * CELLSIZE + j] > minVal + (3*(d/5)) && points[i * CELLSIZE + j] < minVal +  (4*(d/5))) {
    //         //     col = IM_COL32(204, 204, 204, 255);
    //         // } else {
    //         //     col = IM_COL32(255, 255, 255, 255);
    //         // }

    //         if (points[i * CELLSIZE + j] < minVal + d/3) {
    //             col = IM_COL32(255, 0, 0, 255);
    //         } else if (points[i * CELLSIZE + j] > minVal + d/3 && points[i * CELLSIZE + j] < minVal + 0.6 * d) {
    //             col = IM_COL32(0, 255, 0, 255);
    //         } else {
    //             col = IM_COL32(0, 0, 255, 255);
    //         }
    //         ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(canvas_pos.x + j, canvas_pos.y + i), ImVec2(canvas_pos.x + j + 1, canvas_pos.y + i + 1), col);
    //     }
    // }
    
    //ImGui::End();
}

void GUI::free_gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}