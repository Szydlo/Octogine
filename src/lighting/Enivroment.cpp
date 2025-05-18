#include "Enivroment.h"

#include "spdlog/spdlog.h"

using Octo::Enivroment;

Enivroment::Enivroment(const std::string& hdrPath)
:   equirectShader("../../../assets/shaders/cubemap.vs", "../../../assets/shaders/cubemap.fs"),
    bgShader("../../../assets/shaders/background.vs", "../../../assets/shaders/background.fs"),
    irrShader("../../../assets/shaders/cubemap.vs", "../../../assets/shaders/irradiance.fs"),
    preShader("../../../assets/shaders/cubemap.vs", "../../../assets/shaders/prefilter.fs"),
    brdfShader("../../../assets/shaders/brdf.vs", "../../../assets/shaders/brdf.fs"),
    envMap({2048, 2048}),
    irradianceMap({32, 32}),
    prefilterMap({128, 128}),
    brdtTXT(nullptr, {2048, 2048}),
    hdrMap(hdrPath),
    m_CubeBuffer(BufferType::Array),
    m_QuadBuffer(BufferType::Array)
{
    frameBuff.bind();
    renderBuff.bind();
    renderBuff.renderBufferStorage({2048, 2048});
    renderBuff.attachFrameBuffer(frameBuff);

    createCube();
    createQuad();

    initEquirectangularMap();
    initIrradianceMap();
    initPrefilterMap();
    initBrdtf();
    resetViewport();

    glDepthFunc(GL_LEQUAL);
}

Enivroment::~Enivroment()
{

}


void Enivroment::createCube()
{
    m_CubeBuffer.bind();
    m_CubeBuffer.setData(sizeof(CubeVertices), CubeVertices, DrawMode::STATIC);

    m_CubeVAO.bind();

    m_CubeVAO.setAttribute(0, 3, 8 * sizeof(float), 0);
    m_CubeVAO.setAttribute(1, 3, 8 * sizeof(float), 3 * sizeof(float));
    m_CubeVAO.setAttribute(2, 2, 8 * sizeof(float), 6 * sizeof(float));

    m_CubeBuffer.unbind();
    m_CubeVAO.unbind();
}

void Enivroment::drawCube()
{
    m_CubeVAO.bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_CubeVAO.unbind();
}


void Enivroment::createQuad()
{
    m_QuadBuffer.bind();
    m_QuadBuffer.setData(sizeof(QuadVertices), QuadVertices, DrawMode::STATIC);

    m_QuadVAO.bind();

    m_QuadVAO.setAttribute(0, 3, 5 * sizeof(float), 0);
    m_QuadVAO.setAttribute(1, 2, 5 * sizeof(float), 3 * sizeof(float));

    m_QuadBuffer.unbind();
    m_QuadVAO.unbind();
}

void Enivroment::drawQuad()
{
    m_QuadVAO.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_QuadVAO.unbind();
}

void Enivroment::initEquirectangularMap()
{
    equirectShader.bind();
    equirectShader.setInt("equirectangularMap", 0);
    equirectShader.setMat4("projection", captureProjection);

    hdrMap.bind();

    glViewport(0, 0, 2048, 2048); // don't forget to configure the viewport to the capture dimensions.

    frameBuff.bind();

    for (unsigned int i = 0; i < 6; ++i)
    {
        equirectShader.setMat4("view", captureViews[i]);
        frameBuff.setCubeMapFace(envMap, i);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawCube();
    }

    frameBuff.unbind();
    envMap.createMipmap();
}

void Enivroment::initIrradianceMap()
{
    irradianceMap.bind();

    frameBuff.bind();
    renderBuff.bind();
    renderBuff.renderBufferStorage({32, 32});

    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    irrShader.bind();
    irrShader.setInt("environmentMap", 0);
    irrShader.setMat4("projection", captureProjection);

    envMap.bind();

    glViewport(0, 0, 32, 32);
    frameBuff.bind();

    for (unsigned int i = 0; i < 6; ++i)
    {
        irrShader.setMat4("view", captureViews[i]);
        frameBuff.setCubeMapFace(irradianceMap, i);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawCube();
    }

    frameBuff.unbind();
}

void Enivroment::initPrefilterMap()
{
    prefilterMap.createMipmap();

    preShader.bind();
    preShader.setInt("environmentMap", 0);
    preShader.setMat4("projection", captureProjection);

    envMap.bind();

    frameBuff.bind();

    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));

        renderBuff.bind();
        renderBuff.renderBufferStorage({mipWidth, mipHeight});

        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float) mip / (float) (maxMipLevels - 1);
        preShader.setFloat("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            preShader.setMat4("view", captureViews[i]);

            frameBuff.setCubeMapFace(prefilterMap, i, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            drawCube();
        }
    }

    frameBuff.unbind();
}

void Enivroment::initBrdtf()
{
    brdtTXT.bind();

    frameBuff.bind();
    renderBuff.bind();
    renderBuff.renderBufferStorage({2048, 2048});
    frameBuff.setTexture2D(brdtTXT);

    glViewport(0, 0, 2048, 2048);
    brdfShader.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawQuad();

    frameBuff.unbind();
}

void Enivroment::resetViewport()
{
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float) 1920 / (float) 1080, 0.1f, 100.0f);

    bgShader.bind();
    bgShader.setMat4("projection", projection);

    glViewport(0, 0, 1920, 1080);
}

void Enivroment::setShader(const Shader* shader)
{
    irradianceMap.bind(10);
    prefilterMap.bind(11);
    brdtTXT.bind(12);

    shader->bind();
    shader->setInt("irradianceMap", 10);
    shader->setInt("prefilterMap", 11);
    shader->setInt("brdfLUT", 12);
}

void Enivroment::drawBackground(Camera* camera)
{
    bgShader.bind();
    envMap.bind();
    bgShader.setInt("environmentMap", 0);
    bgShader.setMat4("view", camera->getViewMatrix());
    drawCube();
    bgShader.unbind();
}
