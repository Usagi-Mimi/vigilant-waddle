#include "Menu.hpp"
#include "TextService.hpp"
#include "colors.hpp"

#include <SDL3/SDL_render.h>

vw::Menu::Menu(std::string title,
               std::initializer_list<Action> actions,
               GameWindow& win,
               TextService& text_service) :
               actions(actions),
               win(win),
               text_service(text_service),
               title(title)
{
    std::vector<std::string_view> menu_strings =
    {
        title,
        "", // Separator
    };
    for (Action const& a : this->actions)
    {
        menu_strings.push_back(a.text);
    }

    auto [menu_strings_width, menu_strings_height] =
        text_service.string_dimensions(menu_strings,
                                       TextService::Font::Ui);
    float const font_size = text_service.font_size(TextService::Font::Ui);
    int const outer_frame_thick = 7;
    int const text_pad = font_size;
    outer_frame.w = menu_strings_width + outer_frame_thick + text_pad * 2;
    outer_frame.h = menu_strings_height + outer_frame_thick + text_pad * 2;
    outer_frame.x = (g_canvas_w - outer_frame.w) / 2; // Centered width
    outer_frame.y = (g_canvas_h - outer_frame.h) / 2; // Centered height

    inner_frame.w = outer_frame.w - outer_frame_thick * 2;
    inner_frame.h = outer_frame.h - outer_frame_thick * 2;
    inner_frame.x = outer_frame.x + outer_frame_thick;
    inner_frame.y = outer_frame.y + outer_frame_thick;

    title_x = inner_frame.x + text_pad;
    title_y = inner_frame.y + text_pad;

    int y_spacing = title_y + font_size * 2; // Below title and separator

    for (Action& a : this->actions)
    {
        a.x = title_x;
        a.y = y_spacing;
        y_spacing += font_size;
    }

    // Pre-select the first menu action
    action_index = 0;
}

void vw::Menu::process_input(SDL_Event const* e)
{
    if (e->type != SDL_EVENT_KEY_DOWN)
    {
        return;
    }

    switch (e->key.scancode)
    {
        // Selector controls
        case SDL_SCANCODE_RETURN:
            win.state = actions[action_index].transitions_to;
            break;
        case SDL_SCANCODE_W:    // WASD
        case SDL_SCANCODE_UP:   // Arrow
        case SDL_SCANCODE_KP_8: // Numpad
            action_index -= 1;
            break;
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
        case SDL_SCANCODE_KP_2:
            action_index += 1;
            break;
        default:
            break;
    }
    action_index %= actions.size(); // Keep selector in item bounds.
    // Keybinds (bypassing the selector)
    for (size_t i = 0; i < actions.size(); ++i)
    {
        if (e->key.scancode == actions[i].keybind)
        {
            win.state = actions[i].transitions_to;
        }
    }
}

void vw::Menu::render(void)
{
    // Render outer frame
    SDL_Color const& borders_and_text = g_ui_light;
    SDL_SetRenderDrawColor(win.ren,
                           borders_and_text.r,
                           borders_and_text.g,
                           borders_and_text.b,
                           borders_and_text.a);
    SDL_RenderFillRect(win.ren, &outer_frame);

    // Render inner frame
    SDL_Color const& inner_background = g_ui_dark;
    SDL_SetRenderDrawColor(win.ren,
                           inner_background.r,
                           inner_background.g,
                           inner_background.b,
                           inner_background.a);
    SDL_RenderFillRect(win.ren, &inner_frame);

    // Render title and all action items
    text_service.render_string(title, 0, title_x, title_y, borders_and_text);

    for (Action const& a : actions)
    {
        text_service.render_string(a.text, 0, a.x, a.y, borders_and_text);
        text_service.highlight_keybind_indicator(a.text,
                                                 a.keybind_char_index,
                                                 a.x, a.y,
                                                 actions[action_index] == a,
                                                 g_ui_indicator_hover,
                                                 g_ui_indicator);
    }

    // Render selection indicator
    float const font_size = text_service.font_size(TextService::Font::Ui);
    text_service.render_string(selector, 1,
                               actions[action_index].x - font_size / 1.5,
                               actions[action_index].y,
                               borders_and_text);
}
