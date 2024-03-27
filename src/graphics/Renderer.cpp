#include "Renderer.h"

using Octo::Renderer;

void Renderer::basicDraw(VertexArray& vao, Shader& shader, Texture2D& txt, unsigned int count, glm::mat4 model)
{
    if (!m_MainCamera) return;
    
    shader.bind();
    txt.bind();

    shader.setMat4("projection", m_MainCamera->getProjectionMatrix());
    shader.setMat4("view", m_MainCamera->getViewMatrix());
    shader.setMat4("model", model);

    shader.setVec3("viewPos", m_MainCamera->getPosition());

    if (m_DirLight)
    {
        m_DirLight->setShader(shader);
    }

    vao.bind();

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

    vao.unbind();
    shader.unbind();
}