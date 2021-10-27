#include <random>
#include <list>
#include <math.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "SSQ.hpp"
#include "Clock.hpp"
#include "Event.hpp"
#include "Welford.hpp"
#include "ServiceNode.hpp"

typedef std::vector<std::string> CommandLineStringArgs;
const double TAU = 1000.0;
const double R_BAR = 2.0;
const double TIME_TO_COMPLETE = 100.0;

/*
*	Grabbing a random double between 0 an 1
*/

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);
double nextDouble(){
	return dis(gen);
}

/*
* Get next exponential random
*/

double exponential(double lambda) {
	return log(1-nextDouble())*(-lambda);
}

/*
* Get next uniform random
*/
double uniform(int a, int b) {
	return a + nextDouble()*(b-a);
}


int main(int argc, char *argv[]) {

	CommandLineStringArgs cmdlineStringArgs(&argv[0], &argv[0 + argc]);
	int num_servers = std::stoi(cmdlineStringArgs[1]);
	
	// Our welford object which will track the percentage of servers occupied
	// another Welford can be used to generate simulation statistics
	Welford sim_wel;
	
	for (int i = 0; i < 100; i++){
		std::list<Event> events;
		Clock clock = Clock();
		Welford wel;
		
		SSQ ssq = SSQ(num_servers);

		// We need a map or some way to track which events belong to which queues
		std::map<Event, ServiceNode*> event_map;		

		int newJobCount = 0;
		int jobsServiced = 0;
		int busy_servers = 0;

		Event job1 = Event(exponential(R_BAR), uniform(0,TIME_TO_COMPLETE), EventType::JOB_ARRIVAL);
		events.push_back(job1);
		
		while( events.size() > 0 ) {
			
			events.sort();
			Event event = events.front();
			events.pop_front();
			clock.set_time(event.get_at()); // PROCESS EVENT
			
			switch( event.get_type() ) {
			case JOB_ARRIVAL:
			{			
				newJobCount++;
				// Schedule the next job arrival
				double nextArrival = clock.get_time() + exponential(R_BAR);
				if (nextArrival < TAU) {
					// SCHEDULE NEW EVENT
					Event newEvent = Event(
											nextArrival, 
											uniform(0, TIME_TO_COMPLETE),
											EventType::JOB_ARRIVAL);
					events.push_back(newEvent);
				}
				
			}
			case FEEDBACK_ARRIVAL:
			{	
				ssq.enque_e(event);
				break;
			}
			case JOB_COMPLETION:
			{
				
				if ( uniform(0,1) < 0.25) {
					double feedbackArrival = clock.get_time() + exponential(1.0);
					double lastService = event.get_service();
					Event feedback = Event(
											feedbackArrival, 
											lastService/2.0, EventType::FEEDBACK_ARRIVAL);
					events.push_back(feedback);
				}
				
				
				ServiceNode *node = event_map[event];				
				node->switchService(false);						
						
				busy_servers--;
				bool b =  busy_servers >= num_servers/2;
				wel.update((double) b, clock.get_time());
				
				break;
			}
			default:
				break;
			}
			
			// Now we iterate over all the service nodes and get those with jobs
			// being serviced to complete those jobs
		
			for(int i = 0; i < ssq.nodes.size(); i++){
				ServiceNode* node = &(*std::next(ssq.nodes.begin(),i));
				
				if ( !node->get_inService() && ssq.get_l() > 0) {
					Event e = ssq.get_event();
					node->switchService(true);
					node->placeEvent(e);
					Event complete = Event(
											clock.get_time() + e.get_service(), 
											0.0, 
											EventType::JOB_COMPLETION);

					events.push_back(complete);
					
					jobsServiced++;

					// We need to map this event to the proper node
					// so we dequeue correctly
					event_map.insert(std::make_pair(complete, node));

					busy_servers++;
					bool b =  busy_servers >= num_servers /2;
					wel.update((double) b, clock.get_time());
				}
			}
		}

		// this comment section can be used to get the results of a single sim
		// printf ("Final time: %f\nNew jobs: %d\nJobs serviced: %d\nWelford mean: %f\nWelford variance: %f\n", 
		// 			clock.get_time(), 
		// 			newJobCount, 
		// 			jobsServiced,
		// 			wel.get_x_bar(),
		// 			wel.get_v_bar());
		sim_wel.simple_update(wel.get_x_bar());
		
	}
	
	printf ("%f\n%f\n", 
				sim_wel.get_x_bar(),
				sim_wel.get_v_bar());
	return 0;
}

	
	
