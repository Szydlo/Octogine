#include "../src/Octogine.h"

#include <iostream>
#include <filesystem>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <optional>

#include "inspector.h"

class Game
{
public:
    Game()
        : window(1920, 1080, "Basic application"),
        camera(glm::vec2(1920, 1080), glm::vec3(0, 0, 3)),
        /*skybox({
            "../../assets/textures/skybox/right.jpg",
            "../../assets/textures/skybox/left.jpg",
            "../../assets/textures/skybox/top.jpg",
            "../../assets/textures/skybox/bottom.jpg",
            "../../assets/textures/skybox/front.jpg",
            "../../assets/textures/skybox/back.jpg"
        }),*/
        hdrMap("../../assets/textures/hdrmap.hdr"),
        sphere("../../assets/models/sphere.glb"),
        cube("../../assets/models/cube.glb"),
        shader("../../assets/shaders/cubemap.vs", "../../assets/shaders/cubemap.fs"),
        bgShader("../../assets/shaders/background.vs", "../../assets/shaders/background.fs"),
        irrshader("../../assets/shaders/cubemap.vs", "../../assets/shaders/irradiance.fs"),
        preShader("../../assets/shaders/cubemap.vs", "../../assets/shaders/prefilter.fs"),
        brdfShader("../../assets/shaders/brdf.vs", "../../assets/shaders/brdf.fs"),
        cubeMap({512, 512}),
        irradianceMap({32, 32}),
        prefilterMap({128, 128}),
        brdtTXT(nullptr, {512, 512})
    {
        Events::onStart.connect(&Game::start, this);
        Events::onClick.connect(&Game::click, this);
        Events::onUpdate.connect(&Game::update, this);
        Events::onMouseMove.connect(&Game::mouseMove, this);
        
        window.start();
    }

    void start()
    {
        glDepthFunc(GL_LEQUAL);
        Octo::Renderer::setMainCamera(camera);
        //Octo::Renderer::setSkyBox(skybox);
        Octo::Input::setCursorMode(Octo::CursorMode::disabled);

        fb.bind();
        rb.bind();
        rb.renderBufferStorage({512, 512});
        rb.attachFrameBuffer(fb);

        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        shader.bind();
        shader.setInt("equirectangularMap", 0);
        shader.setMat4("projection", captureProjection);

        hdrMap.bind();

        glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.

        fb.bind();

        for (unsigned int i = 0; i < 6; ++i)
        {
            shader.setMat4("view", captureViews[i]);
            fb.setCubeMapFace(cubeMap, i);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto& mesh = cube.getMeshes()[0];

            mesh.getVAO().bind();
            glDrawElements(GL_TRIANGLES, mesh.getEBO().getCount(), GL_UNSIGNED_INT, 0);
            mesh.getVAO().unbind();
        }

        fb.unbind();

        cubeMap.bind();
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);


        irradianceMap.bind();

        fb.bind();
        rb.bind();
        rb.renderBufferStorage({32, 32});

        // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
        // -----------------------------------------------------------------------------
        irrshader.bind();
        irrshader.setInt("environmentMap", 0);
        irrshader.setMat4("projection", captureProjection);

        cubeMap.bind();

        glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
        fb.bind();
        for (unsigned int i = 0; i < 6; ++i)
        {
            irrshader.setMat4("view", captureViews[i]);
            fb.setCubeMapFace(irradianceMap, i);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto& mesh = cube.getMeshes()[0];

            mesh.getVAO().bind();
            glDrawElements(GL_TRIANGLES, mesh.getEBO().getCount(), GL_UNSIGNED_INT, 0);
            mesh.getVAO().unbind();
        }

        fb.unbind();


        prefilterMap.bind();
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        preShader.bind();
        preShader.setInt("environmentMap", 0);
        preShader.setMat4("projection", captureProjection);

        cubeMap.bind();

