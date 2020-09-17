// std
#include <iostream>
#include <tuple>

// foreign
#include <libtcod.hpp>

// local
#include "object.hpp"
#include "map.hpp"

/*
 *  Renders a test Map, returning the Map and the player coordinates in a tuple
 *
 *  @param none
 *  @return std::tuple<Map, std::tuple<int, int>>
 */
std::tuple<Map, std::tuple<int, int>> renderTestMap() {
    Object * wall  = new Object('#', "rock wall");
    Object * floor = new Object('.', "wall floor");
    Object * stick = new Object('/', "stick");
    int player_x = 3;
    int player_y = 4;

    Map newMap;

    newMap.addObjectAt(stick, 5, 3);

    std::cout << "Stick X: " << stick->x() << " Y: " << stick->y() << std::endl;
    newMap.addObjectAt(wall, 1, 1);
    newMap.addObjectAt(wall, 1, 2);
    newMap.addObjectAt(wall, 1, 3);
    newMap.addObjectAt(wall, 1, 4);
    newMap.addObjectAt(wall, std::tuple<int, int>(1,5));
    newMap.addObjectAt(wall, std::tuple<int, int>(1,6));
    newMap.addObjectAt(wall, std::tuple<int, int>(1,7));
    newMap.addObjectAt(wall, std::tuple<int, int>(2,1));
    newMap.addObjectAt(wall, std::tuple<int, int>(3,1));
    newMap.addObjectAt(wall, std::tuple<int, int>(4,1));
    newMap.addObjectAt(wall, std::tuple<int, int>(5,1));
    newMap.addObjectAt(wall, std::tuple<int, int>(2,7));
    newMap.addObjectAt(wall, std::tuple<int, int>(3,7));
    newMap.addObjectAt(wall, std::tuple<int, int>(4,7));
    newMap.addObjectAt(wall, std::tuple<int, int>(5,7));
    newMap.addObjectAt(wall, std::tuple<int, int>(6,1));
    newMap.addObjectAt(wall, std::tuple<int, int>(6,2));
    newMap.addObjectAt(wall, std::tuple<int, int>(6,3));
    newMap.addObjectAt(wall, std::tuple<int, int>(6,5));
    newMap.addObjectAt(wall, std::tuple<int, int>(6,6));
    newMap.addObjectAt(wall, std::tuple<int, int>(6,7));
    newMap.addObjectAt(wall, std::tuple<int, int>(7,3));
    newMap.addObjectAt(wall, std::tuple<int, int>(7,5));
    newMap.addObjectAt(wall, std::tuple<int, int>(8,3));
    newMap.addObjectAt(wall, std::tuple<int, int>(8,5));
    newMap.addObjectAt(wall, std::tuple<int, int>(9,3));
    newMap.addObjectAt(wall, std::tuple<int, int>(10,3));
    newMap.addObjectAt(wall, std::tuple<int, int>(11,3));
    newMap.addObjectAt(wall, std::tuple<int, int>(9,5));
    newMap.addObjectAt(wall, std::tuple<int, int>(10,5));
    newMap.addObjectAt(wall, std::tuple<int, int>(11,5));

    TCODConsole::initRoot(80, 50, "vigilant-waddle", false);

    return {
        newMap,
        std::make_tuple(player_x, player_y)
    };
}

int main() {
    Map newMap;
    int player_x, player_y;

    //auto ret = renderTestMap();
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
