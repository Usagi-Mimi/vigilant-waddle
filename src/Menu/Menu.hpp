#ifndef MENU_HPP
#define MENU_HPP

#include "GameWindow.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>

#include <string>
#include <vector>

namespace vw
{

class FontWoes;

/**
 * @brief Handles state, interactivity logic, and rendering of UI menus
 */
class Menu final
{
public:
    /**
     * @brief A selectable action item inside a menu
     */
    struct MenuAction
    {
        float x;
        float y;
        GameWindow::GameState transitions_to;
        std::string text;
        SDL_Scancode keybind;
        size_t keybind_char_index;

        bool operator==(MenuAction& other)
        {
            return text == other.text;
        }
    };

    Menu(std::string title,
         std::vector<MenuAction> actions,
         GameWindow& window,
         FontWoes& font_manager);

    void process_input(SDL_Event* e);

    void render(void);

    std::vector<MenuAction> actions;

private:
    GameWindow& window;
    FontWoes& font_manager;

    SDL_FRect outer_frame;
    SDL_FRect inner_frame;

    std::string title;
    int title_x;
    int title_y;

    size_t item_padding_for_selector;
    size_t action_index;
};

}

#endif
