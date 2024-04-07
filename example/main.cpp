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
        skybox({
            "../../../assets/textures/skybox/right.jpg",
            "../../../assets/textures/skybox/left.jpg",
            "../../../assets/textures/skybox/top.jpg",
            "../../../assets/textures/skybox/bottom.jpg",
            "../../../assets/textures/skybox/front.jpg",
            "../../../assets/textures/skybox/back.jpg"
        }),
        cube("../../../assets/models/cube.obj", true),
        shape(glm::vec3(1.0)),
        collider(shape),
        rb(collider, Octo::BodyMode::Dynamic, PhysicsLayers::MOVING, true),
        scollider(shape),
        srb(collider, Octo::BodyMode::Static, PhysicsLayers::NON_MOVING, true)
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

        floor = glm::scale(floor, glm::vec3(10.0, 0.2, 10.0));
        floor = glm::translate(floor, glm::vec3(0, -6.2, 0));
    
        rbCube = glm::translate(rbCube, glm::vec3(0, 10, 0));

        rb.setPosition({0, 10, 0});
        srb.setPosition({0, 0, 0});
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
        else if (key == GLFW_KEY_R)
        {
            rb.setPosition({0, 10, 0});
        }
    }


    void update(double delta)
    {
        // CAMERA INPUT
        glm::vec2 inputDirection( Octo::Input::getDirection(GLFW_KEY_W, GLFW_KEY_S), Octo::Input::getDirection(GLFW_KEY_D, GLFW_KEY_A));  
        inputDirection *= cameraSpeed * delta;
        glm::vec3 moveDirection = camera.getPosition() + (camera.getFront() * inputDirection.x) + (camera.getRight() * inputDirection.y);
        camera.setPosition(moveDirection);

        //auto pos = rb.getPosition();
       // spdlog::info("{}, {}, {}", pos.x, pos.y, pos.z);

        cube.setColor(green);
        cube.setTransform(floor);
        cube.draw();

        cube.setColor(red);
        cube.setTransform(cubeTransform);
        cube.draw();

        cube.setColor(blue);

        rbCube = glm::translate(glm::mat4(1), rb.getPosition());

        cube.setTransform(rbCube);
        cube.draw();
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

    Octo::Model cube;

    Octo::BoxShape shape;
    Octo::Collider collider;
    Octo::RigidBody rb;

    Octo::Collider scollider;
    Octo::RigidBody srb;

    float cameraSpeed = 5.0f;
    float mouseSensivity = 0.4f;

    std::optional<float> lastMouseX, lastMouseY;

    float yaw = -90.0f;
    float pitch = 0.0f;

    glm::vec3 red = glm::vec3(1, 0, 0);
    glm::vec3 blue = glm::vec3(0, 0, 1);
    glm::vec3 green = glm::vec3(0, 1, 0);

    glm::mat4 cubeTransform = glm::mat4(1.0f);
    glm::mat4 floor = glm::mat4(1.0f);
    glm::mat4 rbCube = glm::mat4(1.0f);
};

int main() 
{ 
    Game game;
}