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
    struct Asset : IAsset
    {
        Asset() : IAsset() {}

        std::shared_ptr<T> assetData;
        std::shared_ptr<T> getAsset() { return assetData; }
    };

    struct AssetTexture2D : Asset<Texture2D>
    {
        explicit AssetTexture2D(const std::string& path)
        {
            assetData = std::make_shared<Texture2D>(path);
        }
    };

    struct AssetShader : Asset<Shader>
    {
        AssetShader(const std::string& vs, const std::string& fs)
        {
            assetData = std::make_shared<Shader>(vs, fs);
        }
    };

    struct AssetModel :  Asset<Model>
    {
        explicit AssetModel(const std::string& path, const bool loadSkeleton = false)
        {
           assetData = std::make_shared<Model>(path, loadSkeleton);
        }
    };
};