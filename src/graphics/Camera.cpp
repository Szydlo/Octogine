#include "Camera.h"

using Octo::Camera;

Camera::Camera(glm::vec2 resolution, glm::vec3 position)
    : m_Resolution(resolution), m_Position(position)
{
    calculateCameraVectors();
}

Camera::~Camera()
{
    
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(m_FOV), 
        static_cast<float>(m_Resolution.x) / static_cast<float>(m_Resolution.y), 
        m_Near, m_Far);
}

void Camera::calculateCameraVectors()
{
	glm::vec3 front(
		cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
		sin(glm::radians(m_Pitch)),
		sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))
	);

	m_Front = glm::normalize(front);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}