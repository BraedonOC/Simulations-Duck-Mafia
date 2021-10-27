#pragma once
#include "Event.hpp"

class ServiceNode {
    private:
        bool inService;
        Event e;
    public:
        ServiceNode();        
        void placeEvent(Event e);
        void switchService(bool b);
        Event get_event();
        bool get_inService();
        
};