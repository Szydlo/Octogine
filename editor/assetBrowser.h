#pragma once

#include "../src/Octogine.h"
#include "fastgltf/core.hpp"

struct AssetBrowser
{
    static void draw()
    {
        ImGui::Begin("Asset browser");
        ImGui::End();
    }

    static inline auto rootPath = std::filesystem::path("C:/_projects/Octogine/assets/");
};
