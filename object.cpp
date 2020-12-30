#include <iostream>

#include "object.hpp"

Event::Event(EventKind k, std::string m, unsigned int a) {
    kind = k;
    modifier = m;
    amount = a;
}

/*
 * Getters
 */
EventKind Event::getKind() { return kind; }
std::string Event::getModifier() { return modifier; }
unsigned int Event::getAmount() { return amount; }

/*
 * Default constructor
 *
 * @param none
 */
Object::Object() {
    std::string id = "";
    disp_char = '#';
    passable = false;

    x_ = 0;
    y_ = 0;

    // Maybe later, we cam move hp_ to a proper Component object that represents
    // the physical parameters of an Object subject to physics.
    // That Component, which the Object owns, returns the hp after an HP
    // modification, and, if the Object gets a 0 back from the Component, the
    // object determines it has run out of HP and dies. :v
    hp_ = 0;
}

/*
 * @param char, std::string
 */
Object::Object(char c, std::string name) {
    id = name;
    disp_char = c;
}

/*
 * Getter and setter for disp_char
 */
char & Object::ch() { return disp_char; }

/*
 * Getter and setter for disp_char
 */
std::string & Object::name() { return id; }

/*
 * X and Y coordinate getters and setters
 */
unsigned int & Object::x() { return x_; }
unsigned int & Object::y() { return y_; }

/*
 * HP getter/setter
 */
unsigned int & Object::hp() { return hp_; }

/*
 * Handle an Event accordingly if the Event's type is relevant to any of this
 * Object's Components
 */
void Object::handleEvent(Event e) {
    // Only deal with an Event if and how its type is relevant to
    // this Component's kind.
    switch (e.getKind()) {
        case take_damage:
            // if Object's components set contains the physics Component,
            //     this.hp_ -= e.getAmount()
            break;
        case heal:
            break;
        case move_to:
            break;
        default: // Undefined event kind; notify and crash.
            std::cerr << "Object received undefined EventKind: \"" << e.getKind() << "\"" << std::endl;
            exit(EXIT_FAILURE);
    }
}
