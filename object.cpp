#include "object.hpp"
#include "event.hpp"

/*
 * @param EventKind, unsigned int, std::string
 */
Event(
    EventKind k,
    std::string t,
    unsigned int a
) {
    kind = k;
    amount = a;
    type = t;
}

/*
 * Getters
 */
EventKind getKind() { return kind; }
std::string event::getType() { return type; }
unsigned int event::getAmount() { return amount; }

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
void object::getEvent(Event e) {
    for (Component c : components) {
        c.processEvent(e, this);
    }
}

void object::processEvent(Event e) {
    // Only deal with an Event if and how its type is relevant to
    // this component::Component's kind.
    if (kind == "physics") {
        // accept damage
        if (e.getKind() == "take_damage") {
            subject->hp() -= e.getAmount();
        }
    }
//  else if (kind == "...") {
//
//  }
}
