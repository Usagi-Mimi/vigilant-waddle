#include <libtcod.hpp>
#include <map>
#include <tuple>
#include <iostream>

class Object {
    public:
        char disp_char = '#';
};

typedef std::tuple<int, int> Coordinates;

class Map {
    private:
        std::map<Coordinates, Object> stage;
        int objCount;
    public:
        bool checkObject(Coordinates coords) {
            if (stage.count(coords) > 0) {
                return true;   
            }
            else {
                return false;
            }
        }

        Object fetchObject(Coordinates coords){
                return stage[coords];
        }

        bool addObject(std::tuple<int, int> coords, Object obj) {
            if (!checkObject(coords)){
                stage[coords] = obj;
                objCount++;
                return true;
            } else {
                return false;
            }
        }

        void drawStage() {
            for (std::map<Coordinates, Object>::iterator mapIt = stage.begin();
                 mapIt != stage.end(); 
                 mapIt++) {
                TCODConsole::root->putChar(std::get<1>(mapIt->first),std::get<0>(mapIt->first), mapIt->second.disp_char);
            }
        }
};

int main() {
    int player_x = 40;
    int player_y = 25;
    
    Object rock;
    Map newMap;

    newMap.addObject(std::tuple<int, int>(1,1), rock);
    newMap.addObject(std::tuple<int, int>(1,2), rock);
    newMap.addObject(std::tuple<int, int>(1,3), rock);
    newMap.addObject(std::tuple<int, int>(1,4), rock);
    newMap.addObject(std::tuple<int, int>(1,5), rock);
    newMap.addObject(std::tuple<int, int>(1,6), rock);
    newMap.addObject(std::tuple<int, int>(1,7), rock);
    newMap.addObject(std::tuple<int, int>(2,1), rock);
    newMap.addObject(std::tuple<int, int>(3,1), rock);
    newMap.addObject(std::tuple<int, int>(4,1), rock);
    newMap.addObject(std::tuple<int, int>(5,1), rock);
    newMap.addObject(std::tuple<int, int>(2,7), rock);
    newMap.addObject(std::tuple<int, int>(3,7), rock);
    newMap.addObject(std::tuple<int, int>(4,7), rock);
    newMap.addObject(std::tuple<int, int>(5,7), rock);
    newMap.addObject(std::tuple<int, int>(6,1), rock);
    newMap.addObject(std::tuple<int, int>(6,2), rock);
    newMap.addObject(std::tuple<int, int>(6,3), rock);
    newMap.addObject(std::tuple<int, int>(6,4), rock);
    newMap.addObject(std::tuple<int, int>(6,5), rock);
    newMap.addObject(std::tuple<int, int>(6,6), rock);
    newMap.addObject(std::tuple<int, int>(6,7), rock);

    newMap.drawStage();

    TCODConsole::initRoot(80, 50, "vigilant-waddle", false);

    while ( !TCODConsole::isWindowClosed() ) {   
        // Handle key input
        TCOD_key_t key;
        
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
        switch(key.vk) {
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
