#include "Grid.hpp"

#include <SDL3_image/SDL_image.h>

#include <cassert>
#include <random>

using namespace vw;

std::pair<float, float> Grid::pick_tile_sprite_coords(Grid::Type const t)
{
    float x = 0;
    float y = 0;
    static std::uniform_int_distribution<int> dist(0, 4);

    switch (t)
    {
    case Type::Snow:
        y = 0; // Snow tiles are in the first row.
        int const snow_tile = dist(rng);
        x = snow_tile * g_sprite_w;
        break;
    }

    return {x, y};
}

Grid::Grid(GameWindow& win,
           std::mt19937& rng,
           SDL_Texture* floor_sprites) :
    win(win),
    rng(rng),
    floor_sprites(floor_sprites)
{
    // TODO Load maps with specific tile types that have different sprites.
    for (size_t i = 0; i < coords.size(); ++i)
    {
        type[i] = Type::Snow; // Right now, all tiles are snow.

        auto [x_sprite, y_sprite] = pick_tile_sprite_coords(type[i]);
        coords[i] =
        {
            // Spritesheet coords
            .x_sprite = x_sprite,
            .y_sprite = y_sprite,
            // Canvas coords
            .x_canvas = x_canvas(x_tile(i)),
            .y_canvas = y_canvas(y_tile(i))
        };
    }
}

Grid::~Grid()
{
    SDL_DestroyTexture(floor_sprites);
}

void Grid::render()
{
    static SDL_FRect sprite_target
        { 0, 0, (float) g_sprite_w, (float) g_sprite_h };
    static SDL_FRect canvas_target
        { 0, 0, (float) g_sprite_w, (float) g_sprite_h };

    for (Coordinates const& c : coords)
    {
        sprite_target.x = c.x_sprite;
        sprite_target.y = c.y_sprite;

        canvas_target.x = c.x_canvas;
        canvas_target.y = c.y_canvas;

        SDL_RenderTexture(win.ren, floor_sprites,
                          &sprite_target, &canvas_target);
    }
}

float Grid::x_canvas(int const x_tile)
{
    assert(x_tile >= 0);
    assert(x_tile < g_cols);
    return g_grid_start_x + (float) x_tile * g_sprite_w;
}

float Grid::y_canvas(int const y_tile)
{
    assert(y_tile >= 0);
    assert(y_tile < g_rows);
    return g_grid_start_y + (float) y_tile * g_sprite_h;
}

bool Grid::in_bounds(int const x_tile, int const y_tile)
{
    if (x_tile < 0 ||
        y_tile < 0 ||
        x_tile >= g_cols ||
        y_tile >= g_rows)
    {
        return false;
    }

    return index_tile(x_tile, y_tile) < g_tiles;
}

int Grid::index_tile(int const x_tile, int const y_tile)
{
    assert(x_tile >= 0);
    assert(x_tile < g_cols);
    assert(y_tile >= 0);
    assert(y_tile < g_rows);
    return y_tile * g_cols + x_tile;
}

int Grid::x_tile(int const index_tile)
{
    assert(index_tile >= 0);
    assert(index_tile < g_tiles);
    return index_tile % g_cols;
}

int Grid::y_tile(int const index_tile)
{
    assert(index_tile >= 0);
    assert(index_tile < g_tiles);
    return index_tile / g_cols;
}
