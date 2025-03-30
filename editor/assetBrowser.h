#pragma once

#include "imgui_internal.h"
#include "../src/Octogine.h"
#include "fastgltf/core.hpp"

struct AssetBrowser
{
    static void init()
    {
        loadDirectories();
    }

    static void draw()
    {
        ImGui::Begin("Asset browser");

        if (ImGui::TreeNodeEx("root"))
        {

            ImGui::TreePop();
        }

        /*ImGui::SameLine();

        if (ImGui::TreeNodeEx("files"))
        {
            ImGui::TreePop();
        }*/

        ImGui::End();
    }

    static void loadDirectories()
    {

    }

    static inline auto rootPath = std::filesystem::path("C:/_projects/Octogine/assets/");
};
