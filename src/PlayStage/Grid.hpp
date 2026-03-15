#ifndef GRID_HPP
#define GRID_HPP

#include "GameWindow.hpp"

#include <SDL3/SDL_rect.h>

#include <array>
#include <random>

struct SDL_Texture;

namespace vw
{

enum class Direction { N, S, E, W, NE, NW, SE, SW };

/**
 * @brief The grid of tiles where entities walk around, fight, and die
 *
 * This is responsible for how tile (x, y) coordinates are translated to canvas
 * coordinates so other code doesn't have to worry about it. Tile coordinates
 * start at (0, 0), which designates the top-leftmost tile in the grid. They are
 * never negative (even if they're signed): https://youtu.be/Puio5dly9N8?t=2593
 */
class Grid final
{
struct Coordinates
{
    // Spritesheet coordinates
    float x_sprite;
    float y_sprite;
    // Canvas coordinates
    float x_canvas;
    float y_canvas;
};

enum class Type { Snow, };

public:
    Grid(GameWindow& win,
         std::mt19937& rng,
         SDL_Texture* floor_sprites);

    ~Grid();

    void render();

    /**
     * @return Conversions to canvas coordinates from tile coordinates
     *
     * Specifically, these give the canvas coordinate of the top-left corner of
     * all tiles with the corresponding tile coordinate. For example, all tiles
     * in the second row of the grid have tile y-coordinate 1, and each of their
     * top-left corners all have some canvas y-coordinate returned by
     * `y_canvas(1)`.
     * @{
     */
    static float x_canvas(int const x_tile);
    static float y_canvas(int const y_tile);
    /**
     * @}
     */

    /**
     * @return Whether the given tile coordinates are valid
     */
    static bool in_bounds(int const x_tile, int const y_tile);

private:
    /**
     * @return Sprite coordinates fit for a tile of the given type
     */
    std::pair<float, float> pick_tile_sprite_coords(Type const t);

    /**
     * @brief Conversions between conceptual 2D (x, y) coordinates and the index
     * index of their corresponding tile in the physical 1D tile array
     * @{
     */
    static int index_tile(int const x_tile, int const y_tile);
    static int x_tile(int const index_tile);
    static int y_tile(int const index_tile);
    /**
     * @}
     */

    GameWindow& win;

    std::mt19937& rng;

    /**
     * @brief The floor spritesheet
     */
    SDL_Texture* floor_sprites;

    /**
     * @brief All tiles' sprite and canvas coordinates
     */
    std::array<Coordinates, g_tiles> coords;
    std::array<Type, g_tiles> type;
};

}

#endif
