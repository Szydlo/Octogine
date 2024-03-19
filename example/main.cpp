#include "../src/Octogine.h"

class Game
{
public:
    Game()
        : window(1920, 1080, "Basic application")
    {
        Events::onClick.connect(&Game::click, this);

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
};

int main() 
{ 
    Game game;
}