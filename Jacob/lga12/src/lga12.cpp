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

typedef std::vector<std::string> CommandLineStringArgs;
const double TAU = 1000.0;
const double LAMBDA_1 = 2.0;
const double TIME_TO_COMPLETE = 10.0;

// Create a list of SSQ's
std::list<SSQ> ssq_list;

/*
*	Grabbing a random double between 0 an 1
*/

std::random_device rd;
std::mt19937 gen(rd());
double nextDouble(){
	std::uniform_real_distribution<> dis(0.0, 1.0);
	return dis(gen);
}

int num_busy_servers(){
	int b = 0;
	for(std::list<SSQ>::iterator it = ssq_list.begin(); it != ssq_list.end(); ++it){
		if(it->is_in_service()){
			b++;
		}
	}
	return b;
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
	
	std::list<Event> events;
	Clock clock = Clock();

	for (int i = 0; i < num_servers; i++){
		SSQ ssq = SSQ();
		ssq_list.push_back(ssq);
	}

	// We need a map or some way to track which events belong to which queues
	std::map<Event, SSQ*> event_map;

	// Our welford object which will track the percentage of servers occupied
	Welford wel;

	int newJobCount = 0;
	int jobsServiced = 0;
	int busy_servers = 0;

	Event job1 = Event(exponential(LAMBDA_1), uniform(0,TIME_TO_COMPLETE), EventType::JOB_ARRIVAL);
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
			double nextArrival = clock.get_time() + exponential(LAMBDA_1);
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
			SSQ *ssq;
			int min_jobs = INT16_MAX;

			// Finding optimal queue to place job into
			for(int i = 0; i < ssq_list.size(); i++){

				auto it = std::next(ssq_list.begin(), i);				
				if (it->get_l() < min_jobs) {
					ssq = &(*it);
					min_jobs = it->get_l();
				}			
			}
			
			ssq->enque_e(event);
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
			SSQ *ssq = event_map[event];								
			ssq->update_service(false);

			busy_servers--;
			bool b = (busy_servers >= num_servers/2);
			wel.update((double) b, clock.get_time());
			break;
		}
		default:
			break;
		}
		
		// Now we iterate over all the queues and get those with jobs
		// being serviced to complete those jobs
		
		
		for(int i = 0; i < ssq_list.size(); i++){

			SSQ* ssq  = &(*std::next(ssq_list.begin(), i));
			if ( !ssq->is_in_service() && ssq->get_l() > 0) {
				Event e = ssq->get_event();
				Event complete = Event(
										clock.get_time() + e.get_service(), 
										0.0, 
										EventType::JOB_COMPLETION);

				events.push_back(complete);
				ssq->update_service(true);
				
				jobsServiced++;

				// We need to map this event to the proper queue
				// so we dequeue correctly
				event_map.insert(std::make_pair(complete, ssq));

				busy_servers++;
				bool b = (busy_servers >= num_servers/2);
				wel.update((double) b, clock.get_time());
			}
		}
	}
	printf ("Final time: %f\nNew jobs: %d\nJobs serviced: %d\nWelford mean: %f\nWelford variance: %f\n", 
				 clock.get_time(), 
				 newJobCount, 
				 jobsServiced,
				 wel.get_x_bar(),
				 wel.get_v_bar());
	return 0;
}

	
	
