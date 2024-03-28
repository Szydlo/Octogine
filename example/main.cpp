#include "../src/Octogine.h"

#include <iostream>
#include <filesystem>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <optional>

class Game
{
public:
    Game()
        : window(1920, 1080, "Basic application"), 
        camera(glm::vec2(1920, 1080), glm::vec3(0, 0, 3)),
        model("../../../assets/models/viking.obj", true, glm::vec3(0.2, 0.5, 0.3)),
        modelCube("../../../assets/models/cube.glb", true, glm::vec3(0.8))
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
        Octo::Renderer::setDirectionalLight(dirLight);

        Octo::Input::setCursorMode(Octo::CursorMode::disabled);

        glm::mat4 viking = glm::mat4(1);
        viking = glm::rotate(viking, glm::radians(-90.0f), glm::vec3(1, 0, 0));
        model.setTransform(viking);

        glm::mat4 cube = glm::mat4(1);
        cube = glm::scale(cube, glm::vec3(5.0, 0.3, 5.0));
        cube = glm::translate(cube, glm::vec3(0, -3, 0));
        modelCube.setTransform(cube);

        modelCube.setMaterial({ {1.0f, 0.5f, 0.31f}, {1.0f, 0.5f, 0.31f}, {0.5f, 0.5f, 0.5f}, {64.0f} });

        sLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        sLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        sLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        sLight.constant = 1.0f;
        sLight.linear = 0.09f;
        sLight.quadratic = 0.032f;
        sLight.cutOff = glm::cos(glm::radians(35.5f));
        sLight.outerCutOff = glm::cos(glm::radians(180.0f));
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
        glm::vec2 inputDirection( Octo::Input::getDirection(GLFW_KEY_W, GLFW_KEY_S), Octo::Input::getDirection(GLFW_KEY_D, GLFW_KEY_A));  
        inputDirection *= cameraSpeed * delta;
        glm::vec3 moveDirection = camera.getPosition() + (camera.getFront() * inputDirection.x) + (camera.getRight() * inputDirection.y);
        camera.setPosition(moveDirection);

        //model.draw();

        glm::mat4 cube = glm::mat4(1);
        cube = glm::scale(cube, glm::vec3(5.0, 0.3, 5.0));
        cube = glm::translate(cube, glm::vec3(0, -3, 0));
        modelCube.setTransform(cube);

        modelCube.setColor({1.0, 1.0, 1.0});

        modelCube.getShader().bind();

        sLight.position = camera.getPosition();
        sLight.direction = camera.getFront();

        sLight.setShader(modelCube.getShader());

        modelCube.draw();

        cube = glm::mat4(1);
        modelCube.setTransform(cube);

        modelCube.setColor({0.0, 1.0, 0.0});

        modelCube.draw();
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

        camera.setYaw(yaw);
        camera.setPitch(pitch);

        lastMouseX = x;
        lastMouseY = y;
    }

    Octo::Window window;
    Octo::Camera camera;
    Octo::Model model;
    Octo::Model modelCube;
    Octo::SpotLight sLight;

    Octo::DirectionalLight dirLight;

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