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
        cubeMap({512, 512})
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

        rb.attachFrameBuffer(fb);

        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
         };

        shader.bind();
        cubeMap.bind();
        hdrMap.bind();

        shader.setInt("equirectangularMap", 0);
        shader.setMat4("projection", captureProjection);

        glViewport(0, 0, 512, 512);

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


            /*//cube.draw(shader);
            Octo::DrawElement drawel;

            drawel.vao = &cube.getMeshes()[0].getVAO();
            drawel.transform = glm::mat4(1.0f);
            drawel.count = cube.getMeshes()[0].getEBO().getCount();
            drawel.shader = &shader;

            Octo::Renderer::drawElement(drawel, &shader);*/
        }

        fb.unbind();


        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
        bgShader.bind();
        bgShader.setMat4("projection", projection);


        glViewport(0, 0, 1920, 1080);
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

            material.roughness = glm::clamp((float)x / (float)5, 0.05f, 1.0f);
            material.metallic = (float)y / 5.0f;

            material.ao = 1.0;

            glm::mat4 mat = glm::mat4(1.0);
            mat = glm::translate(mat, glm::vec3(x * 2, y * 2, 0));

            sphere.setTransform(mat);
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
    Octo::Shader bgShader;
    Octo::FrameBuffer fb;
    Octo::RenderBuffer rb;
    //Octo::SkyBox skybox;
    Octo::Texture2D hdrMap;
    Octo::Cubemap cubeMap;
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