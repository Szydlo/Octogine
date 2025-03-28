#pragma once

#include "Texture.h"

#include <array>
#include <string>

namespace Octo
{
    class Cubemap : public Texture
    {
        public:
            explicit Cubemap(std::array<std::string, 6> paths, TextureColor color = TextureColor::Color);
            explicit Cubemap(glm::vec2 size, TextureColor color = TextureColor::Color);

            void createMipmap();
    };
};
