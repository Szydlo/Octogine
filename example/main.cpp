#include "../src/Octogine.h"

#include <iostream>
#include <filesystem>

class Game
{
public:
    Game()
        : window(1920, 1080, "Basic application"), shader("../../../assets/shaders/main.vs", "../../../assets/shaders/main.fs")
    {
        Events::onClick.connect(&Game::click, this);

        shader.bind();

        window.start();
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
    
    std::string currentPath = "../../../assets/";
};

int main() 
{ 
    Game game;
}