#pragma once

#include "Object.hpp"

#include <map>


/*
 * Container for the "stage", where the player spends most of their time fighting
 * stuff and looting stuff
 */
class Map {
    private:
        std::map<std::tuple<unsigned int, unsigned int>, Object> stage;
        unsigned int objCount;

        std::tuple<unsigned int, unsigned int> getMaxDimensions(std::string filename);
    public:
        bool isObjectAt(std::tuple<unsigned int, unsigned int> coords);
        bool isObjectAt(unsigned int x, unsigned int y);
        Object fetchObjectAt(std::tuple<unsigned int, unsigned int> coords);
        Object fetchObjectAt(unsigned int x, unsigned int y);
        bool addObjectAt(Object * obj, std::tuple<unsigned int, unsigned int> coords);
        bool addObjectAt(Object * obj, unsigned int x, unsigned int y);
        void drawStage();
        std::tuple<unsigned int, unsigned int> loadMap(std::string filename);
};
