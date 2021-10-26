#include "Clock.hpp"

double Clock::get_time() {
	return time;
}

void Clock::set_time(double t) {
	time = t;
}

Clock::Clock() {
	time = 0.0;
}
