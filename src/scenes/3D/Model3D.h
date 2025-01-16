#pragma once

#include "Node3D.h"
#include "../models/Model.h"

namespace Octo 
{
    class Model3D : public Node3D
    {
        public:
            Model3D(std::string name, std::string path)
            : Node3D(name), model(path) {}

            void update(double deltaTime) override
            {
                Node3D::update(deltaTime);
                model.setTransform(m_Transform.getMat4());
                model.draw();
            }
        private:
            Model model;
    };  
};