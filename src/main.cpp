// std
#include <iostream>
#include <tuple>

// foreign
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <libtcod.hpp>

// local
#include "Object.hpp"
#include "Map.hpp"

int main()
{
    auto console = tcod::Console{80, 25};  // Main console

    // Configure context
    auto params = TCOD_ContextParams{};
    params.console = console.get();
    params.window_title = "Vigilant Waddle";
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;

    // Assuming we're in `build/`
    auto tileset = tcod::load_tilesheet("../fonts/terminal8x12_gs_ro.png",
                                        {16, 16},
                                        tcod::CHARMAP_CP437);
    params.tileset = tileset.get();

    auto context = tcod::Context(params);

    // Load map
    Map newMap;
    int player_x, player_y;
    auto ret = newMap.loadMap("../maps/test.txt"); // Assuming we're in `build/`

    player_x = std::get<0>(ret);
    player_y = std::get<1>(ret);

    bool running = true;
    while (running)
    {
        console.clear();

        tcod::print(console, {player_x, player_y}, "@", std::nullopt, std::nullopt);
        context.present(console); // Update display

        SDL_Event event;
        SDL_zero(event);
        SDL_WaitEvent(nullptr); // Sleep until event becomes available
        while (SDL_PollEvent(&event))
        {
            // Convert pixel coordinates to tile coordinates
            context.convert_event_coordinates(event);

            switch (event.type)
            {
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.scancode)
                {
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_KP_8:
                    player_y--;
                    break;
                case SDL_SCANCODE_DOWN:
                case SDL_SCANCODE_KP_2:
                    player_y++;
                    break;
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_KP_4:
                    player_x--;
                    break;
                case SDL_SCANCODE_RIGHT:
                case SDL_SCANCODE_KP_6:
                    player_x++;
                    break;
                case SDL_SCANCODE_KP_7:
                    player_x--;
                    player_y--;
                    break;
                case SDL_SCANCODE_KP_9:
                    player_x++;
                    player_y--;
                    break;
                case SDL_SCANCODE_KP_1:
                    player_x--;
                    player_y++;
                    break;
                case SDL_SCANCODE_KP_3:
                    player_x++;
                    player_y++;
                    break;
                default:
                    break;
                }
                break;
            case SDL_EVENT_QUIT:
                running = false;
                break;
            default:
                break;
            }
        }
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}
