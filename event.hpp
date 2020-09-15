#pragma once
#include <string>

#include "event.hpp"

class Event {
    private:
        std::string kind;
        std::string type;
        unsigned int amount;
        
    public:
        Event(
            std::string k,
            std::string t,
            unsigned int a
        );
        std::string getKind();
        std::string getType();
        unsigned int getAmount();
};
