#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture2D.h"

#include "../lighting/DirectionalLight.h"

namespace Octo
{
    class Renderer
    {
        public:
            static void basicDraw(VertexArray& vao, Shader& shader, Texture2D& txt, unsigned int count, glm::mat4 model = glm::mat4(1));

            inline static void setMainCamera(Camera& camera) { m_MainCamera = &camera; }
            inline static void setDirectionalLight(DirectionalLight& light) { m_DirLight = &light; }
        private:
            inline static Camera* m_MainCamera;
            inline static DirectionalLight* m_DirLight;
    };
};