#include "PlayStage.hpp"
#include "GameWindow.hpp"

#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_events.h>

vw::PlayStage::PlayStage(GameWindow& win,
                         std::string_view const floor_spritesheet_path,
                         std::string_view const entity_spritesheet_path) :
    win(win),
    grid(win,
         rng,
         load_spritesheet(floor_spritesheet_path, *win.ren)),
    entities(win,
             load_spritesheet(entity_spritesheet_path, *win.ren),
             grid)
{
    // Seed the random number generator.
    std::random_device rd;
    rng.seed(rd());
}

void vw::PlayStage::process_input(SDL_Event const* e)
{
    if (e->type != SDL_EVENT_KEY_DOWN)
    {
        return;
    }

    entities.brace_for_input();
    switch (e->key.scancode)
    {
        case SDL_SCANCODE_ESCAPE: // Menu
            win.state = GameState::InGameMenu;
            break;
        case SDL_SCANCODE_F:
            win.show_fps = !win.show_fps;
            break;
        case SDL_SCANCODE_W:    // WASD
        case SDL_SCANCODE_UP:   // Arrow
        case SDL_SCANCODE_KP_8: // Numpad
            entities.player_move(Direction::N);
            break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_KP_4:
            entities.player_move(Direction::W);
            break;
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
        case SDL_SCANCODE_KP_2:
            entities.player_move(Direction::S);
            break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_KP_6:
            entities.player_move(Direction::E);
            break;
        case SDL_SCANCODE_KP_7:
            entities.player_move(Direction::NW);
            break;
        case SDL_SCANCODE_KP_9:
            entities.player_move(Direction::NE);
            break;
        case SDL_SCANCODE_KP_1:
            entities.player_move(Direction::SW);
            break;
        case SDL_SCANCODE_KP_3:
            entities.player_move(Direction::SE);
            break;
        case SDL_SCANCODE_KP_5:
            entities.player_wait();
            break;
        default:
            break;
    }
}

void vw::PlayStage::update(void)
{
    entities.update();
}

void vw::PlayStage::render(void)
{
    grid.render();
    entities.render();
}
