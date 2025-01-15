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
        Octo::Renderer::setSkyBox(skybox);
        Octo::Renderer::setDirectionalLight(dirLight);
        Octo::Input::setCursorMode(Octo::CursorMode::disabled);

        Octo::AssetManager::loadAsset("cube", Octo::AssetType::Model, "../../../assets/models/cube.glb");
        cube = Octo::AssetManager::getAsset<Octo::Model>("cube");
        //Octo::AssetManager::loadAsset("backjpg", Octo::AssetType::Texture2D, "../../../assets/textures/skybox/back.jpg");
        //auto asset = Octo::AssetManager::getAsset<Octo::Texture2D>("backjpg");
      //  Octo::Asset<Octo::Texture2D> asset;
       // auto asdas = asset.get();
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

    void scene(bool depth)
    {
        cube->setTransform(glm::mat4(1.0));
        cube->setColor(glm::vec3(1.0, 0.0, 0.0));

        Octo::Shader& cubeShader = cube->getShader();
        cubeShader.bind();
        cubeShader.setInt("shadowMap", 0);
        cubeShader.setMat4("lightSpaceMatrix", shadow.getLightSpaceMatrix());

        shadow.getDepthTexture().bind();
        if (!depth) cube->draw(true);
        if (depth) cube->draw(shadow.getDepthShader());

        glm::mat4 transform = glm::mat4(1.0);
        transform = glm::translate(transform, glm::vec3(0, -2, 0));
        transform = glm::scale(transform, glm::vec3(5, 0.2, 5.0));

        cube->setTransform(transform);
        cube->setColor(glm::vec3(0.2, 0.8, 0.1));

        shadow.getDepthTexture().bind();
        if (!depth) cube->draw(true);
        if (depth) cube->draw(shadow.getDepthShader());
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

        shadow.startPass();
        scene(true);
        shadow.endPass();

        scene(false);
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
    Octo::DirectionalLight dirLight;
    Octo::Shadow shadow;
    std::shared_ptr<Octo::Model> cube;
    
    float cameraSpeed = 5.0f;
    float mouseSensivity = 0.4f;

    std::optional<float> lastMouseX, lastMouseY;

    float yaw = -90.0f;
    float pitch = 0.0f;

    glm::mat4 floor = glm::mat4(1.0f);
};

int main() 
{ 
    Game game;
}