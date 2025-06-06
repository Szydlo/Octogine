#include "Texture2D.h"

#include <filesystem>
#include "spdlog/spdlog.h"
#include "../../vendor/stb/stb_image.h"
#include "../../vendor/stb/stb_image_write.h"

using Octo::Texture2D;

Texture2D::Texture2D(std::string path, const TextureColor color)
    : Texture(TextureType::Texture2D)
{
    bind();

    if (path.empty())
    {
        spdlog::error("[Texture] path is empty!");
        return;
    }

    if (!std::filesystem::exists(path))
    {
        spdlog::error("[Texture] path doesn't exist! {}", path);
    }

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data)
    {
        spdlog::error("[Texture] can't load texture {}", path);
        return;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    getFormatFromColor(color, channels);

    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    m_Width = width;
    m_Height = height;

    unbind();
}

Texture2D::Texture2D(const void* data, const glm::vec2 size, const TextureColor color)
     : Texture(TextureType::Texture2D)
{
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    getFormatFromColor(color);

	glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, size.x, size.y, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);

    //constexpr float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    unbind();

    m_Width = size.x;
    m_Height = size.y;
}

Texture2D::Texture2D(const uint8_t* imageData, size_t dataSize, const TextureColor color, bool flipVertically)
    : Texture(TextureType::Texture2D)
{
    bind();

    if (!imageData || dataSize == 0)
    {
        spdlog::error("[Texture] Invalid image data (nullptr or zero size)");
        return;
    }

    int width, height, channels;
    stbi_set_flip_vertically_on_load(flipVertically);

    unsigned char* data = stbi_load_from_memory(
        imageData, 
        static_cast<int>(dataSize), 
        &width, &height, &channels, 
        0 // Auto-detect channels
    );

    if (!data)
    {
        spdlog::error("[Texture] Failed to decode image from memory");
        return;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    getFormatFromColor(color, channels);

    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        m_InternalFormat, 
        width, 
        height, 
        0, 
        m_DataFormat, 
        GL_UNSIGNED_BYTE, 
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    m_Width = width;
    m_Height = height;

    unbind();
}