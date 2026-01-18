#include "FontWoes.hpp"
#include "GameWindow.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <cassert>
#include <string>
#include <format>

vw::FontWoes::FontWoes(GameWindow& window) : window(window)
{
    // Initialize the font library and load our font(s).
    TTF_Init();

    char const* font_path = "../fonts/Not-Jam/Old_Style/NotJamOldStyle14.ttf";
    ui_font = TTF_OpenFont(font_path, 28);
    assert(ui_font);
    ui_font_small = TTF_OpenFont(font_path, 14);
    assert(ui_font_small);
}

vw::FontWoes::~FontWoes()
{
    // Unload our font(s) and tear down the font library.
    TTF_CloseFont(ui_font);
    TTF_CloseFont(ui_font_small);

    TTF_Quit();
}

TTF_Font* vw::FontWoes::resolve_font(Font const font) const
{
    TTF_Font* f = NULL;
    switch (font)
    {
        case Font::Ui:
            f = ui_font;
            break;
        case Font::UiSmall:
            f = ui_font_small;
            break;
    }
    assert(f);
    return f;
}

void vw::FontWoes::render_string(std::string_view string,
                                 size_t const len,
                                 float const x,
                                 float const y,
                                 SDL_Color const& color) const
{
    /*
     * NOTE: Takes `0` for lengths of null-terminated strings.
     *
     * Also, mind the four rendering modes:
     *     - Solid
     *         - Very fast but low quality
     *         - 8-bit RGB
     *         - Best for fast-changing text, like a pinball score
     *     - Shaded
     *         - Slower than solid but high quality
     *         - 8-bit RGB
     *         - AA'd
     *     - Blended
     *         - Slow but very high quality
     *         - 32-bit RGBA
     *         - AA'd
     *     - LCD
     *         - Slow but very high quality
     *         - Sub-pixel rendering
     *         - 32-bit RGBA
     */
    SDL_Surface* surface = TTF_RenderText_Solid(ui_font,
                                                string.data(),
                                                len,
                                                color);
    assert(surface);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(window.render, surface);
    assert(texture);
    SDL_FRect dst = {x, y, (float) surface->w, (float) surface->h};

    SDL_RenderTexture(window.render, texture, NULL, &dst);

    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

void vw::FontWoes::highlight_keybind_indicator(std::string_view string,
                                               size_t const index,
                                               float const x,
                                               float const y,
                                               bool const hovered,
                                               SDL_Color const& hovered_color,
                                               SDL_Color const& other_color)
{
    char const* char_offset = string.data() + index;
    SDL_Color const* color = hovered ? &hovered_color : &other_color;
    SDL_Surface* surface = TTF_RenderText_Solid(ui_font,
                                                char_offset,
                                                1,
                                                *color);
    assert(surface);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(window.render, surface);
    assert(texture);

    int distance_to_keybind_char = 0;
    if (index != 0) // When `0`, `TTF_GetStringSize()` will go to end of string.
    {
        TTF_GetStringSize(ui_font,
                          string.data(),
                          index,
                          &distance_to_keybind_char,
                          NULL);
    }
    SDL_FRect dst = {x + distance_to_keybind_char, y,
        (float) surface->w, (float) surface->h};

        SDL_RenderTexture(window.render, texture, NULL, &dst);

        SDL_DestroySurface(surface);
        SDL_DestroyTexture(texture);
}

float vw::FontWoes::font_size(Font font) const
{
    TTF_Font* f = resolve_font(font);
    return TTF_GetFontSize(f);
}

std::tuple<size_t, size_t>
vw::FontWoes::font_dimensions(std::string_view string, Font font)
{
    TTF_Font* f = resolve_font(font);

    int width = 0;
    int height = 0;
    TTF_GetStringSize(f, string.data(), 0, &width, &height);
    return { static_cast<size_t>(width), static_cast<size_t>(height) };
}

std::tuple<size_t, size_t>
vw::FontWoes::font_dimensions(std::vector<std::string_view>& strings, Font font)
{
    TTF_Font* f = resolve_font(font);

    int width_final = 0;
    int width_temp = 0;
    int height_final = 0;
    int height_temp = 0;

    for (std::string_view string : strings)
    {
        TTF_GetStringSize(f, string.data(), 0, &width_temp, &height_temp);
        if (width_temp > width_final) { width_final = width_temp; }
        height_final += height_temp;
    }

    return { static_cast<size_t>(width_final),
             static_cast<size_t>(height_final) };
}

void vw::FontWoes::render_fps(float const fps, SDL_Color const& color) const
{
    static std::string fps_display;

    fps_display.clear();
    fps_display = std::format("FPS: {}", fps);

    render_string(fps_display.c_str(),
                  0,
                  0, window.height - TTF_GetFontHeight(ui_font),
                  color);
}
