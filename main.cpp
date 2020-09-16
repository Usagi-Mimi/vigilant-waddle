// std
#include <tuple>
#include <iostream>
#include <fstream>
#include <iostream>
#include <stdlib.h>

// foreign
#include <libtcod.hpp>

// local
#include "object.hpp"
#include "map.hpp"

/*
 *  Renders a test Map::Map, returning the Map::Map and the player std::tuple<int, int>
 *
 *  @param none
 *  @return std::tuple<Map::Map, std::tuple<int, int>>
 */
std::tuple<Map::Map, std::tuple<int, int>> renderTestMap() {
    Object::Object * wall  = new Object::Object('#', "rock wall");
    Object::Object * floor = new Object::Object('.', "wall floor");
    Object::Object * stick = new Object::Object('/', "stick");
    int player_x = 3;
    int player_y = 4;

    Map::Map newMap;

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

/*
 *  Load a Map from a file-enclose and return its maximum x, y dimensions as
 *  a std::tuple, skipping empty lines or lines beginning with "//"
 *
 *  @param std::string
 */
std::tuple<int, int> getMaxDimensions(std::string filename) {
    int max_x = 0;
    int max_y = 0;
    std::ifstream ifs { filename };

    while (ifs) {
        std::string currentLine;

        std::getline(ifs, currentLine);
        if (currentLine.length() > 0 && currentLine.substr(0, 2) != "//") {
            if (max_x < currentLine.length()) {
                max_x = currentLine.length();
            }
            max_y++;
        }
    }

    return std::make_tuple(max_x, max_y);
}

/*
 *  Load a Map from a file, render it to a Map::Map, and return that Map::Map and the
 *  player std::tuple<int, int>
 *
 *  If a player was found, the last player's coordinates are returned. If no
 *  player was found, (0,0) are returned as the player coordinates.
 *
 *  @param std::string
 */
std::tuple<Map::Map, std::tuple<int, int>> loadMap(std::string filename) {
    Object::Object * wall  = new Object::Object('#', "rock wall");
    Object::Object * floor = new Object::Object('.', "wall floor");
    Object::Object * stick = new Object::Object('/', "stick");
    int player_x = 0;
    int player_y = 0;

    Map::Map newMap;

    std::ifstream ifs { filename };

    if (!ifs) {
        std::cerr << "loadMap.cpp: Could not open \"" << filename << "\"!" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto max_dimensions = getMaxDimensions(filename);
    int max_x = std::get<0>(max_dimensions);
    int max_y = std::get<1>(max_dimensions);

    if (max_x <= 0 || max_y <= 0) {
        std::cerr << "loadMap.cpp: Invalid Map \"" << filename << "\"!" << std::endl;
        exit(EXIT_FAILURE);
    }

    TCODConsole::initRoot(max_x, max_y, "vigilant-waddle", false);

    int curr_x = 0;
    int curr_y = 0;

    while (ifs) {
        std::string currentLine;

        std::getline(ifs, currentLine);
        // Skip lines that are empty or begin with "//"
        if (currentLine.substr(0, 2) == "//") {
            continue;
        }
        for (auto iter = currentLine.cbegin(); iter != currentLine.cend(); ++iter) {
            switch (*iter) {
                case '#': // wall
                    newMap.addObjectAt(wall, curr_x, curr_y);
                    break;
                case '.': // floor
                    newMap.addObjectAt(floor, curr_x, curr_y);
                    break;
                case '@': // player
                    TCODConsole::root->putChar(curr_x, curr_y, '@');
                    player_x = curr_x;
                    player_y = curr_y;
                    break;
                case '/': // stick
                    newMap.addObjectAt(stick, curr_x, curr_y);
                    break;
                default:
                    break;
            }
            curr_x++;
        }
        curr_x = 0; // reset curr_x for next row
        if (currentLine.length() > 0) {
            curr_y++;
        }
    }

    return {
        newMap,
        std::make_tuple(player_x, player_y)
    };
}

int main() {
    Map::Map newMap;
    int player_x, player_y;

    //auto ret = renderTestMap();
    auto ret = loadMap("./Maps/test.txt");

    newMap = std::get<0>(ret);
    player_x = std::get<0>(std::get<1>(ret));
    player_y = std::get<1>(std::get<1>(ret));

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
