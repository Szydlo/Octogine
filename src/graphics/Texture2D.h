#pragma once

#include "Texture.h"

#include <string>

namespace Octo
{
    class Texture2D : public Texture
    {
        public:
            explicit Texture2D(std::string path, TextureColor color = TextureColor::Color);
            Texture2D(const void* data, glm::vec2 size, TextureColor color = TextureColor::Color);
    };
};