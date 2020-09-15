#pragma once
#include <string>

#include "event.hpp"
#include "object.hpp"

class Component {
    private:
        std::string kind;
    public:
        Component(std::string k);
        
        void dealWith(event::Event e, object::Object subject);
};
