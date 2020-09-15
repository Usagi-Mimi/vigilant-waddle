#include "object.hpp"
#include "map.hpp"
bool map::isObjectAt(map::Coordinates coords) {
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
bool map::isObjectAt(int x, int y) {
    map::Coordinates coords = std::tuple<int, int>(x, y);
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
 *  @param map::Coordinates tuple
 *  @return object::Object
 */
object::Object map::fetchObject(map::Coordinates coords) {
    return stage[coords];
}

/*
 *  @param integer x and y coordinates
 */
object::Object map::fetchObjectAt(int x, int y) {
    map::Coordinates coords = std::tuple<int, int>(x, y);
    return stage[coords];
}

/*
 *  Add object to specific location. Fails if coordinates are occupied.
 *
 *  @param map::Coordinates tuple, object::Object
 *  @return boolean, true if addition successful
 */
bool map::addObjectAt(object::Object * obj, map::Coordinates coords) {
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
bool map::addObjectAt(object::Object * obj, int x, int y) {
    map::Coordinates coords = std::tuple<int, int>(x, y);
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
 *  Draws stage to console naively. Iterates through every object::Object located in Map and places it
 *  in the console for display.
 *  
 *  @param none
 *  @return void
 */
void map::drawStage() {
    for (std::map<map::Coordinates, object::Object>::iterator mapIt = stage.begin();
            mapIt != stage.end(); 
            mapIt++) {
        TCODConsole::root->putChar(std::get<0>(mapIt->first),
                                    std::get<1>(mapIt->first), 
                                    mapIt->second.ch());
    }
}
};
