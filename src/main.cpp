#include "GameWindow.hpp"
#include "TextService.hpp"
#include "Menu.hpp"
#include "PlayStage.hpp"
#include "colors.hpp"

#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_render.h>

#include <cstdlib>
#include <mutex> // `once_flag`

namespace
{

vw::GameWindow win
{
    "Vigilant Waddle",
    "../img/icon.png"
};

vw::TextService text_service
{
    win,
    "../fonts/Not-Jam/Old_Style/NotJamOldStyle14.ttf"
};

vw::Menu::Action a_new_game
{
    0, 0,
    vw::GameState::PlayStage,
    "New game", SDL_SCANCODE_N, 0
};
vw::Menu::Action a_quit
{
    0, 0,
    vw::GameState::Exit,
    "Quit", SDL_SCANCODE_Q, 0
};
vw::Menu main_menu
{
    "Main Menu",
    { a_new_game, a_quit },
    win,
    text_service
};

vw::PlayStage stage
{
    win,
    "../img/floor_spritesheet_16x24.png",
    "../img/entity_spritesheet_16x24.png"
};

// Update game state at 60 Hz (1000 ms / 60 updates = 16 ms per update).
constexpr Uint64 update_ms = 16;
Uint64 curr_ms = 0;
Uint64 elapsed_ms = 0;
Uint64 prev_ms = 0;
Uint64 lag_ms = 0;
Uint64 frames_drawn = 0;
Uint64 since_last_fps_report_ms = 0;
float fps = 0;

std::once_flag game_started;

}

int main()
{
    SDL_Event e;
    SDL_zero(e);

    while (win.state != vw::GameState::Exit)
    {
        // Keep time.
        curr_ms = SDL_GetTicks();
        elapsed_ms = curr_ms - prev_ms;
        prev_ms = curr_ms;
        lag_ms += elapsed_ms;
        since_last_fps_report_ms += elapsed_ms;

        /*************************
         * 1: Process user input *
         *************************/
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT) // Quit request (e.g. 'x' button)
            {
                win.state = vw::GameState::Exit;
            }

            switch (win.state)
            {
                case vw::GameState::MainMenu:
                    main_menu.process_input(&e);
                    break;
                case vw::GameState::PlayStage:
                    /*
                     * Change "New game" to "Continue" after starting a game.
                     * TODO Refactor this though cuz it's kind of silly.
                     */
                    std::call_once
                    (
                        game_started,
                        [&]
                        {
                            vw::Menu::Action& play = main_menu.actions.front();
                            play.text = "Continue";
                            play.keybind = SDL_SCANCODE_C;
                        }
                    );

                    stage.process_input(&e);
                    break;
                case vw::GameState::InGameMenu:
                    main_menu.process_input(&e); // Re-use main menu
                    break;
                default:
                    break;
            }
        }

        /****************************************************
         * 2: Update the game's logic (execute AI, physics) *
         ****************************************************/
        /*
         * The reason to have this as a separate step (and to give everything
         * dedicated update functions instead of having stuff update its state
         * immediately on arrival of input) is to be able to use the loop below.
         * This "lag loop" makes sure that game state updates deterministically
         * in a given interval of wall time regardless of how quickly or slowly
         * the hardware can run the game. It decouples state updates from the
         * rendering framerate.
         *
         * This only really matters for physics calculations or animation, where
         * not having it causes obvious glitches (probably like Doc Mitchell's
         * head spin when playing at over 60 FPS: https://youtu.be/ITOrKb5HP6s).
         *
         * Also, note that this breaks the assumption that you'll always have an
         * "input, update, render" cycle. With sufficiently fast input events,
         * the lag loop might run for zero iterations, yielding an "input,
         * render, input" cycle.
         */
        while (lag_ms >= update_ms)
        {
            switch (win.state)
            {
                case vw::GameState::PlayStage:
                    stage.update();
                    break;
                default:
                    break;
            }

            // Run as many updates as needed to catch up to the update target.
            lag_ms -= update_ms;
        }

        /******************************
         * 3: Render the game objects *
         ******************************/
        win.prepare_frame();
        switch (win.state)
        {
            case vw::GameState::MainMenu:
                main_menu.render();
                break;
            case vw::GameState::PlayStage:
                stage.render();

                // Hint overlay
                text_service.render_string("Playing! Press ESC for menu.",
                                           0,
                                           10, 10,
                                           vw::g_ui_dark);
                break;
            case vw::GameState::InGameMenu:
                // Render main menu atop game world
                stage.render();
                main_menu.render();
                break;
            default:
                break;
        }

        if (win.show_fps)
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
            text_service.render_fps(fps, vw::g_ui_dark);
        }
        win.present_frame();
    }

    return EXIT_SUCCESS;
}
