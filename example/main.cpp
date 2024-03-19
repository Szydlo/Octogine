#include "../src/Octogine.h"

class Game 
{
public:
    Game()
    {
      //  Events::onStart.connect(Game::start, start);
       // Events::onStart.connect(update);
       Events::onUpdate.connect(&Game::update, this);
    }

    void start()
    {
        spdlog::info("started");
    }

    void update(double delta)
    {
        spdlog::info("{}", delta);
    }
};

void update(double s)
{
    spdlog::info("a: {}", s);
}

int main() 
{ 
    Game game;

    Octo::Core engine;
    Octo::Window window();
}