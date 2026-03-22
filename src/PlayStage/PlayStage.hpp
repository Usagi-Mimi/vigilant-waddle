#ifndef PLAYSTAGE_HPP
#define PLAYSTAGE_HPP

#include "GameWindow.hpp"
#include "Grid.hpp"
#include "Entities.hpp"

#include <random>

union SDL_Event;
struct SDL_Texture;
struct SDL_Renderer;

namespace vw
{

/**
 * @brief Manages the tiles and entities where all the action happens
 */
class PlayStage final
{

public:
    PlayStage(GameWindow& win,
              std::string_view const floor_spritesheet_path,
              std::string_view const entity_spritesheet_path);

    void process_input(SDL_Event const* e);

    void update();

    void render();

private:
    GameWindow& win;

    std::mt19937 rng;

    Grid grid;
    Entities entities;
};

}

#endif
