#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Octo 
{
    class Camera
    {
        public:
            explicit Camera(glm::vec2 resolution, glm::vec3 position = glm::vec3(0));
            ~Camera();

            [[nodiscard]] glm::mat4 getViewMatrix() const;
            [[nodiscard]] glm::mat4 getProjectionMatrix() const;

            [[nodiscard]] glm::vec3 getPosition() const { return m_Position; }
            [[nodiscard]] glm::vec3 getFront() const { return m_Front; }
            [[nodiscard]] glm::vec3 getRight() const { return m_Right; }
            [[nodiscard]] glm::vec3 getUp() const { return m_Up; }
            [[nodiscard]] glm::vec3 getWorldUp() const { return m_WorldUp; }

            void setPosition(const glm::vec3 pos) {m_Position = pos; calculateCameraVectors();}
            void setYaw(const float yaw) {m_Yaw = yaw; calculateCameraVectors();}
            void setPitch(const float pitch) {m_Pitch = pitch; calculateCameraVectors();}
        private:
            glm::vec2 m_Resolution = {};

            glm::vec3 m_Position = {};
            glm::vec3 m_Right = {};
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