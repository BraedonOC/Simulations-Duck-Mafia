#include <queue>
#include <list>
#include "Event.hpp"
#include "ServiceNode.hpp"

class SSQ{

	private:
		std::queue<Event> job_queue; 
		bool inService;	
		int num_servers;
	public:
		std::list<ServiceNode> nodes;
		SSQ(int num_servers);	
		void enque_e(Event e);	
		Event get_event();
		int get_l();
		int get_num_servers();
		bool is_in_service();
		void update_service(bool u);
		void pop_e();
};
