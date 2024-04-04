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
        shadow(glm::vec2(1024)),
        depthShader("../../../assets/shaders/depth.vs", "../../../assets/shaders/depth.fs")
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

        glm::mat4 viking = glm::mat4(1);
        viking = glm::rotate(viking, glm::radians(-90.0f), glm::vec3(1, 0, 0));
        model.setTransform(viking);

        glm::mat4 cube = glm::mat4(1);
        cube = glm::scale(cube, glm::vec3(5.0, 0.3, 5.0));
        cube = glm::translate(cube, glm::vec3(0, -3, 0));
        modelCube.setTransform(cube);

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

    void renderScene(double delta, bool useDepth)
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
        
        //shadow.getDepthTexture().bind();

        if (useDepth) modelCube.draw(depthShader);
        else modelCube.draw();

        cube = glm::mat4(1);
        modelCube.setTransform(cube);

        modelCube.setColor({0.0, 1.0, 0.0});


        if (useDepth) modelCube.draw(depthShader);
        else modelCube.draw();
    }

    void update(double delta)
    {
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
     //   lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)1920 / (GLfloat)1080, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(dirLight.direction, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        depthShader.bind();
        depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        //depthShader.setMat4("model", lightSpaceMatrix);

        shadow.startPass();

        renderScene(delta, false);

        shadow.endPass();

        modelCube.getShader().bind();
        modelCube.getShader().setMat4("lightSpaceMatrix", lightSpaceMatrix);
        modelCube.getShader().setInt("shadowMap", 0);

    
        shadow.getDepthTexture().bind();
        renderScene(delta, false);

        float gowno[4];

        ImGui::Begin("Yass");
        ImGui::InputFloat4("ass", gowno);
        ImGui::End();
    }

    void mouseMove(double x, double y)
    {
        if (Octo::Input::getCursorMode() == Octo::CursorMode::normal) return;

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
    Octo::Shadow shadow;
    Octo::Shader depthShader;

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