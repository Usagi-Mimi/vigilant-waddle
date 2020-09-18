// std
#include <iostream>
#include <tuple>

// foreign
#include <libtcod.hpp>

// local
#include "object.hpp"
#include "map.hpp"

int main() {
    Map newMap;
    unsigned int player_x, player_y;

    auto ret = newMap.loadMap("./maps/test.txt");

    player_x = std::get<0>(ret);
    player_y = std::get<1>(ret);

    while ( !TCODConsole::isWindowClosed() ) {
        // Handle key input
        TCOD_key_t key;

        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
        switch (key.vk) {
            case TCODK_UP:
            case TCODK_KP8:
                player_y--;
                break;
            case TCODK_DOWN:
            case TCODK_KP2:
                player_y++;
                break;
            case TCODK_LEFT:
            case TCODK_KP4:
                player_x--;
                break;
            case TCODK_RIGHT:
            case TCODK_KP6:
                player_x++;
                break;
            case TCODK_KP7:
                player_x--;
                player_y--;
                break;
            case TCODK_KP9:
                player_x++;
                player_y--;
                break;
            case TCODK_KP1:
                player_x--;
                player_y++;
                break;
            case TCODK_KP3:
                player_x++;
                player_y++;
                break;
            default: break;
        }

        // Clear and rebuild scene >w<
        TCODConsole::root->clear();
        newMap.drawStage();
        TCODConsole::root->putChar(player_x, player_y, '@');
        // Render scene to window @w@
        TCODConsole::flush();
    }

    return 0;
}
