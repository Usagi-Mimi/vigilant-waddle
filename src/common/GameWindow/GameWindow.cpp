#include "GameWindow.hpp"

#include <SDL3/SDL.h>

#include <cassert>

vw::GameWindow::
GameWindow(const size_t sprite_w, const size_t sprite_h,
           const size_t width, const size_t height) :
           sprite_w(sprite_w), sprite_h(sprite_h),
           width(width), height(height),
           state(GameState::MainMenu), // Start at the main menu.
           show_fps(false)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL init: %s\n", SDL_GetError());
        assert(false);
    }

    window = SDL_CreateWindow("Vigilant Waddle", width, height, 0);
    assert(window);

    render = SDL_CreateRenderer(window, NULL);
    assert(render);
    SDL_SetRenderVSync(render, 1);
}

vw::GameWindow::~GameWindow()
{
    SDL_DestroyRenderer(render);

    SDL_DestroyWindow(window);

    SDL_Quit();
}
