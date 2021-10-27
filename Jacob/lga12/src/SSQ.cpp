#include "SSQ.hpp"

std::queue<Event> job_queue; 

SSQ::SSQ(int num_servers) {
	job_queue = std::queue<Event>();
	inService = false;
	this->num_servers = num_servers;
	for (int i = 0; i < num_servers; i++){
		ServiceNode node;
		nodes.push_back(node);
	}
}
	
void SSQ::enque_e(Event e) {
	job_queue.push(e);
}

Event SSQ::get_event() {
	Event e = job_queue.front();
	job_queue.pop();
	return e;
}

void SSQ::pop_e() {
	job_queue.pop();
}

/*
* l returns the l statistic
*/
int SSQ::get_l() {
	return job_queue.size();
}

bool SSQ::is_in_service() {
	return inService;
}

void SSQ::update_service(bool u) {
	inService = u;
}

int SSQ::get_num_servers(){
	return num_servers;
}