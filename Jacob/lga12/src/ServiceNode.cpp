#include "ServiceNode.hpp"

   
ServiceNode::ServiceNode(){
    this->inService = false;
} 

void ServiceNode::switchService(bool b){
    this->inService = b;
}

void ServiceNode::placeEvent(Event event){
    this->e = event;
}

Event ServiceNode::get_event(){
    this->inService = true;
    return this->e;
}

bool ServiceNode::get_inService(){
    return this->inService;
}