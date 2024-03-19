#include "../src/Octogine.h"

#include <iostream>
#include <filesystem>

class Game
{
public:
    Game()
        : window(1920, 1080, "Basic application"), shader("../../../assets/shaders/main.vs", "../../../assets/shaders/main.fs"), vbo(Octo::BufferType::Array), ebo(Octo::BufferType::Element)
    {
        Events::onStart.connect(&Game::start, this);
        Events::onClick.connect(&Game::click, this);

        shader.bind();

        window.start();
    }

    void start()
    {
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
        vbo.unbind();

        ebo.bind();
        ebo.setData(sizeof(indices), indices, Octo::DrawMode::STATIC);
        ebo.unbind();

        vao.setAttribute(0, 3, 3 * sizeof(float), 0);

        vao.unbind();
    }

    void click(int key, bool pressed)
    {
        if (!pressed) return;

        if (key == GLFW_KEY_ESCAPE)
        {
            window.closeWindow();
        }
    }

    Octo::Window window;
    Octo::Shader shader;
    Octo::VertexArray vao;
    Octo::Buffer vbo;
    Octo::Buffer ebo;
};

int main() 
{ 
    Game game;
}