#include <iostream>
#include <stdlib.h>
#include <fstream>

#include <libtcod.hpp>

#include "map.hpp"
#include "object.hpp"

bool Map::isObjectAt(std::tuple<int, int> coords) {
    if (stage.count(coords) > 0) {
        return true;
    }
    else {
        return false;
    }
}

/*
 * @param integer x and y coordinates
 */
bool Map::isObjectAt(int x, int y) {
    std::tuple<int, int> coords = std::tuple<int, int>(x, y);
    if (stage.count(coords) > 0) {
        return true;
    }
    else {
        return false;
    }
}

/*
 * Return object at specified location
 *
 * @param std::tuple<int, int> tuple
 * @return Object
 */
Object Map::fetchObjectAt(std::tuple<int, int> coords) {
    return stage[coords];
}

/*
 * @param integer x and y coordinates
 */
Object Map::fetchObjectAt(int x, int y) {
    std::tuple<int, int> coords = std::tuple<int, int>(x, y);
    return stage[coords];
}

/*
 * Add object to specific location. Fails if coordinates are occupied.
 *
 * @param std::tuple<int, int> tuple, Object
 * @return boolean, true if addition successful
 */
bool Map::addObjectAt(Object * obj, std::tuple<int, int> coords) {
    if (!isObjectAt(coords)) {
        stage[coords] = *obj;
        obj->x() = std::get<1>(coords);
        obj->y() = std::get<0>(coords);
        objCount++;
        return true;
    }
    else {
        return false;
    }
}

/*
 * @param integer x and y coordinates
 */
bool Map::addObjectAt(Object * obj, int x, int y) {
    std::tuple<int, int> coords = std::tuple<int, int>(x, y);
    if (!isObjectAt(coords)) {
        stage[coords] = *obj;
        obj->x() = x;
        obj->y() = y;
        objCount++;
        return true;
    }
    else {
        return false;
    }
}

/*
 * Draws stage to console naively. Iterates through every Object located in Map and places it
 * in the console for display.
 *
 * @param none
 * @return void
 */
void Map::drawStage() {
    for (std::map<std::tuple<int, int>, Object>::iterator mapIt = stage.begin();
            mapIt != stage.end();
            mapIt++) {
        TCODConsole::root->putChar(std::get<0>(mapIt->first),
                                    std::get<1>(mapIt->first),
                                    mapIt->second.ch());
    }
}

/*
 *  Load a Map from a file-enclose and return its maximum x, y dimensions as
 *  a std::tuple, skipping empty lines or lines beginning with "//"
 *
 *  @param std::string
 */
std::tuple<int, int> Map::getMaxDimensions(std::string filename) {
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
 *  Load a text map from a file, render it to a Map object, and return that the
 *  player coordinates in a tuple
 *
 *  If a player was found, the last player's coordinates are returned. If no
 *  player was found, (0,0) are returned as the player coordinates.
 * 
 * NOTE: In the future, this should probably be terrain-only, not allowing any
 * non-terrain entities, because they overlap and you can't indicate the kind
 * of floor that the '@' you typed is standing on, for example.
 *
 *  @param std::string
 */
std::tuple<int, int> Map::loadMap(std::string filename) {
    Object * wall  = new Object('#', "rock wall");
    Object * floor = new Object('.', "wall floor");
    Object * stick = new Object('/', "stick");
    int player_x = 0;
    int player_y = 0;

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
                    this->addObjectAt(wall, curr_x, curr_y);
                    break;
                case '.': // floor
                    this->addObjectAt(floor, curr_x, curr_y);
                    break;
                case '@': // player
                    TCODConsole::root->putChar(curr_x, curr_y, '@');
                    player_x = curr_x;
                    player_y = curr_y;
                    break;
                case '/': // stick
                    this->addObjectAt(stick, curr_x, curr_y);
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

    return std::make_tuple(player_x, player_y);
}
