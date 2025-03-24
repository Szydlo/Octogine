#pragma once

#include "glm/glm.hpp"
#include "glad/glad.h"

namespace Octo
{
    enum class TextureType : int
    {
        Texture2D = GL_TEXTURE_2D,
        Cubemap = GL_TEXTURE_CUBE_MAP,
    };

    enum class TextureAttachment : int
    {
        Color = GL_COLOR_ATTACHMENT0,
        Depth = GL_DEPTH_ATTACHMENT,
        Stencil = GL_DEPTH_STENCIL_ATTACHMENT,
    };

    enum class TextureColor : char
    {
        Color, Normal, Displacement, Depth
    };


    class Texture
    {
        public:
            explicit Texture(TextureType type);
            ~Texture();

            void bind(unsigned int slot = 0);
            void unbind();

            [[nodiscard]] TextureType getType() const { return m_Type; }
            [[nodiscard]] TextureAttachment getAttachment() const { return m_Attachment; }

            unsigned int getIdentity() { return m_Identity; }

            [[nodiscard]] int getWidth() const { return m_Width; }
            [[nodiscard]] int getHeight() const { return m_Height; }
        protected:
            void getFormat(unsigned int channels);
            void getFormatFromColor(TextureColor color, unsigned int channels = 4);

            unsigned int m_Identity{};
            int m_Width{}, m_Height{};

            TextureType m_Type;
            TextureAttachment m_Attachment;
            GLenum m_InternalFormat{}, m_DataFormat{};
    };
};