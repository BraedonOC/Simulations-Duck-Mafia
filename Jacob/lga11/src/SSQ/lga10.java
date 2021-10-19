package SSQ;

import java.util.*;


public class lga10 {
	
	private static Random r = new Random();
	
	/*
	 * Sorts a linked list of events
	 */
	private static LinkedList<Event> event_sort(LinkedList<Event> unsorted){
		Collections.sort(unsorted, new Comparator<Event>() {
			@Override
			public int compare(Event e1, Event e2) {
				return Double.compare(e1.get_at(), e2.get_at());
			}
		});
		return unsorted;
	}
	
	/*
	 * Get next exponential random
	 */

	private static double exponential(double lambda) {
		return Math.log(1-r.nextDouble())/(-lambda);
	}
	
	/*
	 * Get next uniform random
	 */
	private static double uniform(int a, int b) {
		return a + r.nextDouble()*(b-a);
	}
	
	public final static double TAU = 1000.0;
	
	public static void main(String[] args) {
		// Based on psuedo-code
		// INITIALIZE
		LinkedList<Event> events = new LinkedList<Event>();
		Clock clock = new Clock();
		SSQ ssq = new SSQ();
		
		boolean inService = false;
		
		int newJobCount = 0;
		int jobsServiced = 0;
		
		
		
		Event job1 = new Event(exponential(2.0), uniform(0,2), EventType.JOB_ARRIVAL);
		events.add(job1);
		
		while( events.size() > 0 ) {
			event_sort(events);
			Event event = events.removeFirst();
			clock.set_time(event.get_at()); // PROCESS EVENT
			
			switch( event.get_type() ) {
			case JOB_ARRIVAL:
				newJobCount++;
				// Schedule the next job arrival
				double nextArrival = clock.get_time() + exponential(2.0);
				if (nextArrival < TAU) {
					// SCHEDULE NEW EVENT
					Event newEvent = new Event(nextArrival, uniform(0,2), EventType.JOB_ARRIVAL);
					events.add(newEvent);
				}
			case FEEDBACK_ARRIVAL:
				ssq.enque_e(event);
				break;
			case JOB_COMPLETION:
				if ( Double.compare( uniform(0,1), 0.25) < 0 ) {
					double feedbackArrival = clock.get_time() + exponential(1.0);
					double lastService = event.get_service();
					Event feedback = new Event(feedbackArrival, lastService/2.0, EventType.FEEDBACK_ARRIVAL);
					events.add(feedback);
				}
				inService = false;
			default:
				break;
			}
			if ( !inService && ssq.get_l() > 0) {
				Event e = ssq.get_event();
				Event complete = new Event(clock.get_time() + e.get_service(), 0.0, EventType.JOB_COMPLETION);
				events.add(complete);
				inService = true;
				jobsServiced++;
			}
		}
		System.out.println("Final time: " + clock.get_time() + "\nNew jobs: " + newJobCount + "\nJobs serviced: " + jobsServiced);
		
	}
}
