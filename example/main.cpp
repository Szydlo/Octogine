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
        modelCube("../../../assets/models/cube.glb", true, glm::vec3(0.8)),
        skybox({
            "../../../assets/textures/skybox/right.jpg",
            "../../../assets/textures/skybox/left.jpg",
            "../../../assets/textures/skybox/top.jpg",
            "../../../assets/textures/skybox/bottom.jpg",
            "../../../assets/textures/skybox/front.jpg",
            "../../../assets/textures/skybox/back.jpg"
        }),
        shadow(glm::vec2(1024))
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
        Octo::Renderer::setSkyBox(skybox);

        Octo::Input::setCursorMode(Octo::CursorMode::disabled);

        modelCube.setMaterial({ {1.0f, 0.5f, 0.31f}, {1.0f, 0.5f, 0.31f}, {0.5f, 0.5f, 0.5f}, {64.0f} });
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

    glm::mat4 model1 = glm::mat4(1);

    void rndScene(bool depth)
    {
        modelCube.setColor({1.0, 0.0, 0.0});

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0));
        model = glm::scale(model, glm::vec3(10.0, 0.2, 10.0));

        modelCube.getShader().bind();
        modelCube.getShader().setInt("shadowMap", 0);
        modelCube.getShader().setMat4("lightSpaceMatrix", shadow.getLightSpaceMatrix());

        modelCube.setTransform(model);
        if (!depth) modelCube.draw();
        else modelCube.draw(shadow.getDepthShader());

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));

        modelCube.setTransform(model);
        if (!depth) modelCube.draw();
        else modelCube.draw(shadow.getDepthShader());

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
        model = glm::scale(model, glm::vec3(0.5f));

        modelCube.setTransform(model);
        if (!depth) modelCube.draw();
        else modelCube.draw(shadow.getDepthShader());

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(0.25));

        modelCube.setTransform(model);
        if (!depth) modelCube.draw();
        else modelCube.draw(shadow.getDepthShader());
    }

    void update(double delta)
    {
        glm::vec2 inputDirection( Octo::Input::getDirection(GLFW_KEY_W, GLFW_KEY_S), Octo::Input::getDirection(GLFW_KEY_D, GLFW_KEY_A));  
        inputDirection *= cameraSpeed * delta;
        glm::vec3 moveDirection = camera.getPosition() + (camera.getFront() * inputDirection.x) + (camera.getRight() * inputDirection.y);
        camera.setPosition(moveDirection);

        shadow.startPass();
        rndScene(true);
        shadow.endPass();

        rndScene(false);

        ImGui::Begin("Framebuffer preview");
        shadow.getDepthTexture().bind();
        ImGui::Image((ImTextureID)(shadow.getDepthTexture().getIdentity()), ImVec2(1024, 1024), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
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
    Octo::Model model;
    Octo::Model modelCube;

    Octo::Shadow shadow;

    Octo::DirectionalLight dirLight;

    Octo::SkyBox skybox;

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