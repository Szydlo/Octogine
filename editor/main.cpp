#include "../src/Octogine.h"
#include "spdlog/spdlog.h"

#include "inspector.h"
#include "baseUI.h"
#include "viewport.h"
#include "assetBrowser.h"
#include "log.h"
#include "hierarchy.h"

class Editor
{
public:
    Editor()
        : window(1920, 1080, "Octodit - an octogine editor"),
        camera(glm::vec2(1920, 1080), glm::vec3(0, 0, 3)),
        env("../../assets/textures/hdrmap.hdr"),
        scene("test scene")
    {
        Events::onStart.connect(&Editor::start, this);
        Events::onClick.connect(&Editor::click, this);
        Events::onUpdate.connect(&Editor::update, this);
        Events::onMouseMove.connect(&Editor::mouseMove, this);

        window.start();
    }

    void start()
    {
        Octo::Renderer::setMainCamera(camera);
        Octo::Renderer::setEnivroment(env);
        Octo::Input::setCursorMode(Octo::CursorMode::normal);

        Viewport::init();
        AssetBrowser::init();
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
        DockingSpace::begin();

        DockingSpace::topBar(window);

        Inspector::draw();
        Hierarchy::draw();
        Log::draw();
        AssetBrowser::draw();
        Viewport::draw();
        DockingSpace::end();

        //ImGui::ShowDemoWindow();
    }

    void mouseMove(double x, double y)
    {
        if (!Viewport::isFocused) return;
    }

    Octo::Window window;
    Octo::Camera camera;
    Octo::Enivroment env;
    Octo::Scene scene;
};

int main()
{
    Editor editor;
}