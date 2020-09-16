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
 *  @param integer x and y coordinates
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
 *  Return object at specified location
 *
 *  @param std::tuple<int, int> tuple
 *  @return Object
 */
Object Map::fetchObject(std::tuple<int, int> coords) {
    return stage[coords];
}

/*
 *  @param integer x and y coordinates
 */
Object Map::fetchObjectAt(int x, int y) {
    std::tuple<int, int> coords = std::tuple<int, int>(x, y);
    return stage[coords];
}

/*
 *  Add object to specific location. Fails if coordinates are occupied.
 *
 *  @param std::tuple<int, int> tuple, Object
 *  @return boolean, true if addition successful
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
 *  @param integer x and y coordinates
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
 *  Draws stage to console naively. Iterates through every Object located in Map and places it
 *  in the console for display.
 *
 *  @param none
 *  @return void
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
