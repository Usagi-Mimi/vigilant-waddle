#ifndef COMPONENT_CPP
#define COMPONENT_CPP
#include <string>

#include "event.hpp"
#include "object.hpp"
#include "component.hpp"

/*
 * @param std::string
 */
component::Component(std::string k) {
    kind = k;
}

/*
 * Deal with an event::Event based on kind
 * 
 * @param event::Event, object::Object
 */
void component::dealWith(event::Event e, object::Object subject) {
    // Only deal with an event::Event if and how its type is relevant to
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
