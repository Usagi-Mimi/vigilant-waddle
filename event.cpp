#include <string>

#include "event.hpp"

/*
 * @param std::string, unsigned int, std::string
 */
event::Event(
    std::string k,
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
std::string event::getKind() { return kind; }
std::string event::getType() { return type; }
unsigned int event::getAmount() { return amount; }
