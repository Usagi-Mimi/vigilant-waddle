#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <string_view>

class SDL_Window;
class SDL_Renderer;
struct SDL_Texture;

namespace vw
{

/**
 * @brief Sprite dimensions
 * @{
 */
inline constexpr int g_sprite_w = 16;
inline constexpr int g_sprite_h = 24;
/**
 * @}
 */

/**
 * @brief The number of tiles in the on-screen grid
 * @{
 */
inline constexpr int g_rows = 25;
inline constexpr int g_cols = 80;
inline constexpr int g_tiles = g_cols * g_rows;
/**
 * @}
 */

/**
 * @brief The screen coordinates of the top-leftmost grid tile (or more
 * specifically, that tile's top-left corner)
 * @{
 */
inline constexpr float g_grid_vert_pad = 60;
inline constexpr float g_grid_start_x = 0;
inline constexpr float g_grid_start_y = 0 + g_grid_vert_pad;
/**
 * @}
 */

/**
 * @brief The screen coordinates of the bottom-rightmost grid tile (or more
 * specifically, that tile's top-left corner)
 * @{
 */
inline constexpr float g_grid_end_x = g_grid_start_x + g_sprite_w * g_cols;
inline constexpr float g_grid_end_y = g_grid_start_y + g_sprite_h * g_rows;
/**
 * @}
 */

/**
 * @brief Canvas texture dimensions and scale factor
 *
 * These are based on the size of our tile sprites and how many tiles we
 * want to fit on-screen. Ideally, the canvas aspect ratio matches that of
 * the window (otherwise, the canvas must always be letterboxed).
 * @{
 */
inline constexpr int g_canvas_w = g_sprite_w * g_cols;
inline constexpr int g_canvas_h = g_sprite_h * g_rows + g_grid_vert_pad * 2;
/**
 * @}
 */

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

/**
 * @return A texture loaded from the spritesheet at the given path using the
 * given rendering context
 *
 * WARNING: The caller is responsible for freeing the texture via
 * `SDL_DestroyTexture()`.
 */
SDL_Texture* load_spritesheet(std::string_view const path, SDL_Renderer& ren);

/**
 * @brief Manages the game window and renderer
 */
class GameWindow final
{
public:
    GameWindow(std::string_view name,
               std::string_view icon_path);
    ~GameWindow();

    /**
     * @brief Readies the renderer and canvas for rendering a new frame's stuff
     */
    void prepare_frame();

    /**
     * @brief Presents the currently-rendered frame to the window
     */
    void present_frame();

    SDL_Renderer* ren = nullptr;

    enum GameState state = GameState::MainMenu; // Start at the main menu.
    bool show_fps = false;

private:
    /**
     * @brief Window dimensions
     * @{
     */
    int w = 0;
    int h = 0;
    /**
     * @}
     */

    SDL_Window* win = nullptr;
    std::string_view const name;
    std::string_view const icon_path;

    /**
     * @brief This is the texture that everything in a frame is rendered on (and
     * then this texture is, itself, rendered to the window)
     *
     * Rendering to a "canvas" (or "back buffer") texture like this instead of
     * directly to the window has the advantage that we only have to worry about
     * scaling or post-processing our graphics once, at the end of the rendering
     * pipeline: we only have to scale/post-process this canvas texture before
     * we render it to the window. (As opposed to doing that with everything
     * individually just before it's rendered directly to the window.)
     */
    SDL_Texture* canvas = nullptr;

    /**
     * @brief Canvas screen coordinates (specifically, the coordinates of the
     * canvas's top-left corner on the screen post scaling and centering)
     * @{
     */
    float canvas_screen_x = 0;
    float canvas_screen_y = 0;
    /**
     * @}
     */

    /**
     * @brief Determines how much the canvas can be scaled up to fit the window
     * @{
     */
    int canvas_scale = 1;
    int canvas_scaled_w = g_canvas_w;
    int canvas_scaled_h = g_canvas_h;
    /**
     * @}
     */
};

}

#endif
