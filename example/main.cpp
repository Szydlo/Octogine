#include "../src/Octogine.h"

#include <iostream>
#include <filesystem>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <optional>

#include "inspector.h"
#include "../src/lighting/Enivroment.h"

class Game
{
public:
    Game()
        : window(1920, 1080, "Basic application"),
        camera(glm::vec2(1920, 1080), glm::vec3(0, 0, 3)),
        env("../../assets/textures/hdrmap.hdr"),
        sphere("../../assets/models/sphere.glb")
    {
        Events::onStart.connect(&Game::start, this);
        Events::onClick.connect(&Game::click, this);
        Events::onUpdate.connect(&Game::update, this);
        Events::onMouseMove.connect(&Game::mouseMove, this);
        
        window.start();
    }

    void start()
    {
        Octo::Renderer::setMainCamera(camera);
        Octo::Renderer::setEnivroment(env);
        Octo::Input::setCursorMode(Octo::CursorMode::disabled);
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

        {
            ImGui::Begin("Game settings");
            ImGui::DragFloat("camera speed", &cameraSpeed);
            ImGui::DragFloat("mouse sens ", &mouseSensivity);
            ImGui::LabelText(std::to_string(window.getFPS()).c_str(), "FPS");

            auto cameraPos = camera.getPosition();
            auto cameraRot = camera.getFront();

            ImGui::InputFloat3("camera pos", &cameraPos.x);
            ImGui::InputFloat3("camera rot", &cameraRot.x);
            ImGui::End();
        }

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
            sphere.draw();
        }
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
    //Octo::SkyBox skybox;
    Octo::Model sphere;

    Octo::Enivroment env;

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