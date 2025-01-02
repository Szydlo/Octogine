#pragma once

#include "Object3D.h"

#include "../../models/Model.h"

namespace Octo
{
    class MeshObject : public Object3D
    {
        public:
            MeshObject(std::string name, std::string meshPath);

            void update(double deltaTime) override;
        private:
            Model* m_CurrentModel = nullptr;
    };
};