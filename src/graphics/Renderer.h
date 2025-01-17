#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture2D.h"

#include "../lighting/SkyBox.h"

#include "../lighting/DirectionalLight.h"
#include "../lighting/Shadow.h"

namespace Octo
{
    struct DrawElement
    {
        VertexArray* vao;
        Shader* shader;
        unsigned int count;
        glm::mat4 transform;
    };

    class Renderer
    {
        public:
            static void basicDraw(VertexArray& vao, Shader& shader, unsigned int count, glm::mat4 model = glm::mat4(1));
            static void basicDraw(VertexArray& vao, Shader& shader, Texture2D& txt, unsigned int count, glm::mat4 model = glm::mat4(1));

            static void startPass();
            static void drawElement(DrawElement& el, Shader* shader);

            inline static void setMainCamera(Camera& camera) { m_MainCamera = &camera; }
            inline static void setDirectionalLight(DirectionalLight& light) { m_DirLight = &light; }
            inline static void setShadow(Shadow& shadow) { m_Shadow = &shadow; }
            inline static void setSkyBox(SkyBox& skybox) { m_SkyBox = &skybox; }
        
            inline static Camera* getMainCamera() { return m_MainCamera; }
            inline static DirectionalLight* getDirectionalLight() { return m_DirLight; }
            inline static SkyBox* getSkyBox() { return m_SkyBox; }

            static void endPass();

            static void destroy();
        private:
            static void drawSkyBox(SkyBox* skybox);

            inline static Camera* m_MainCamera;
            inline static DirectionalLight* m_DirLight;
            inline static SkyBox* m_SkyBox;
            inline static Shadow* m_Shadow = nullptr;
            inline static std::vector<DrawElement> m_DrawQueue;
    };
};