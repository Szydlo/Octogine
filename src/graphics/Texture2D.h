#pragma once

#include "Texture.h"

#include <string>
#include <filesystem>

#include "spdlog/spdlog.h"
#include "../../vendor/stb/stb_image.h"
#include "../../vendor/stb/stb_image_write.h"

namespace Octo
{
    class Texture2D : public Texture
    {
        public:
            Texture2D(std::string path, TextureColor color = TextureColor::Color);
            Texture2D(void* data, glm::vec2 size, TextureColor color = TextureColor::Color);
    };
};