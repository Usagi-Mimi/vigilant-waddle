#include "GameWindow.hpp"
#include "colors.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <cassert>

SDL_Texture* vw::load_spritesheet(std::string_view const path,
                                  SDL_Renderer& ren)
{
    assert(!path.empty());
    SDL_Texture* spritesheet = nullptr;
    SDL_Surface* sprite_surface = IMG_Load(path.data());
    assert(sprite_surface);
    Uint32 key_color_pixel =
    SDL_MapRGB(SDL_GetPixelFormatDetails(sprite_surface->format),
               NULL,
               g_sprite_key_color.r,
               g_sprite_key_color.g,
               g_sprite_key_color.b);
    SDL_SetSurfaceColorKey(sprite_surface,
                           true,
                           key_color_pixel);
    spritesheet = SDL_CreateTextureFromSurface(&ren, sprite_surface);
    assert(spritesheet);

    SDL_DestroySurface(sprite_surface);
    return spritesheet;
}

vw::GameWindow::GameWindow(std::string_view name,
                           std::string_view icon_path) :
    name(name),
    icon_path(icon_path)
{
    // Initialize SDL.
    SDL_SetHint(SDL_HINT_APP_NAME, name.data());
    SDL_Init(SDL_INIT_VIDEO);

    /*
     * Create the game window.
     *
     * We grab the dimensions of the primary display, initially hide the window,
     * size it explicitly, and then fullsreen it. This ritual is necessary as of
     * 2026-03 because:
     *     - SDL3 doesn't always honor the window size requested. Randomly,
     *       it'll give us the native resolution instead of whatever resolution
     *       we request.
     *     - On both X11 and Wayland, SDL3 fails to find the primary monitor.
     *       Despite claims by the docs that `SDL_WINDOW_FULLSCREEN` always
     *       sends windows to the primary monitor, on X11 it seems to pick the
     *       monitor the cursor was on at launch time.
     */
    SDL_Rect primary_display_bounds {0, 0, 0, 0};
    bool const got_primary_display_bounds =
        SDL_GetDisplayBounds(SDL_GetPrimaryDisplay(), &primary_display_bounds);
    assert(got_primary_display_bounds);
    // Use the primary display's native resolution.
    w = primary_display_bounds.w;
    h = primary_display_bounds.h;
    SDL_CreateWindowAndRenderer(name.data(),
                                w, h,
                                SDL_WINDOW_HIDDEN,
                                &win, &ren);
    assert(win);
    assert(ren);
    SDL_SetWindowPosition(win,
                          primary_display_bounds.x, primary_display_bounds.y);
    SDL_SetWindowFullscreen(win, true);
    SDL_ShowWindow(win);

    // Set the window's icon.
    SDL_Surface* icon_surface = IMG_Load(icon_path.data());
    SDL_SetWindowIcon(win, icon_surface);
    SDL_DestroySurface(icon_surface);

    // Make the renderer display frames at the monitor's refresh rate.
    SDL_SetRenderVSync(ren, 1);

    /*
     * Scale the canvas integer-wise, as in: scale the canvas up by the smallest
     * whole integer such that the canvas fits within the window along the
     * window's smallest window dimension. For example, picturing the canvas as
     * already centered:
     * ```
     * +-Window----------------------------------------------------+
     * |                             .                             |
     * |                             .                             |
     * |                             . scale_h                     |
     * |                             .                             |
     * |                             .                             |
     * |                   +-Canvas------------+                   |
     * |                   |                   |                   |
     * |                   |                   |...................|
     * |                   |                   |      scale_w      |
     * |                   +-------------------+                   |
     * |                                                           |
     * |                                                           |
     * |                                                           |
     * |                                                           |
     * |                                                           |
     * +-----------------------------------------------------------+
     * ```
     * Eyeballing the diagram, say `scale_h` is smaller than `scale_w`. So we'll
     * scale the canvas by `scale_h`. As a real number, `scale_h` might be
     * something like 3.5; integer-wise, `scale_h` would then be 3. So we'll
     * scale the whole canvas (both its dimensions) by 3:
     * ```
     * +-Window----------------------------------------------------+
     * |                                                           |
     * |  +-Canvas---------------------------------------------+ l |
     * |  |                                                    | e |
     * |  |                                                    | t |
     * |  |                                                    | t |
     * |  |                                                    | e |
     * |  |                                                    | r |
     * |  |                                                    | b |
     * |  |                                                    | o |
     * |  |                                                    | x |
     * |  |                                                    | i |
     * |  |                                                    | n |
     * |  |                                                    | g |
     * |  +----------------------------------------------------+   |
     * |  l   e   t   t   e   r   b   o   x   i   n   g            |
     * +-----------------------------------------------------------+
     * ```
     * Since we're scaling integer-wise, we might have leftover space. After the
     * canvas is centered, the leftover space appears as letterboxing. This only
     * happens when the canvas dimensions aren't divisors of window dimensions.
     * (E.g., there would be no vertical or horizontal letterboxing with a
     * 1280x720 canvas and a 2560x1440 window since both canvas dimensions scale
     * up by 2 to the window dimensions perfectly.)
     *
     * We scale integer-wise with possible letterboxing (instead of real-wise,
     * which would never risk letterboxing) to preserve the fidelity of our
     * sprites' pixel graphics. Real-wise scaling would give the pixel graphics
     * "texel artifacting": https://youtu.be/d6tp43wZqps?t=89
     *
     * NOTE: This logic assumes a fullscreen window; the sizes of window border
     * or titlebar decorations or a taskbar aren't taken into account.
     */
    int scale_w = std::max(w / g_canvas_w, 1); // Never scale BELOW canvas size.
    int scale_h = std::max(h / g_canvas_h, 1);
    canvas_scale = std::min(scale_w, scale_h);
    canvas_scaled_w = g_canvas_w * canvas_scale;
    canvas_scaled_h = g_canvas_h * canvas_scale;

    // Since we're using pixel graphics, scale textures without blurriness.
    SDL_SetDefaultTextureScaleMode(ren, SDL_SCALEMODE_NEAREST);

    // Center the canvas to the window.
    canvas_screen_x = (w - canvas_scaled_w) / 2; // Centered width
    canvas_screen_y = (h - canvas_scaled_h) / 2; // Centered height

    // Create the canvas (aka "back buffer") texture.
    canvas = SDL_CreateTexture(ren,
                               SDL_PIXELFORMAT_RGBA32,
                               SDL_TEXTUREACCESS_TARGET,
                               g_canvas_w, g_canvas_h);
    assert(canvas);
}

