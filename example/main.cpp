#include "../src/Octogine.h"

#include <iostream>
#include <filesystem>

class Game
{
public:
    Game()
        : window(1920, 1080, "Basic application"), shader("../../../assets/shaders/main.vs", "../../../assets/shaders/main.fs"), 
        vbo(Octo::BufferType::Array), ebo(Octo::BufferType::Element),
        camera(glm::vec2(1920, 1080), glm::vec3(0, 0, 1))
    {
        Events::onStart.connect(&Game::start, this);
        Events::onClick.connect(&Game::click, this);
        Events::onUpdate.connect(&Game::update, this);

        shader.bind();

        window.start();
    }

    void start()
    {
        Octo::Renderer::setMainCamera(camera);

        float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left 
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
        };

        spdlog::info("binding vao, vbo, ebo");
        vao.bind();

        vbo.bind();
        vbo.setData(sizeof(vertices), vertices, Octo::DrawMode::STATIC);

        ebo.bind();
        ebo.setData(sizeof(indices), indices, Octo::DrawMode::STATIC);

        vao.setAttribute(0, 3, 3 * sizeof(float), 0);

        vao.unbind();
        vbo.unbind();
        ebo.unbind();
    }

    void click(int key, bool pressed)
    {
        if (!pressed) return;

        if (key == GLFW_KEY_ESCAPE)
        {
            window.closeWindow();
        }
    }

    void update(double delta)
    {
        Octo::Renderer::basicDraw(vao, shader, ebo.getCount());
    }

    Octo::Window window;
    Octo::Shader shader;
    Octo::VertexArray vao;
    Octo::Buffer vbo;
    Octo::Buffer ebo;
    Octo::Camera camera;
};

int main() 
{ 
    Game game;
}