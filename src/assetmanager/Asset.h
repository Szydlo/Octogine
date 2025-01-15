#pragma once

#include <filesystem>
#include <string>

#include "../graphics/Shader.h"
#include "../graphics/Texture2D.h"
#include "../models/Model.h"

#include <any>

namespace Octo
{
    enum class AssetType : unsigned int
    {
        None = 0,
        Texture2D, Shader, Model
    };

    struct IAsset
    {
        IAsset() = default;
        AssetType type;
    };

    template<typename T>
    struct Asset : public IAsset
    {
        Asset() {}

        std::shared_ptr<T> assetData;
        std::shared_ptr<T> getAsset() { return assetData; }
    };

    struct AssetTexture2D : public Asset<Texture2D> 
    {
        AssetTexture2D(std::string path)
        {
            assetData = std::make_shared<Octo::Texture2D>(path);
        }
    };

    struct AssetShader : public Asset<Shader>
    {
        AssetShader(std::string vs, std::string fs)
        {
            assetData = std::make_shared<Octo::Shader>(vs, fs);
        }
    };

    struct AssetModel : public Asset<Model>
    {
        AssetModel(std::string path, bool loadSkeleton = false)
        {
           assetData = std::make_shared<Model>(path, loadSkeleton);
        }
    };
};