#pragma once

#include "../src/Octogine.h"

#include <iostream>
#include <filesystem>

#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/type_ptr.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"

#include <vector>
#include <optional>

namespace Inspector
{
    inline static Octo::Entity* selectedNode = nullptr;

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

    void draw(Octo::Scene& scene)
    {
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
                if (ImGui::InputFloat3("position", posArray))
                {  
                    if (selectedNode->isA<Octo::Rigidbody3D>())
                    {
                        Octo::Rigidbody3D* rbody3D = selectedNode->as<Octo::Rigidbody3D*>();
                        rbody3D->getBody().setPosition(transform.position);
                    }
                }
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

                if (ImGui::CollapsingHeader("Material"))
                {
                    Octo::Material& mat = model3D->getModel().getMeshes()[0].material;

                    float* albedoArray = &mat.albedo.x;

                    ImGui::InputFloat3("albedo", albedoArray);

                    ImGui::InputFloat("metallic", &mat.metallic);
                    ImGui::InputFloat("roughness", &mat.roughness);
                    ImGui::InputFloat("ao", &mat.ao);

                    ImGui::LabelText("normal txt", "albedo txt");
                    ImGui::Image((void*)mat.albedoTXT.getIdentity(), ImVec2(64, 64));
                    ImGui::SameLine();
                    ImGui::Image((void*)mat.normalTXT.getIdentity(), ImVec2(64, 64));
                }
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

            if (selectedNode->isA<Octo::Pointlight3D>() && ImGui::CollapsingHeader("Pointlight3D"))
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

            if (selectedNode->isA<Octo::Rigidbody3D>() && ImGui::CollapsingHeader("Rigidbody3D"))
            {
                Octo::Rigidbody3D* rbody3D = selectedNode->as<Octo::Rigidbody3D*>();
            }

            if (selectedNode->isA<Octo::Collider3D>() && ImGui::CollapsingHeader("Collider3D"))
            {
                Octo::Collider3D* collider3D = selectedNode->as<Octo::Collider3D*>();
            }
        }

        ImGui::End();
    }
}; 