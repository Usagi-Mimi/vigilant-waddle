#ifndef MENU_HPP
#define MENU_HPP

#include "GameWindow.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>

#include <string>
#include <vector>
#include <string_view>

namespace vw
{

class TextService;

/**
 * @brief A UI menu that the user can interact
 */
class Menu final
{
public:
    /**
     * @brief A selectable action item inside a menu
     */
    struct Action
    {
        int x = 0;
        int y = 0;
        GameState transitions_to;
        std::string text;
        SDL_Scancode keybind;
        size_t keybind_char_index = 0;

        bool operator==(Action const& other) const
        {
            return text == other.text;
        }
    };

    Menu(std::string title,
         std::initializer_list<Action> actions,
         GameWindow& win,
         TextService& text_service);

    void process_input(SDL_Event const* e);

    void render(void);

    std::vector<Action> actions;

private:
    GameWindow& win;
    TextService& text_service;

    SDL_FRect outer_frame { 0, 0, 0, 0 };
    SDL_FRect inner_frame { 0, 0, 0, 0 };

    std::string title;
    int title_x = 0;
    int title_y = 0;

    std::string_view selector = ">";
    int action_index = 0;
};

}

#endif
