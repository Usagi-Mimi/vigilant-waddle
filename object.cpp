#include <vector>
#include <string>

#include "object.hpp"
#include "component.hpp"
#include "event.hpp"

/*
*  Default constructor 
*
*  @param none
*/
object::Object() {
    std::string id = "";
    disp_char = '#';
    passable = false;

    x_ = 0;
    y_ = 0;
    
    hp_ = 0;
}
/*
*  @param char, std::string
*/
object::Object(char c, std::string name) {
    id = name;
    disp_char = c;
}
/*
*  Getter and setter for disp_char
*/
char & object::ch() { return disp_char; }
/*
*  Getter and setter for disp_char
*/
std::string & object::name() { return id; }
/*
*  X and Y coordinate getters and setters
*/
int & object::x() { return x_; }
int & object::y() { return y_; }

/*
* hp getter/setter
*/
unsigned int & object::hp() { return hp_; }

/*
* Receive an event::Event and have all possessed components deal with it as 
* they might
*/
void object::getEvent(event::Event e) {
    for (component::Component c : components) {
        c.dealWith(e, this);
    }
}
