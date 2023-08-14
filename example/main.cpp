#include <iostream>

#include "game.hpp"
#include <memory>
int main()
{
    std::unique_ptr<Game> game(new Game());
    game->Run();
    return 0;
}