        fb.bind();
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth  = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));

            rb.bind();
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            preShader.setFloat("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                preShader.setMat4("view", captureViews[i]);

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap.getIdentity(), mip);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                auto& mesh = cube.getMeshes()[0];

                mesh.getVAO().bind();
                glDrawElements(GL_TRIANGLES, mesh.getEBO().getCount(), GL_UNSIGNED_INT, 0);
                mesh.getVAO().unbind();
            }
        }
        fb.unbind();


        brdtTXT.bind();

        // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
        fb.bind();
        rb.bind();
        rb.renderBufferStorage({512, 512});
        fb.setTexture2D(brdtTXT);

        glViewport(0, 0, 512, 512);
        brdfShader.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        unsigned int quadVAO = 0;
        unsigned int quadVBO;

        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        fb.unbind();


        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)1920 / (float)1080, 0.1f, 100.0f);

        bgShader.bind();
        bgShader.setMat4("projection", projection);

        glViewport(0, 0, 1920, 1090);
    }

    void click(int key, bool pressed)
    {
        if (!pressed) return;

        if (key == GLFW_KEY_ESCAPE)
        {
            window.closeWindow();
        }
        else if (key == GLFW_KEY_C)
        {
            if (Octo::Input::getCursorMode() == Octo::CursorMode::disabled)
                Octo::Input::setCursorMode(Octo::CursorMode::normal);
            else
                Octo::Input::setCursorMode(Octo::CursorMode::disabled);
        }
    }

    void update(double delta)
    {
        // CAMERA INPUT
        if (Octo::Input::getCursorMode() == Octo::CursorMode::disabled)
        {
            glm::vec2 inputDirection( Octo::Input::getDirection(GLFW_KEY_W, GLFW_KEY_S), Octo::Input::getDirection(GLFW_KEY_D, GLFW_KEY_A));  
            inputDirection *= cameraSpeed * delta;
            glm::vec3 moveDirection = camera.getPosition() + (camera.getFront() * inputDirection.x) + (camera.getRight() * inputDirection.y);
            camera.setPosition(moveDirection);
        }

        ImGui::Begin("Game settings");
        ImGui::DragFloat("camera speed", &cameraSpeed);
        ImGui::DragFloat("mouse sens ", &mouseSensivity);
        ImGui::LabelText(std::to_string(window.getFPS()).c_str(), "FPS");

        auto cameraPos = camera.getPosition();
        auto cameraRot = camera.getFront();

        ImGui::InputFloat3("camera pos", &cameraPos.x);
        ImGui::InputFloat3("camera rot", &cameraRot.x);
        ImGui::End();

        for (int y = 0; y < 5; y++)
        for (int x = 0; x < 5; x++)
        {
            Octo::Material& material = sphere.getMeshes()[0].material;

            material.albedo = glm::vec3(0.5f, 0.0, 0.0);

            material.roughness = 1.0f / x;
            material.metallic = 1.0f / y;

            material.ao = 1.0;

            glm::mat4 mat = glm::mat4(1.0);
            mat = glm::translate(mat, glm::vec3(x * 2, y * 2, 0));

            sphere.setTransform(mat);

            irradianceMap.bind(10);
            prefilterMap.bind(11);
            brdtTXT.bind(12);

            sphere.getShader().bind();
            sphere.getShader().setInt("irradianceMap", 10);
            sphere.getShader().setInt("prefilterMap", 11);
            sphere.getShader().setInt("brdfLUT", 12);

            sphere.draw();
        }


        bgShader.bind();
        cubeMap.bind();
        //hdrMap.bind();
        //hdrMap.bind();
        bgShader.setInt("environmentMap", 0);
        cube.draw(bgShader);
        bgShader.unbind();
    }

    void mouseMove(double x, double y)
    {
        if (!lastMouseX.has_value()) {
            lastMouseX = x;
            lastMouseY = y;
        }

        float xOffset = (x - lastMouseX.value()) * mouseSensivity;
        float yOffset = (lastMouseY.value() - y) * mouseSensivity;

        yaw += xOffset;
        pitch += yOffset;

        pitch = std::clamp(pitch, -89.0f, 89.0f);

        lastMouseX = x;
        lastMouseY = y;

        if (Octo::Input::getCursorMode() == Octo::CursorMode::normal) return;

        camera.setYaw(yaw);
        camera.setPitch(pitch);
    }

    Octo::Window window;
    Octo::Camera camera;
    Octo::Shader shader;
    Octo::Shader irrshader;
    Octo::Shader bgShader;
    Octo::Shader preShader;
    Octo::Shader brdfShader;
    Octo::FrameBuffer fb;
    Octo::RenderBuffer rb;
    //Octo::SkyBox skybox;
    Octo::Texture2D hdrMap;
    Octo::Cubemap cubeMap;
    Octo::Cubemap irradianceMap;
    Octo::Cubemap prefilterMap;
    Octo::Texture2D brdtTXT;
    Octo::Model sphere;
    Octo::Model cube;

    float cameraSpeed = 5.0f;
    float mouseSensivity = 0.4f;

    std::optional<float> lastMouseX, lastMouseY;

    float yaw = -90.0f;
    float pitch = 0.0f;
};

int main() 
{ 
    Game game;
}