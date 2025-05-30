#include "Texture.h"

#include "spdlog/spdlog.h"

using Octo::Texture;

Texture::Texture(const TextureType type)
    : m_Type(type)
{
    glGenTextures(1, &m_Identity);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_Identity);
    spdlog::info("TEXTURE DECONSTRUCTOR CALLED");
}

void Texture::bind(const unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(static_cast<int>(m_Type), m_Identity);
}

void Texture::unbind()
{
    glBindTexture(static_cast<int>(m_Type), 0);
}

void Texture::getFormat(const unsigned int channels)
{
    switch(channels)
    {
    case 4:
    {
        m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

        break;
    }
    case 3:
    {
		m_InternalFormat = GL_RGB8;
		m_DataFormat = GL_RGB;

        break;
    }
        case 2:
    {
        m_InternalFormat = GL_RGBA;
        m_DataFormat = GL_RG;

        break;
    }
    case 1:
    {
        m_InternalFormat = GL_SRGB;
        m_DataFormat = GL_RED;

        break;
    }
    default:
    {
        spdlog::error("[TEXTURE] texture channels not supported: {}", channels);
        break;
    }
    }
}

void Texture::getFormatFromColor(const TextureColor color, const unsigned int channels)
{
    if (color == TextureColor::Color)
    {
        getFormat(channels);
        m_Attachment = TextureAttachment::Color;
    }
    else if (color == TextureColor::Normal)
    {
        getFormat(channels);
        m_Attachment = TextureAttachment::Color;
    }
    else if (color == TextureColor::Displacement)
    {
        m_InternalFormat = GL_RED;
        m_DataFormat = GL_RED;
        m_Attachment = TextureAttachment::Color;
    }
    else if (color == TextureColor::Depth)
    {
        m_InternalFormat = GL_DEPTH_COMPONENT;
        m_DataFormat = GL_DEPTH_COMPONENT;
        m_Attachment = TextureAttachment::Depth;
    }
}