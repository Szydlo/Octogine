#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"

#include "Texture2D.h"

#include "FrameBuffer.h"
#include "RenderBuffer.h"


#include "../lighting/Enivroment.h"
#include "../lighting/SkyBox.h"
#include "../lighting/LightingManager.h"
#include "../lighting/Material.h"

namespace Octo
{
    struct DrawElement
    {
        VertexArray* vao;
        Shader* shader;
        unsigned int count;
        glm::mat4 transform;
        Material* material = nullptr;
    };

    class Renderer
    {
        public:
            static void basicDraw(VertexArray& vao, Shader& shader, unsigned int count, const glm::mat4 &model = glm::mat4(1));
            static void basicDraw(VertexArray& vao, Shader& shader, Texture2D& txt, unsigned int count, const glm::mat4 &model = glm::mat4(1));
            static void basicDraw(VertexArray& vao, Shader& shader, Material* material, unsigned int count, const glm::mat4 &model);

            static void startPass();
            static void drawElement(const DrawElement& el, const Shader* shader);
            static void drawElement(const DrawElement& el, Shader* shader, Material* material);

            static void setMainCamera(Camera& camera) { m_MainCamera = &camera; }
            static void setSkyBox(SkyBox& skybox) { m_SkyBox = &skybox; }
            static void setEnivroment(Enivroment& enivroment) { m_Enivroment = &enivroment; }
        
            static Camera* getMainCamera() { return m_MainCamera; }
        
            static SkyBox* getSkyBox() { return m_SkyBox; }
            static void endPass();

            static void init();
            static void destroy();
        private:
            static void drawSkyBox(SkyBox* skybox);

            inline static Camera* m_MainCamera = nullptr;
            inline static SkyBox* m_SkyBox = nullptr;
            inline static Enivroment* m_Enivroment = nullptr;
            inline static std::vector<DrawElement> m_DrawQueue;
    };
};