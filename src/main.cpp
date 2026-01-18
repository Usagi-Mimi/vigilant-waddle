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

namespace
{

// Update game state at 60 Hz (1000 ms / 60 updates = 16 ms per update).
Uint64 constexpr update_ms = 16;
Uint64 curr_ms = 0;
Uint64 elapsed_ms = 0;
Uint64 prev_ms = 0;
Uint64 lag_ms = 0;
Uint64 frames_drawn = 0;
Uint64 since_last_fps_report_ms = 0;
float fps = 0;

}

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

    while (window.state != vw::GameWindow::GameState::Exit)
    {
        // Keep time.
        curr_ms = SDL_GetTicks();
        elapsed_ms = curr_ms - prev_ms;
        prev_ms = curr_ms;
        lag_ms += elapsed_ms;
        since_last_fps_report_ms += elapsed_ms;

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
        /*
         * The reason to have this as a separate step (and to give everything
         * dedicated update functions instead of having it update its state
         * immediately on arrival of input) is to be able to use the loop below.
         * This "lag loop" makes sure that game state updates deterministically
         * in a given interval of wall time regardless of how quickly or slowly
         * the hardware can run the game. It decouples state updates from the
         * rendering framerate.
         *
         * This only really matters for physics calculations or animation, where
         * not having it causes obvious glitches (probably like Doc Mitchell's
         * head spin when playing at over 60 FPS: https://youtu.be/ITOrKb5HP6s).
         */
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

            // Run as many updates as needed to catch up to the update target.
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
            // Calculate FPS. (Ticks are in milliseconds, hence the 1000.)
            if (since_last_fps_report_ms >= 1000)
            {
                // Report frames drawn in the last second and reset.
                fps = frames_drawn;
                frames_drawn = 0;
                since_last_fps_report_ms = 0;
            }
            else
            {
                ++frames_drawn;
            }
            font_manager.render_fps(fps, vw::g_ui_dark);
        }

        SDL_RenderPresent(window.render); // Show current frame!
    }

    return EXIT_SUCCESS;
}
