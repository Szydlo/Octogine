#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture2D.h"

namespace Octo
{
    class Renderer
    {
        public:
            static void basicDraw(VertexArray& vao, Shader& shader, Texture2D& txt, unsigned int count, glm::mat4 model = glm::mat4(1));
            inline static void setMainCamera(Camera& camera) { m_MainCamera = std::make_shared<Camera>(camera); }
        private:
            inline static std::shared_ptr<Camera> m_MainCamera;
    };
};