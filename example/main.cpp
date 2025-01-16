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
        })
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

    /*
    bool ent = false;
    bool is3d = false;

    void setInsepctor(Octo::Entity& ent)
    {
        selectedNode = &ent;
    }

    void hierarchy(Octo::Entity& ent)
    {
        if (!ent.hasChildren())
            if (ImGui::TreeNodeEx(ent.getName().c_str(), ImGuiTreeNodeFlags_Leaf)) 
            { 
                if (ImGui::IsItemClicked())
                {
                    setInsepctor(ent);
                }

                ImGui::TreePop(); 

                return; 
            }

        if (ImGui::TreeNodeEx(ent.getName().c_str())) 
        { 
            if (ImGui::IsItemClicked())
            {
                setInsepctor(ent);
            }

            for (auto& child : ent.getChildren())
            {
                hierarchy(child);
            }

            ImGui::TreePop(); 
        }
    }*/

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

    /*
       ImGui::Begin("Scene Hierarchy");

        if (ImGui::TreeNodeEx(("scene: %s", scene.getName().c_str())))
        {
            for (auto& object : scene.getChildren())
            {
                hierarchy(object);
            }

            ImGui::TreePop();
        }

        scene.update(delta);

        ImGui::End();

        ImGui::Begin("Inspector");

        if (selectedNode != nullptr)
        {
            ImGui::LabelText(selectedNode->getName().c_str(), "name: ");

            if (dynamic_cast<Octo::Node3D*>(selectedNode))
            {
                Octo::Node3D* sn = dynamic_cast<Octo::Node3D*>(selectedNode);

                float pos[3];
            
                pos[0] = sn->getPosition().x;
                pos[1] = sn->getPosition().y;
                pos[2] = sn->getPosition().z;

                ImGui::InputFloat3("pos", pos);
            }
        }

        ImGui::End();*/
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