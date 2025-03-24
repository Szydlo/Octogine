#pragma once

#include "Asset.h"

#include <map>
#include <string>

namespace Octo
{
    class AssetManager
    {
        public:
            template<class ...Args>
            static void loadAsset(const std::string& id, const AssetType type, Args && ...args)
            {
                // @ TODO REMOVE LATER THIS FACTORY, CAN BE AUTOMATED
                switch (type)
                {
                    case AssetType::Texture2D:
                        m_LoadedAssets[id] = std::make_shared<AssetTexture2D>(args...);
                        break;
                    case AssetType::Shader:
                        m_LoadedAssets[id] = std::make_shared<AssetTexture2D>(args...);
                        break;
                    case AssetType::Model:
                        m_LoadedAssets[id] = std::make_shared<AssetModel>(args...);
                        break;
                    default: ;
                }
            }


            // NOT REALLY PROUDEST OF THIS CODE, IDK FOR NOW HOW TO WRITE IT BETTER, SO I LEAVE IT LIKE THIS FOR NOW
            // MODERN CPP xd
            template<typename T>
            static std::shared_ptr<T> getAsset(const std::string& id)
            {
                std::shared_ptr<IAsset> iasset = m_LoadedAssets.at(id);
                std::shared_ptr<Asset<T>> asset = std::static_pointer_cast<Asset<T>>(iasset);
                return asset.get()->getAsset();
            }
        private:
            static inline std::map<std::string, std::shared_ptr<IAsset>> m_LoadedAssets;
    };
};