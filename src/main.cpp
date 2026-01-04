#include "GameWindow.hpp"
#include "FontWoes.hpp"
#include "Menu.hpp"
#include "PlayStage.hpp"
#include "colors.hpp"

#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_render.h>

#include <cstdlib>
#include <mutex>

#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 48
#define WINDOW_WIDTH 70 * SPRITE_WIDTH
#define WINDOW_HEIGHT 25 * SPRITE_HEIGHT

int main()
{
    vw::GameWindow window(SPRITE_WIDTH, SPRITE_HEIGHT,
                          WINDOW_WIDTH, WINDOW_HEIGHT);
    vw::FontWoes font_manager(window);

    vw::Menu::MenuAction a_new_game { 0, 0,
                                vw::GameWindow::GameState::PlayStage,
                                "New game", SDL_SCANCODE_N, 0 };
    vw::Menu::MenuAction a_quit     { 0, 0,
                                vw::GameWindow::GameState::Exit,
                                "Quit", SDL_SCANCODE_Q, 0 };
    vw::Menu main_menu
    ("Main Menu",
     {
         a_new_game,
         a_quit,
     },
     window,
     font_manager);

    vw::PlayStage stage(window, font_manager);

    std::once_flag game_started;
    SDL_Event e;
    SDL_zero(e);

    // Update game state at 60 FPS (`1000 ms / 60 FPS = 16 ms` per frame).
    Uint64 const update_ms = 16;
    Uint64 prev_ms = SDL_GetTicks();
    Uint64 lag_ms = 0;
    size_t frames = 0;
    float avg_fps = 0;

    while (window.state != vw::GameWindow::GameState::Exit)
    {
        // Update the elapsed time.
        Uint64 curr_ms = SDL_GetTicks();
        Uint64 elapsed_ms = curr_ms - prev_ms;
        prev_ms = curr_ms;
        lag_ms += elapsed_ms;

        /*************************
         * 1 Process user in put *
         *************************/
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT) // Quit request (e.g. 'x' button)
            {
                window.state = vw::GameWindow::GameState::Exit;
            }

            switch (window.state)
            {
                case vw::GameWindow::GameState::MainMenu:
                    main_menu.process_input(&e);
                    break;
                case vw::GameWindow::GameState::PlayStage:
                    // Change "New game" to "Continue" after starting a game.
                    std::call_once
                    (
                        game_started,
                        [&]
                        {
                            vw::Menu::MenuAction& play_action = main_menu.actions.front();
                            play_action.text = "Continue";
                            play_action.keybind = SDL_SCANCODE_C;
                        }
                    );
                    stage.process_input(&e);
                    break;
                case vw::GameWindow::GameState::InGameMenu:
                    main_menu.process_input(&e); // Re-use main menu
                    break;
                default:
                    break;
            }
        }

        /***************************************************
         * 2 Update the game's logic (execute AI, physics) *
         ***************************************************/
        while (lag_ms >= update_ms)
        {
            switch (window.state)
            {
                case vw::GameWindow::GameState::PlayStage:
                    stage.update();
                    break;
                default:
                    break;
            }

            lag_ms -= update_ms;
        }

        /*****************************
         * 3 Render the game objects *
         *****************************/
        SDL_SetRenderDrawColor(window.render,
                               vw::g_black.r,
                               vw::g_black.g,
                               vw::g_black.b,
                               vw::g_black.a);
        SDL_RenderClear(window.render);

        switch (window.state)
        {
            case vw::GameWindow::GameState::MainMenu:
                main_menu.render();
                break;
            case vw::GameWindow::GameState::PlayStage:
                stage.render();
                break;
            case vw::GameWindow::GameState::InGameMenu:
                // Render main menu atop game world
                stage.render();
                main_menu.render();
                break;
            default:
                break;
        }

        if (window.show_fps)
        {
            // Calculate FPS. (Ticks are in ms, hence division by 1000 for s.)
            ++frames;
            avg_fps = frames / (SDL_GetTicks() / 1000.f);
            font_manager.show_fps(avg_fps, vw::g_ui_dark);
        }

        SDL_RenderPresent(window.render); // Show current frame!
    }

    return EXIT_SUCCESS;
}
