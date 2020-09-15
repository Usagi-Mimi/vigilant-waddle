#include <libtcod.hpp>

int main() {
    int player_x = 40;
    int player_y = 25;
    
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
        TCODConsole::root->putChar(player_x, player_y, '@');
        // Render scene to window @w@
        TCODConsole::flush();
    }
    
    return 0;
}
