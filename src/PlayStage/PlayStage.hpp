#ifndef PLAYSTAGE_HPP
#define PLAYSTAGE_HPP

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>

struct SDL_Texture;

namespace vw
{

class GameWindow;
class FontWoes;

class PlayStage final
{
public:
    PlayStage(GameWindow& window,
              FontWoes& font_manager);
    ~PlayStage();

    void process_input(SDL_Event* e);

    void update(void);

    void render(void);

private:
    void try_moving_to(float const x, float const y);

    GameWindow& window;
    FontWoes& font_manager;
    SDL_Texture* player_sprite;
    SDL_Texture* floor_sprite;
    SDL_FPoint player_pos;
    bool going_up;
    bool going_down;
    bool going_left;
    bool going_right;
};

}

#endif
