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
    calc_map(ap);
}

void GUI::calc_map(AppPanel* ap) {
        
    float x = (float)rand();
    float z = (float)rand();

    ALTITUDE = 20;
    //NUMOCTAVES = 8;

    CELLSIZE = 512;
    for(int i=0; i<CELLSIZE; i++) {
        for(int j=0; j<CELLSIZE; j++) {
            float c =  ALTITUDE * ap->s_manager->current_scene->get_generator()->v.GetHeight(x + i/256.0f, z + j/256.0f, 0.5f, 2.0f, NUMOCTAVES);
            points.push_back(c);
        }
    }


    // CELLSIZE = 32;
    // for(int i=0; i<CELLSIZE; i++) {
    //     for(int j=0; j<CELLSIZE; j++) {
    //         float c =  ALTITUDE * ap->s_manager->current_scene->get_generator()->v.GetHeight(x + i/256.0f, z + j/256.0f, 0.5f, 2.0f, NUMOCTAVES);
    //         points1.push_back(c);
    //     }
    // }
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

    ImGui::Text("Biome: ");
    ImGui::Text(biome_names[ap->s_manager->current_scene->get_generator()->get_current_biome(ap->renderer->get_cam()->position.x, ap->renderer->get_cam()->position.z)]);

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

#define JC_VORONOI_IMPLEMENTATION
#include "noise/jc_voronoi.h"
#include "noise/PoissonGenerator.h"
#include "noise/valuenoise.h"

#include <stdlib.h>
#include <algorithm>

void GUI::draw_diagram(AppPanel* ap) {

    // std::vector<float> points;
    // int CELLSIZE = 500;
    // const int NUMOCTAVES = 12;
    // const int ALTITUDE = 8000;
    // float x = (float)rand();
    // float z = (float)rand();

    ImGui::Begin("Temperature");
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();


    int minVal = *std::min_element(points.begin(), points.end());
    int maxVal = *std::max_element(points.begin(), points.end());
    int d = maxVal - minVal;


    //CELLSIZE = 512;
    for (int i = 0; i < CELLSIZE; i++) {
        for (int j = 0; j < CELLSIZE; j++) {
            ImU32 col;
            // if (points[i * CELLSIZE + j] < minVal + d/5) {
            //     col = IM_COL32(51, 51, 51, 255);
            // } else if (points[i * CELLSIZE + j] > minVal + d/5 && points[i * CELLSIZE + j] < minVal + (2*(d/5))) {
            //     col = IM_COL32(102, 102, 102, 255);
            // } else if (points[i * CELLSIZE + j] > minVal + (2*(d/5)) && points[i * CELLSIZE + j] < minVal +  (3*(d/5))) {
            //     col = IM_COL32(153, 153, 153, 255);
            // } else if (points[i * CELLSIZE + j] > minVal + (3*(d/5)) && points[i * CELLSIZE + j] < minVal +  (4*(d/5))) {
            //     col = IM_COL32(204, 204, 204, 255);
            // } else {
            //     col = IM_COL32(255, 255, 255, 255);
            // }

            if (points[i * CELLSIZE + j] < minVal + d/3) {
                col = IM_COL32(255, 0, 0, 255);
            } else if (points[i * CELLSIZE + j] > minVal + d/3 && points[i * CELLSIZE + j] < minVal + 0.6 * d) {
                col = IM_COL32(0, 255, 0, 255);
            } else {
                col = IM_COL32(0, 0, 255, 255);
            }
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(canvas_pos.x + j, canvas_pos.y + i), ImVec2(canvas_pos.x + j + 1, canvas_pos.y + i + 1), col);
        }
    }
    
    ImGui::End();




    // ImGui::Begin("Biomy");
    // ImVec2 canvas_pos1 = ImGui::GetCursorScreenPos();
    // ImVec2 canvas_size1 = ImGui::GetContentRegionAvail();

    
    // int minVal1 = *std::min_element(points1.begin(), points1.end());
    // int maxVal1 = *std::max_element(points1.begin(), points1.end());
    // int d1 = maxVal1 - minVal1;

    // int min = 0, max = 40;

    // CELLSIZE = 32;
    // for (int i = 0; i < CELLSIZE; i++) {
    //     for (int j = 0; j < CELLSIZE; j++) {
    //         ImU32 col;
    //         if (points1[i * CELLSIZE + j] < minVal1 + d1/3) {
    //             col = IM_COL32(50, 50, 50, 255);
    //         } else if (points1[i * CELLSIZE + j] > minVal1 + d1/3 && points1[i * CELLSIZE + j] < minVal1 + 0.6 * d1) {
    //             col = IM_COL32(150, 150, 150, 255);
    //         } else {
    //             col = IM_COL32(255, 255, 255, 255);
    //         }
    //         ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(canvas_pos1.x + j, canvas_pos1.y + i), ImVec2(canvas_pos1.x + j + 1, canvas_pos1.y + i + 1), col);
    //     }
    // }

    // ImGui::SliderScalar("NUMOCTAVES", ImGuiDataType_U32, &NUMOCTAVES, &min, &max);

    // if (ImGui::Button("reset")) {
    //     calc_map(ap);
    // }

    // ImGui::End();

    // int seed = ap->s_manager->current_scene->get_generator()->seed;

    // PoissonGenerator::DefaultPRNG prng(seed);
    
    // std::vector<jcv_point> jvc_points;
    // for (int i = 1; i < 6; ++i) {
    //     for (int j = 1; j < 6; ++j) {
    //         std::vector<PoissonGenerator::Point> chunk = PoissonGenerator::generatePoissonPoints(2, prng, false);
    //         for (auto p: chunk) {
    //             p.x += j;
    //             p.y += i;
    //             p.x *= 100;
    //             p.y *= 100;
    //             //std::cout<<p.x<<" "<<p.y<<"\n";
    //             jvc_points.push_back({p.x, p.y});
    //         }
    //     }
    // }

    // for (auto p: jvc_points) {
    //     ImGui::GetWindowDrawList()->AddCircle(ImVec2(p.x + canvas_pos.x, p.y + canvas_pos.y), 5.0f, IM_COL32(255, 0, 0, 255));
    // }

    // jcv_diagram diagram;
    // memset(&diagram, 0, sizeof(jcv_diagram));
    // jcv_diagram_generate(jvc_points.size(), jvc_points.data(), nullptr, nullptr, &diagram);

    // std::vector<std::vector<jcv_point>> voronoi_cells;
    // const jcv_site* sites = jcv_diagram_get_sites(&diagram);

    // for (int i = 0; i < diagram.numsites; ++i) {
    //     const jcv_site* site = &sites[i];
    //     const jcv_graphedge* edge = site->edges;
    //     std::vector<jcv_point> cell_vertices;
        
    //     while (edge) {
    //         cell_vertices.push_back(edge->pos[0]);
    //         edge = edge->next;
    //     }
    //     voronoi_cells.push_back(cell_vertices);
    // }


    // //ImGui::GetWindowDrawList()->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(30, 30, 30, 255));

    // for (const auto& cell : voronoi_cells) {
    //     if (cell.empty()) continue;

    //     std::vector<ImVec2> points;
    //     for (const auto& p : cell) {
    //         points.push_back(ImVec2(canvas_pos.x + p.x, canvas_pos.y + p.y));
    //     }

    //     ImGui::GetWindowDrawList()->AddPolyline(points.data(), (int)points.size(), IM_COL32(0, 255, 0, 255), ImDrawFlags_Closed, 2.0f);
        
    //     //ImGui::GetWindowDrawList()->AddConvexPolyFilled(points.data(), (int)points.size(), IM_COL32(0, 255, 0, 40));

    // }
    //     jcv_diagram_free(&diagram);


}

void GUI::free_gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}