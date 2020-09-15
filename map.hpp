#pragma once
typedef std::tuple<int, int> Coordinates;

class Map {
    private:
        std::map<Coordinates, Object> stage;
        int objCount;
    public:
        bool isObjectAt(Coordinates coords);
        bool isObjectAt(int x, int y);
        Object fetchObject(Coordinates coords);
        Object fetchObjectAt(int x, int y);
        bool addObjectAt(Object * obj, Coordinates coords);
        bool addObjectAt(Object * obj, int x, int y);
        void drawStage();
};
