#pragma once
#include <map>

#include "object.hpp"

/*
 * Container for the "stage", where the player spends most of their time fighting
 * stuff and looting stuff
 */
class Map {
    private:
        std::map<std::tuple<int, int>, Object> stage;
        int objCount;
    public:
        bool isObjectAt(std::tuple<int, int> coords);
        bool isObjectAt(int x, int y);
        Object fetchObjectAt(std::tuple<int, int> coords);
        Object fetchObjectAt(int x, int y);
        bool addObjectAt(Object * obj, std::tuple<int, int> coords);
        bool addObjectAt(Object * obj, int x, int y);
        void drawStage();
};
