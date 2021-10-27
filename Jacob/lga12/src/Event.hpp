#pragma once
#include "EventType.hpp"

class Event{

	private:

		double at;
		double service;
		EventType type;
	
	public:
	 	double get_at() {
			return at;
		}


	
		EventType get_type() {
			return type;
		}
	
		double get_service() {
			return service;
		}
	
		Event(double a, double s, EventType t) {
			at = a;
			service = s;
			type = t;
		}

		Event(){
			type = EventType::NULL_JOB;
		}

		bool operator <(const Event & eventObj) const{
			return at < eventObj.at;
		}
};
