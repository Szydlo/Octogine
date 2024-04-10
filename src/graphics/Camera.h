#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Octo 
{
    class Camera
    {
        public:
            Camera(glm::vec2 resolutiion, glm::vec3 position = glm::vec3(0));
            ~Camera();

            glm::mat4 getViewMatrix();
            glm::mat4 getProjectionMatrix();

            glm::vec3 getPosition() { return m_Position; }
            glm::vec3 getFront()    { return m_Front; }
            glm::vec3 getRight()    { return m_Right; }
            glm::vec3 getUp() { return m_Up; }
            glm::vec3 getWorldUp() { return m_WorldUp; }

            void setPosition(glm::vec3 pos) {m_Position = pos; calculateCameraVectors();}
            void setYaw(float yaw) {m_Yaw = yaw; calculateCameraVectors();}
            void setPitch(float pitch) {m_Pitch = pitch; calculateCameraVectors();}
        private:
            glm::vec2 m_Resolution;

            glm::vec3 m_Position;
            glm::vec3 m_Right;
            glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
            glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

            float m_Yaw = -90.0f;
            float m_Pitch = 0.0f;
            float m_FOV = 90.0f; 

            float m_Near = 0.1f;
            float m_Far = 100000.0f;

            void calculateCameraVectors();
    };
};