#pragma once
#include <vector>
#include <string>

#include "component.hpp"
#include "event.hpp"

class Object {
    private:
        std::string id;
        char disp_char;
        bool passable;

        int x_;
        int y_;
        
        unsigned int hp_;
        
        std::vector<component::Component> components;
    public:
        Object();
        Object(char c, std::string name);
        char & ch();
        std::string & name();
        int & x();
        int & y();
        unsigned int & hp();
        void getEvent(event::Event e);
};
