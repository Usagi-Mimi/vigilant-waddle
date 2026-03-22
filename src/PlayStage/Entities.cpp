#include "Entities.hpp"
#include "Grid.hpp"

#include <SDL3_image/SDL_image.h>

#include <ranges>
#include <algorithm>
#include <cassert>

vw::Entities::Entities(GameWindow& win,
                       SDL_Texture* entity_sprites,
                       Grid& grid) :
                       win(win),
                       entity_sprites(entity_sprites),
                       grid(grid)
{
    // The player is the first entity.
    Coordinates player_coords
    {
        // The player sprite's the first in the spritesheet...
        .x_sprite = 0,
        .y_sprite = 0,
        // ...and these are the player's starting tile coordinates.
        .x_tile = 0,
        .y_tile = 0
    };
    coords.push_back(player_coords);
}

vw::Entities::~Entities()
{
    SDL_DestroyTexture(entity_sprites);
}

void vw::Entities::update()
{
    if (actions.empty())
    {
        /*
         * If there was no actionable input from the player, we're not executing
         * a turn. We're still waiting for the player to do something on this
         * turn.
         */
        return;
    }

    if (invalid_action(actions.front()))
    {
        /*
         * If the player's action is invalid (like attempting to move into a
         * wall), discard it and await a valid action.
         */
        actions.clear();
        return;
    }

    /*
     * If we have an action from the player (the first action in the list), then
     * all NPC entities need to choose their actions.
     */
    assert(actions.size() == 1);
    // for (int i = 1; i < coords.size(); ++i)
    // {
    //     Action a = ai.at(i).think(); // TODO
    //     actions.push_back(a);
    // }

    /*
     * With all entity actions prepared, the current turn can be played. (It is
     * assumed that NPC actions are always valid -- that is: NPC AI will never
     * pick invalid actions. So, by now, all queued actions should be valid.)
     */
    play_turn();
}

void vw::Entities::render()
{
    static SDL_FRect sprite_target
        { 0, 0, (float) g_sprite_w, (float) g_sprite_h };
    static SDL_FRect canvas_target
        { 0, 0, (float) g_sprite_w, (float) g_sprite_h };

    for (Coordinates const& c : coords)
    {
        sprite_target.x = c.x_sprite;
        sprite_target.y = c.y_sprite;

        canvas_target.x = Grid::x_canvas(c.x_tile);
        canvas_target.y = Grid::y_canvas(c.y_tile);

        SDL_RenderTexture(win.ren, entity_sprites,
                          &sprite_target, &canvas_target);
    }
}

void vw::Entities::brace_for_input()
{
    /*
     * Here, we've got to satisfy any preconditions for being able to handle
     * input (such as making sure the action list is empty).
     *
     * This is necessary for back-to-back input cycles that lack an update cycle
     * inbetween (e.g., due to input events so rapid that they skip the update
     * cycle threshold).
     */
    actions.clear();
}

void vw::Entities::player_move(Direction const d)
{
    int x = player_x();
    int y = player_y();

    switch (d)
    {
    case Direction::N:
        --y;
        break;
    case Direction::S:
        ++y;
        break;
    case Direction::E:
        ++x;
        break;
    case Direction::W:
        --x;
        break;
    case Direction::NE:
        --y;
        ++x;
        break;
    case Direction::NW:
        --y;
        --x;
        break;
    case Direction::SE:
        ++y;
        ++x;
        break;
    case Direction::SW:
        ++y;
        --x;
        break;
    }

    Action move = Move{x, y};
    assert(actions.empty());
    actions.push_back(move);
}

void vw::Entities::player_wait()
{
    Action wait = Wait{};
    assert(actions.empty());
    actions.push_back(wait);
}

int vw::Entities::player_x() const
{
    return coords.at(0).x_tile;
}

int vw::Entities::player_y() const
{
    return coords.at(0).y_tile;
}

bool vw::Entities::at(int const x_tile, int const y_tile) const
{
    return std::ranges::find_if
    (
        coords,
        [&](Coordinates const& c)
        {
            return c.x_tile == x_tile && c.y_tile == y_tile;
        }
    ) != std::ranges::cend(coords);
}

bool vw::Entities::invalid_action(Action const& a) const
{
    // Validate all actions that may be invalid in some circumstances.
    bool valid = true;

    auto const validator = Reactions
    {
        [&](Move const& m)
        {
            valid &= Grid::in_bounds(m.x, m.y);
            valid &= !at(m.x, m.y);
            // TODO When we all walls/unpassable tiles, check those here too.
        },
        /*
         * All possible action variants need a lambda entry here, so actions
         * that are always valid can just have an empty lambda below this line:
         */
        [&](Wait const&) {},
    };
    std::visit(validator, a);

    return not valid;
}

void vw::Entities::play_turn()
{
    // Do all queued actions.
    size_t i = 0;
    static auto const reaction = Reactions
    {
        [&](Move const& m)
        {
            Coordinates& c = coords.at(i);
            c.x_tile = m.x;
            c.y_tile = m.y;
        },
        [&](Wait const&)
        {
            // Stuff like staunch wounds/put out fire can be here in the future.
        },
    };

    for (; i < actions.size(); ++i)
    {
        std::visit(reaction, actions.at(i));
    }

    // After actions have been executed, reset the list of actions.
    actions.clear();

    // TODO Reap the dead from some "recently killed" list here?
}
