#include "PlayStage.hpp"
#include "GameWindow.hpp"
#include "FontWoes.hpp"
#include "colors.hpp"

#include <SDL3_image/SDL_image.h>

#include <cassert>
#include <cmath>

vw::PlayStage::PlayStage(GameWindow& window,
                         FontWoes& font_manager) :
                         window(window),
                         font_manager(font_manager)
{
    // Initialize point coordinates at the center of the window.
    float const mid_x = round(window.height / window.sprite_h / 2);
    float const mid_y = round(window.height / window.sprite_h / 2);
    player_pos.x = mid_x * window.sprite_h;
    player_pos.y = mid_y * window.sprite_h;

    going_up = false;
    going_down = false;
    going_left = false;
    going_right = false;

    // Load player sprite (assuming CWD is `build/`).
    player_sprite = NULL;
    SDL_Surface* sprite_surface = IMG_Load("../img/player.png");
    assert(sprite_surface);
    Uint32 key_color_pixel =
    SDL_MapRGB(SDL_GetPixelFormatDetails(sprite_surface->format),
               NULL,
               g_sprite_key_color.r,
               g_sprite_key_color.g,
               g_sprite_key_color.b);
    SDL_SetSurfaceColorKey(sprite_surface,
                           true,
                           key_color_pixel);
    player_sprite = SDL_CreateTextureFromSurface(window.render, sprite_surface);
    assert(player_sprite);
    SDL_DestroySurface(sprite_surface);

    // Load floor tile sprite.
    floor_sprite = NULL;
    sprite_surface = IMG_Load("../img/floor.png");
    assert(sprite_surface);
    floor_sprite = SDL_CreateTextureFromSurface(window.render, sprite_surface);
    assert(floor_sprite);
    SDL_DestroySurface(sprite_surface);
}

vw::PlayStage::~PlayStage()
{
    SDL_DestroyTexture(player_sprite);
    SDL_DestroyTexture(floor_sprite);
}

void vw::PlayStage::process_input(SDL_Event* e)
{
    if (e->type != SDL_EVENT_KEY_DOWN)
    {
        return;
    }

    switch (e->key.scancode)
    {
        case SDL_SCANCODE_ESCAPE: // Menu
            window.state = GameWindow::GameState::InGameMenu;
            break;
        case SDL_SCANCODE_F:
            window.show_fps = !window.show_fps;
            break;
        case SDL_SCANCODE_W:    // WASD
        case SDL_SCANCODE_UP:   // Arrow
        case SDL_SCANCODE_KP_8: // Numpad
            going_up = true;
            break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_KP_4:
            going_left = true;
            break;
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
        case SDL_SCANCODE_KP_2:
            going_down = true;
            break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_KP_6:
            going_right = true;
            break;
            // Numpad diagonals
        case SDL_SCANCODE_KP_7:
            going_up = true;
            going_left = true;
            break;
        case SDL_SCANCODE_KP_9:
            going_up = true;
            going_right = true;
            break;
        case SDL_SCANCODE_KP_1:
            going_down = true;
            going_left = true;
            break;
        case SDL_SCANCODE_KP_3:
            going_down = true;
            going_right = true;
            break;
        default:
            break;
    }
}

void vw::PlayStage::try_moving_to(float const x, float const y)
{
    // Do nothing if map bounds are exceeded.
    if (x < 0 ||
        y < 0 ||
        x > window.width - window.sprite_w ||
        y > window.height - window.sprite_h)
    {
        return;
    }

    player_pos.x = x;
    player_pos.y = y;
}

void vw::PlayStage::update(void)
{
    float d_x = 0;
    float d_y = 0;

    if (going_up)
    {
        d_y -= window.sprite_h;
        going_up = false;
    }
    if (going_down)
    {
        d_y += window.sprite_h;
        going_down = false;
    }
    if (going_left)
    {
        d_x -= window.sprite_w;
        going_left = false;
    }
    if (going_right)
    {
        d_x += window.sprite_w;
        going_right = false;
    }

    try_moving_to(player_pos.x + d_x, player_pos.y + d_y);
}

void vw::PlayStage::render(void)
{
    // Render the current sprite frame.
    SDL_RenderTextureTiled(window.render, floor_sprite, NULL, 1.0, NULL);

    SDL_FRect dst =
    {
        player_pos.x,
        player_pos.y,
        (float) window.sprite_w,
        (float) window.sprite_h
    };
    SDL_RenderTexture(window.render, player_sprite, NULL, &dst);

    font_manager.render_string("Playing! Press ESC for menu.",
                               0,
                               10, 10,
                               g_ui_dark);
}
