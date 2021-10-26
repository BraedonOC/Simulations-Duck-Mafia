#include <queue>
#include <list>
#include "Event.hpp"

class SSQ{

	private:
		std::queue<Event> job_queue; 
		bool inService;

	public:
		SSQ();	
		void enque_e(Event e);	
		Event get_event();
		int get_l();
		bool is_in_service();
		void update_service(bool u);
		void pop_e();
};
