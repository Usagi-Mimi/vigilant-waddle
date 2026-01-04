#ifndef COLORS_HPP
#define COLORS_HPP

#include <SDL3/SDL_pixels.h>

namespace vw
{

/*
 * The `inline` makes these globals C++17 inline variables, able to be shared
 * across all translation units that include this header (no ODR violations)
 * without having to use `extern` and define them elsewhere!
 */
inline constexpr SDL_Color g_black              = {  0,   0,   0, 255};
inline constexpr SDL_Color g_white              = {255, 255, 255, 255};
inline constexpr SDL_Color g_ui_light           = g_white;
inline constexpr SDL_Color g_ui_dark            = {  0, 120,  80, 255};
inline constexpr SDL_Color g_ui_indicator_hover = {217, 217,  70, 255};
inline constexpr SDL_Color g_ui_indicator       = {170, 150, 100, 255};
inline constexpr SDL_Color g_sprite_key_color   = {255,   0, 255, 255};

}

#endif
