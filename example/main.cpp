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
        scene("basic scene"),
        node("node3d2"),
        model("nice model", "../../../assets/models/cube.glb"),
        floor("floor", "../../../assets/models/cube.glb"),
        sun("sun"),
        sLight("spot light"),
        pLight("point light")
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
        scene.addChild(&model);
        scene.addChild(&sun);
        scene.addChild(&floor);
        scene.addChild(&sLight);
//        scene.addChild(&pLight);

        sun.getTransform().rotation = glm::vec3(-2.0f, 4.0f, -1.0f);

        floor.getTransform().position = glm::vec3(0.0f, -1.0f, 0.0f);
        floor.getTransform().scale = glm::vec3(10.0f, 0.1f, 10.0f);
        floor.setColor({0.1, 0.8, 0.1});

        sLight.ambient = glm::vec3(0.5f);
        sLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        sLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        sLight.constant = 1.0f;
        sLight.linear = 0.09f;
        sLight.quadratic = 0.032f;
        sLight.cutOff = glm::cos(glm::radians(35.5f));
        sLight.outerCutOff = glm::cos(glm::radians(180.0f));

        pLight.getTransform().position = glm::vec3(0, 2, 0);
        pLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
        pLight.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        pLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        pLight.constant = 1.0f;
        pLight.linear = 0.09f;
        pLight.quadratic = 0.032f;
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


    void hierarchy(Octo::Entity* ent)
    {
        if (!ent->hasChildren())
            if (ImGui::TreeNodeEx(ent->getName().c_str(), ImGuiTreeNodeFlags_Leaf)) 
            { 
                if (ImGui::IsItemClicked())
                {
                    selectedNode = ent;
                }

                ImGui::TreePop(); 

                return; 
            }

        if (ImGui::TreeNodeEx(ent->getName().c_str())) 
        { 
            if (ImGui::IsItemClicked())
            {
                selectedNode = ent;
            }

            for (auto child : ent->getChildren())
            {
                hierarchy(child);
            }

            ImGui::TreePop(); 
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

        ImGui::Begin("Scene Hierarchy");

        if (ImGui::TreeNodeEx(("scene: %s", scene.getName().c_str()), ImGuiTreeNodeFlags_Leaf))
        {
            if (ImGui::IsItemClicked())
            {
                selectedNode = &scene;
            }

            for (auto object : scene.getChildren())
            {
                hierarchy(object);
            }

            ImGui::TreePop();
        }

        ImGui::End();

        ImGui::Begin("inspector");

        if (!selectedNode) { ImGui::LabelText("none", "selected node: "); }
        else 
        {
            ImGui::LabelText(selectedNode->getName().c_str(), "selected node: ");

            if (selectedNode->getParent())
            {
                std::string& parentName = selectedNode->getParent()->getName();
                ImGui::LabelText(parentName.data(), "parent: ");
            }
            else 
            {
                ImGui::LabelText("none", "parent: ");
            }

            if (ImGui::CollapsingHeader("Entity"))
            {
                ImGui::InputText(selectedNode->getName().data(), "name: ", selectedNode->getName().size(), ImGuiInputTextFlags_ReadOnly);
            }

            if (selectedNode->isA<Octo::Node3D>() && ImGui::CollapsingHeader("Node3D"))
            {
                Octo::Node3D* node3D = selectedNode->as<Octo::Node3D*>();
                Octo::Transform& transform = node3D->getTransform();

                // @TODO YEY UNSAFE CODE!!!
                float* posArray = &transform.position.x;
                float* rotArray = &transform.rotation.x;
                float* scaleArray = &transform.scale.x;

                ImGui::LabelText("", "Transform");
                ImGui::InputFloat3("position", posArray);
                ImGui::InputFloat3("rotation", rotArray);
                ImGui::InputFloat3("scale", scaleArray);
            }

            if (selectedNode->isA<Octo::Model3D>() && ImGui::CollapsingHeader("Model3D"))
            {
                Octo::Model3D* model3D = selectedNode->as<Octo::Model3D*>();
                std::string& modelPath = model3D->getPath();

                ImGui::InputText("path", modelPath.data(), modelPath.size(), ImGuiInputTextFlags_ReadOnly);

                glm::vec3& color = model3D->getColor();

                float* colorArr = &color.x;
                ImGui::ColorPicker3("color", colorArr);
            }

            if (selectedNode->isA<Octo::Sun3D>() && ImGui::CollapsingHeader("Sun3D"))
            {
                Octo::Sun3D* sun3D = selectedNode->as<Octo::Sun3D*>();
            }

            if (selectedNode->isA<Octo::Spotlight3D>() && ImGui::CollapsingHeader("Spotlight3D"))
            {
                Octo::Spotlight3D* spotLight3D = selectedNode->as<Octo::Spotlight3D*>();


                float* ambientArr = &spotLight3D->ambient.x;
                float* diffuseArr = &spotLight3D->diffuse.x;
                float* specularArr = &spotLight3D->specular.x;

                ImGui::InputFloat3("ambient", ambientArr);
                ImGui::InputFloat3("diffuse", diffuseArr);
                ImGui::InputFloat3("specular", specularArr);

                ImGui::InputFloat("constant", &spotLight3D->constant);
                ImGui::InputFloat("linear", &spotLight3D->linear);
                ImGui::InputFloat("quadratic", &spotLight3D->quadratic);
                ImGui::InputFloat("cutOff", &spotLight3D->cutOff);
                ImGui::InputFloat("outerCutOff", &spotLight3D->outerCutOff);
            }

            if (selectedNode->isA<Octo::Pointlight3D>() && ImGui::CollapsingHeader("Pointlight3d"))
            {
                Octo::Pointlight3D* pointLight3D = selectedNode->as<Octo::Pointlight3D*>();

                float* ambientArr = &pointLight3D->ambient.x;
                float* diffuseArr = &pointLight3D->diffuse.x;
                float* specularArr = &pointLight3D->specular.x;

                ImGui::InputFloat3("ambient", ambientArr);
                ImGui::InputFloat3("diffuse", diffuseArr);
                ImGui::InputFloat3("specular", specularArr);

                ImGui::InputFloat("constant", &pointLight3D->constant);
                ImGui::InputFloat("linear", &pointLight3D->linear);
                ImGui::InputFloat("quadratic", &pointLight3D->quadratic);
            }
        }

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
    Octo::SkyBox skybox;
    Octo::Scene scene;
    Octo::Entity* selectedNode = nullptr;
    Octo::Node3D node;
    Octo::Model3D model;
    Octo::Sun3D sun;
    Octo::Model3D floor;
    Octo::Spotlight3D sLight;
    Octo::Pointlight3D pLight;

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