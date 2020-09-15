#include <libtcod.hpp>
#include <map>
#include <tuple>
#include <iostream>

typedef std::tuple<int, int> Coordinates;

class Object {
    private:
        std::string id = "";
        char disp_char = '#';
        bool passable = false;

        int x_ = 0;
        int y_ = 0;
    public:
        /*
         *  Default constructor 
         *
         *  @param none
         */
        Object() {
            return;
        }
        /*
         *  @param char, std::string
         */
        Object(char c, std::string name) {
            id = name;
            disp_char = c;
        }
        /*
         *   Getter and setter for disp_char
         */
        char & ch() { return disp_char; }
        /*
         *  Getter and setter for disp_char
         */
        std::string & name() { return id; }
        /*
         *  X and Y coordinate getters and setters
         */
        int & x() { return x_; }
        int & y() { return y_; }
};

class Map {
    private:
        std::map<Coordinates, Object> stage;
        int objCount;
    public:
        /*   Return true if the specified location contains an object.
         *   
         *   @param Coordinates tuple
         *   @return boolean, true if occupied
         */
        bool objectAt(Coordinates coords) {
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
        bool objectAt(int x, int y) {
            Coordinates coords = std::tuple<int, int>(x, y);
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
         *  @param Coordinates tuple
         *  @return Object
         */
        Object fetchObject(Coordinates coords){
            return stage[coords];
        }
        /*
         *  @param integer x and y coordinates
         */
        Object fetchObject(int x, int y){
            Coordinates coords = std::tuple<int, int>(x, y);
            return stage[coords];
        }
        /*
         *  Add object to specific location. Fails if coordinates are occupied.
         *
         *  @param Coordinates tuple, Object
         *  @return boolean, true if addition successful
         */
        bool addObject(Coordinates coords, Object * obj) {
            if (!objectAt(coords)){
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
        bool addObject(int x, int y, Object * obj) {
            Coordinates coords = std::tuple<int, int>(x, y);
            if (!objectAt(coords)){
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
        void drawStage() {
            for (std::map<Coordinates, Object>::iterator mapIt = stage.begin();
                 mapIt != stage.end(); 
                 mapIt++) {
                TCODConsole::root->putChar(std::get<0>(mapIt->first),
                                           std::get<1>(mapIt->first), 
                                           mapIt->second.ch());
            }
        }
};

int main() {
    int player_x = 3;
    int player_y = 4;
    
    Object * rock  = new Object('#', "rock wall");
    Object * stick = new Object('/', "stick");
    
    Map newMap;
    
    newMap.addObject(5, 3, stick);

    std::cout << "Stick X: " << stick->x() << " Y: " << stick->y() << std::endl;
    newMap.addObject(1, 1, rock);
    newMap.addObject(1, 2, rock);
    newMap.addObject(1, 3, rock);
    newMap.addObject(1, 4, rock);
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
    newMap.addObject(std::tuple<int, int>(6,5), rock);
    newMap.addObject(std::tuple<int, int>(6,6), rock);
    newMap.addObject(std::tuple<int, int>(6,7), rock);
    newMap.addObject(std::tuple<int, int>(7,3), rock);
    newMap.addObject(std::tuple<int, int>(7,5), rock);
    newMap.addObject(std::tuple<int, int>(8,3), rock);
    newMap.addObject(std::tuple<int, int>(8,5), rock);
    newMap.addObject(std::tuple<int, int>(9,3), rock);
    newMap.addObject(std::tuple<int, int>(10,3), rock);
    newMap.addObject(std::tuple<int, int>(11,3), rock);
    newMap.addObject(std::tuple<int, int>(9,5), rock);
    newMap.addObject(std::tuple<int, int>(10,5), rock);
    newMap.addObject(std::tuple<int, int>(11,5), rock);

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
