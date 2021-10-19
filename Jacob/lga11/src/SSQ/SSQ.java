package SSQ;

import java.util.*;

public class SSQ{

	private Queue<Event> job_queue; 

	public SSQ() {
		this.job_queue = new LinkedList<Event>();
	}
	
	public void enque_e(Event e) {
		this.job_queue.add(e);
	}
	
	public Event get_event() {
		return this.job_queue.remove();
	}
	/*
	 * l returns the l statistic
	 */
	public int get_l() {
		return this.job_queue.size();
	}
	
}
