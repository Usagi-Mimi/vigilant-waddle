#include "TextService.hpp"
#include "GameWindow.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <cassert>
#include <string>
#include <format>

vw::TextService::TextService(GameWindow& win,
                             std::string_view const font_path) : win(win)
{
    assert(!font_path.empty());

    // Initialize the font library and load our font(s).
    TTF_Init();
    ui_font = TTF_OpenFont(font_path.data(), 14);
    assert(ui_font);
    ui_font_small = TTF_OpenFont(font_path.data(), 7);
    assert(ui_font_small);
}

vw::TextService::~TextService()
{
    // Unload our font(s) and tear down the font library.
    TTF_CloseFont(ui_font);
    TTF_CloseFont(ui_font_small);

    TTF_Quit();
}

TTF_Font* vw::TextService::resolve_font(Font const font) const
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

void vw::TextService::render_string(std::string_view string,
                                    int const len,
                                    float const x,
                                    float const y,
                                    SDL_Color const& color) const
{
    assert(!string.empty());
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
    SDL_Texture* texture = SDL_CreateTextureFromSurface(win.ren, surface);
    assert(texture);
    SDL_FRect dst = {x, y, (float) surface->w, (float) surface->h};

    SDL_RenderTexture(win.ren, texture, NULL, &dst);

    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

void vw::TextService::highlight_keybind_indicator(std::string_view string,
                                                  int const index,
                                                  float const x,
                                                  float const y,
                                                  bool const hovered,
                                                  SDL_Color const& hovered_color,
                                                  SDL_Color const& other_color)
{
    assert(!string.empty());
    assert(index >= 0);
    char const* char_offset = string.data() + index;
    SDL_Color const* color = hovered ? &hovered_color : &other_color;
    SDL_Surface* surface = TTF_RenderText_Solid(ui_font,
                                                char_offset,
                                                1,
                                                *color);
    assert(surface);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(win.ren, surface);
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

        SDL_RenderTexture(win.ren, texture, NULL, &dst);

        SDL_DestroySurface(surface);
        SDL_DestroyTexture(texture);
}

float vw::TextService::font_size(Font font) const
{
    TTF_Font* f = resolve_font(font);
    return TTF_GetFontSize(f);
}

std::pair<int, int>
vw::TextService::string_dimensions(std::string_view const string, Font font)
{
    assert(!string.empty());
    TTF_Font* f = resolve_font(font);

    int width = 0;
    int height = 0;
    TTF_GetStringSize(f, string.data(), 0, &width, &height);
    return { static_cast<int>(width), static_cast<int>(height) };
}

std::pair<int, int>
vw::TextService::string_dimensions(std::vector<std::string_view> const& strings,
                                   Font font)
{
    assert(!strings.empty());
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

    return { static_cast<int>(width_final),
             static_cast<int>(height_final) };
}

void vw::TextService::render_fps(float const fps, SDL_Color const& color) const
{
    static std::string fps_display;

    fps_display.clear();
    fps_display = std::format("FPS: {}", fps);

    render_string(fps_display,
                  0,
                  0, g_canvas_h - TTF_GetFontHeight(ui_font),
                  color);
}
