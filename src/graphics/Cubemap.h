#pragma once

#include "Texture.h"

#include "../../vendor/stb/stb_image.h"

#include <array>

namespace Octo
{
    class Cubemap : public Texture
    {
        public:
            Cubemap(std::array<std::string, 6> paths, TextureColor color = TextureColor::Color);
            Cubemap(glm::vec2 size, TextureColor color = TextureColor::Color);
    };
};
