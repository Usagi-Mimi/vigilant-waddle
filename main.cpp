// std
#include <map>
#include <tuple>
#include <iostream>
#include <fstream>
#include <iostream>
#include <stdlib.h>

// foreign
#include <libtcod.hpp>

// local
// #include "map_or_something.cpp"

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
         *  Getter and setter for disp_char
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
        bool isObjectAt(Coordinates coords) {
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
        bool isObjectAt(int x, int y) {
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
        Object fetchObject(Coordinates coords) {
            return stage[coords];
        }
        /*
         *  @param integer x and y coordinates
         */
        Object fetchObjectAt(int x, int y) {
            Coordinates coords = std::tuple<int, int>(x, y);
            return stage[coords];
        }
        /*
         *  Add object to specific location. Fails if coordinates are occupied.
         *
         *  @param Coordinates tuple, Object
         *  @return boolean, true if addition successful
         */
        bool addObjectAt(Object * obj, Coordinates coords) {
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
        bool addObjectAt(Object * obj, int x, int y) {
            Coordinates coords = std::tuple<int, int>(x, y);
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

/*
 *  Renders a test Map, returning the Map and the player Coordinates
 *  
 *  @param none
 *  @return std::tuple<Map, Coordinates>
 */
std::tuple<Map, Coordinates> renderTestMap() {
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

/*
 *  Load a map from a file-enclose and return its maximum x, y dimensions as 
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
 *  Load a map from a file, render it to a Map, and return that Map and the 
 *  player Coordinates
 * 
 *  If a player was found, the last player's coordinates are returned. If no
 *  player was found, (0,0) are returned as the player coordinates.
 *
 *  @param std::string
 */
std::tuple<Map, Coordinates> loadMap(std::string filename) {
    Object * wall  = new Object('#', "rock wall");
    Object * floor = new Object('.', "wall floor");
    Object * stick = new Object('/', "stick");
    int player_x = 0;
    int player_y = 0;
    
    Map newMap;
    
    std::ifstream ifs { filename };
    
    if (!ifs) {
        std::cerr << "loadMap.cpp: Could not open \"" << filename << "\"!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    auto max_dimensions = getMaxDimensions(filename);
    int max_x = std::get<0>(max_dimensions);
    int max_y = std::get<1>(max_dimensions);
    
    if (max_x <= 0 || max_y <= 0) {
        std::cerr << "loadMap.cpp: Invalid map \"" << filename << "\"!" << std::endl;
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
    Map newMap; 
    int player_x, player_y;
    
    //auto ret = renderTestMap();
    auto ret = loadMap("./maps/test.txt");
    
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
