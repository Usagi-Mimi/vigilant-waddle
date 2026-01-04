#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <cstddef>

class SDL_Window;
class SDL_Renderer;

namespace vw
{

/**
 * @brief Takes responsibility for the graphical window and renderer
 */
class GameWindow final
{
public:
    /**
     * @brief A phase in the game's basic finite state machine, determining what's
     * currently rendering and reacting to player input
     */
    enum class GameState
    {
        MainMenu,
        PlayStage,
        InGameMenu,
        Exit
    };

    GameWindow(const size_t sprite_w, const size_t sprite_h,
               const size_t width, const size_t height);
    ~GameWindow();

    GameWindow(GameWindow const&) = delete; // No copy construction
    GameWindow& operator=(GameWindow const&) = delete; // No copy assignment
    GameWindow(GameWindow&&) noexcept = delete; // No move construction
    GameWindow& operator=(GameWindow&&) noexcept = delete; // No move assignment

    const size_t sprite_w;
    const size_t sprite_h;
    const size_t width;
    const size_t height;
    SDL_Window* window = NULL;

    SDL_Renderer* render = NULL;

    enum GameState state = GameState::MainMenu;
    bool show_fps;
};

}

#endif
