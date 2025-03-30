#pragma once

#include "../src/Octogine.h"

namespace DockingSpace
{
    static void begin()
    {
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;
        ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        bool isOpen = false;
        ImGui::Begin("Amateur Editor", &isOpen, windowFlags);

        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockSpaceFlags);
        }
    }

    static void topBar(Octo::Window& window)
    {
        if (ImGui::BeginMenuBar())
        {
            ImGui::Text("Octodit");

            if (ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Exit"))
                {
                    window.closeWindow();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

    static void end()
    {
        ImGui::End();
    }
};