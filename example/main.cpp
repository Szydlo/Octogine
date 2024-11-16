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
        cube("../../../assets/models/archer.glb"),
        anim("../../../assets/models/walkarch.glb"),
        anim2("../../../assets/models/run.glb"),
        animator(&cube)
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

        //cube.setColor(glm::vec3(0, 1, 0));
        //cube.setTransform(floor);
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

        ImGui::Begin("blend");
        ImGui::SliderFloat("factor", &blendF, 0.0, 1.0);
        ImGui::End();

        glm::mat4 transform = glm::mat4(1.0);

        transform = glm::scale(transform, glm::vec3(0.01));
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(1, 0, 0));


        animator.prepareModel();
        //animator.updateAnimation(&anim, delta, transform);
        animator.updateBlended(&anim, &anim2, blendF, delta, transform);

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
    Octo::Animation anim;
    Octo::Animation anim2;
    Octo::Animator animator;

    float cameraSpeed = 5.0f;
    float mouseSensivity = 0.4f;

    float blendF = 0.0f;

    std::optional<float> lastMouseX, lastMouseY;

    float yaw = -90.0f;
    float pitch = 0.0f;

    glm::mat4 floor = glm::mat4(1.0f);
};

int main() 
{ 
    Game game;
}