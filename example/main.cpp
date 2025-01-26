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
        skybox({
            "../../../assets/textures/skybox/right.jpg",
            "../../../assets/textures/skybox/left.jpg",
            "../../../assets/textures/skybox/top.jpg",
            "../../../assets/textures/skybox/bottom.jpg",
            "../../../assets/textures/skybox/front.jpg",
            "../../../assets/textures/skybox/back.jpg"
        }),
        scene("basic scene"),
        node("node3d2"),
        model("nice model", "../../../assets/models/cube.glb"),
        floor("floor", "../../../assets/models/cube.glb"),
        sun("sun")
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
        Octo::Renderer::setSkyBox(skybox);
        Octo::Input::setCursorMode(Octo::CursorMode::disabled);

        scene.addChild(&node);
        scene.addChild(&sun);
        scene.addChild(&floor);
        scene.addChild(&model);

        sun.getTransform().rotation = glm::vec3(-2.0f, 4.0f, -1.0f);

        floor.getTransform().position = glm::vec3(0.0f, -1.0f, 0.0f);
        floor.getTransform().scale = glm::vec3(10.0f, 0.1f, 10.0f);
        floor.setColor({0.1, 0.8, 0.1});

        model.getTransform().position = glm::vec3(0.0f, 1.0f, 0.0f);
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

        Inspector::draw(scene);
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
    Octo::SkyBox skybox;
    Octo::Scene scene;
    Octo::Node3D node;
    Octo::Model3D model;
    Octo::Sun3D sun;
    Octo::Model3D floor;

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