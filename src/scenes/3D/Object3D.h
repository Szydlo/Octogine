#pragma once

#include "../Object.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Octo
{
    class Object3D : public Object
    {
        public:
            Object3D(std::string name = "");

            void setPosition(glm::vec3 pos) { m_Position = pos; }
            void setRotation(glm::vec3 rot) { m_Rotation = rot; }
            void setScale(glm::vec3 scale) { m_Scale = scale; }

            glm::mat4 getTransform();
            bool isVisible() { return m_Visible; }
        protected:
            glm::vec3 m_Position, m_Rotation, m_Scale;
            bool m_Visible = true;
    };
};