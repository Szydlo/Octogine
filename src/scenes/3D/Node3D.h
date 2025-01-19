#pragma once

#include "../Entity.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
 

namespace Octo
{
    struct Transform
    {
        glm::vec3 position = glm::vec3(0.0);
        glm::vec3 rotation = glm::vec3(0.0);
        glm::vec3 scale = glm::vec3(1.0);

        glm::mat4 getMat4()
        {
            glm::mat4 pos = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 rot = glm::toMat4(glm::quat(rotation));
            glm::mat4 scl = glm::scale(glm::mat4(1.0f), scale);

            return pos * rot * scl;
        }

        glm::mat4 getGlobalMat4(Transform& parentTransform)
        {
            glm::mat4 pos = glm::translate(glm::mat4(1.0f), position + parentTransform.position);
            glm::mat4 rot = glm::toMat4(glm::quat(rotation));
            glm::mat4 scl = glm::scale(glm::mat4(1.0f), scale);


            return pos * rot * scl;
        }
    };

    class Node3D : public Entity
    {
        public:
            Node3D(std::string name) : Entity(name) {}

            void onUpdate(double delta) override
            {
                Entity::onUpdate(delta);

               /* if (m_Parent && m_Parent->isA<Octo::Node3D>())
                {
                    Octo::Node3D* parent = m_Parent->as<Octo::Node3D*>();


                }*/
            }

            Transform& getTransform() { return m_Transform; } 
        protected:
            Transform m_Transform;
    };
};