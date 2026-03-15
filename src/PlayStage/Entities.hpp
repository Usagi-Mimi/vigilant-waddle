#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include "GameWindow.hpp"
#include "Action.hpp"
#include "Grid.hpp"

#include <SDL3/SDL_rect.h>

#include <vector>

struct SDL_Texture;

namespace vw
{

class Grid;

/**
 * @brief The collection, and manager, of entities -- the things that walk
 * around, fight, and die on the tile grid (like the player!)
 */
class Entities final
{
struct Coordinates
{
    // Spritesheet coordinates
    float x_sprite;
    float y_sprite;
    // Tile coordinates
    int x_tile;
    int y_tile;
};

public:
    Entities(GameWindow& win,
             SDL_Texture* entity_sprites,
             Grid& grid);

    ~Entities();

    void update();
    void render();

    /**
     * @brief Prepares the entities to handle input
     *
     * WARNING: This MUST be called before any of the input functions below!
     */
    void brace_for_input();

    /**
     * @brief Convenience functions for inserting actions for the player entity
     * based on player input
     * @{
     */
    void player_move(Direction const d);
    void player_wait();
    /**
     * @}
     */

private:
    /**
     * @return The player's tile coordinates
     * @{
     */
    int player_x() const;
    int player_y() const;
    /**
     * @}
     */

    /**
     * @return Whether an entity occupies the tile at the given coordinates
     */
    bool at(int const x_tile, int const y_tile) const;

    /**
     * @return Whether the given action is valid (e.g., whether a move is within
     * the bounds of the grid)
     */
    bool invalid_action(Action const& a) const;

    /**
     * @brief Executes a turn of the game, carrying out all entities' actions
     */
    void play_turn();

    GameWindow& win;

    /**
     * @brief The entity spritesheet
     */
    SDL_Texture* entity_sprites;

    Grid& grid;

    /**
     * @brief Entities' sprite and tile coordinates
     */
    std::vector<Coordinates> coords;

    /**
     * @brief Entities' actions in the current turn
     */
    std::vector<Action> actions;
};

}

#endif