vw::GameWindow::~GameWindow()
{
    SDL_DestroyRenderer(ren);

    SDL_DestroyWindow(win);

    SDL_Quit();
}

void vw::GameWindow::prepare_frame()
{
    // Clear the canvas in preparation for rendering a new frame's stuff.
    SDL_SetRenderDrawColor(ren,
                           vw::g_black.r,
                           vw::g_black.g,
                           vw::g_black.b,
                           vw::g_black.a);
    SDL_RenderClear(ren);

    // Set the canvas as the rendering target.
    SDL_SetRenderTarget(ren, canvas);

    /*
     * Fill the canvas with the padding color (just so that the padding around
     * the canvas has a color other than the letterboxing in the cases where
     * there actually is letterboxing).
     */
    SDL_SetRenderDrawColor(ren,
                           vw::g_padding.r,
                           vw::g_padding.g,
                           vw::g_padding.b,
                           vw::g_padding.a);
    SDL_RenderClear(ren);
}

void vw::GameWindow::present_frame()
{
    // Reset the rendering target to the window.
    SDL_SetRenderTarget(ren, NULL);

    // Scale (as able) and center the canvas before rendering it to the window.
    static SDL_FRect original_canvas
        { 0, 0, (float) g_canvas_w, (float) g_canvas_h };
    static SDL_FRect window_canvas
        {
            canvas_screen_x,
            canvas_screen_y,
            (float) canvas_scaled_w,
            (float) canvas_scaled_h
        };

    SDL_RenderTexture(ren, canvas, &original_canvas, &window_canvas);

    SDL_RenderPresent(ren); // Show current frame!
}
