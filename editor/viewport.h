#pragma once

#include "../src/Octogine.h"

struct Viewport
{
    static void init()
    {
        Events::onRenderStartPass.connect(&Viewport::startPass);
        Events::onRenderEndPass.connect(&Viewport::endPass);

        frameBuffer = new Octo::FrameBuffer();
        frameTXT = new Octo::Texture2D(nullptr, {1920, 1080});

        frameBuffer->bind();
        frameTXT->bind();
        frameBuffer->setTexture2D(*frameTXT);
        frameBuffer->unbind();
    }

    static void startPass()
    {
        frameBuffer->bind();
        glViewport(0, 0, 1920, 1080);
    }

    static void endPass()
    {
        frameBuffer->unbind();
    }

    static void draw()
    {
        ImGui::Begin("Viewport");

        isFocused = ImGui::IsWindowFocused();
        frameTXT->bind();

        auto size = ImGui::GetContentRegionAvail();
        viewportSize = {size.x, size.y};

        ImGui::Image((ImTextureID)frameTXT->getIdentity(), size, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }

    static inline Octo::FrameBuffer* frameBuffer;
    static inline Octo::Texture2D* frameTXT;
    static inline glm::vec2 viewportSize = {512, 512};
    static inline bool isFocused;
};