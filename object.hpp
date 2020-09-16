#pragma once
#include <vector>
#include <string>
#include <set>

enum Component {
    physics, // can take damage, has hp, etc.
    sentient, // can be talked to, has pathfinding if not sessile, etc.
};

enum EventKind {
    take_damage,
    heal,
    move_to,
};

class Event {
    private:
        EventKind kind;
        std::string modifier;
        unsigned int amount;

    public:
        Event(EventKind k, std::string m, unsigned int a);
        EventKind getKind();
        std::string getModifier();
        unsigned int getAmount();
};

class Object {
    private:
        std::string id;
        char disp_char;
        bool passable;

        int x_;
        int y_;

        unsigned int hp_;

        std::set<Component> components;
    public:
        Object();
        Object(char c, std::string name);
        char & ch();
        std::string & name();
        int & x();
        int & y();
        unsigned int & hp();
        void handleEvent(Event e);
};
