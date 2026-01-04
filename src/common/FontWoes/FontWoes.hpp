#ifndef FONTWOES_HPP
#define FONTWOES_HPP

#include <cstddef>

#include <tuple>
#include <vector>
#include <string_view>

struct SDL_Color;
struct TTF_Font;

namespace vw
{

class GameWindow;

/**
 * @brief Manages all font state and rendering, acting as the buffer between the
 * font library and the rest of the program
 */
class FontWoes final
{
public:
    /**
     * @brief Opaque aliases that can be used to specify font types without
     * knowing the actual names of the loaded fonts
     */
    enum class Font
    {
        Ui,
        UiSmall,
    };

    FontWoes(GameWindow& window);
    ~FontWoes();

    /**
     * @brief Renders the given string at the given screen coordinates
     * @param len 0 for null-terminated strings
     */
    void render_string(std::string_view string,
                       size_t const len,
                       float const x,
                       float const y,
                       SDL_Color const& color) const;

    /**
     * @brief Highlights the keybind-indicating character in the given string
     * @param index The index of the indicator character in the given string
     * @param x The x-coordinate of the entire string
     * @param y The y-coordinate of the entire string
     * @param hovered Whether the character should be colored with the brighter
     * "hovered" indicator color
     * @param hovered_color The color to use for the keybind indicator of the
     * string being hovered over
     * @param other_color The color to use for all the other indicators
     */
    void highlight_keybind_indicator(std::string_view string,
                                     size_t const index,
                                     float const x,
                                     float const y,
                                     bool const hovered,
                                     SDL_Color const& hovered_color,
                                     SDL_Color const& other_color);

    /**
     * @return The size of the given font
     */
    float font_size(Font font) const;

    /**
     * @brief Measures the dimensions that the given string would have when
     * rendered using the given font
     *
     * WARNING: The string is expected to be null-terminated.
     *
     * @param font The font to use for measurement
     * @return Width and height, in pixels
     */
    std::tuple<size_t, size_t>
    font_dimensions(std::string_view string, Font font);

    /**
     * @brief Measures the dimensions that the given collection of strings
     * (presumably laid out vertically, one under another) would have when
     * rendered using the given font
     *
     * This calculates the bounding rectangle for strings in a menu.
     *
     * WARNING: Each string is expected to be null-terminated.
     *
     * @param strings A collection of vertically-listed, null-terminated strings
     * @return Width and height, in pixels (the width is the width of the
     * longest string in the collection
     */
    std::tuple<size_t, size_t>
    font_dimensions(std::vector<std::string_view>& strings, Font font);

    /**
     * @brief Prints the given FPS value at a corner of the screen in the given
     * color
     */
    void show_fps(float const fps, SDL_Color const& color) const;

    TTF_Font* ui_font;
    TTF_Font* ui_font_small;

private:
    /**
     * @return The actual loaded font that the given alias corresponds to
     */
    TTF_Font* resolve_font(Font const font) const;

    GameWindow& window;
};

}

#endif
