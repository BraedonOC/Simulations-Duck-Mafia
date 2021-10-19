package SSQ;

public class Event{

	private double at;
	private double service;
	private EventType type;
	
	public double get_at() {
		return this.at;
	}

	
	public EventType get_type() {
		return this.type;
	}
	
	public double get_service() {
		return this.service;
	}
	
	public Event(double at, double service, EventType type) {
		this.at = at;
		this.service = service;
		this.type = type;
	}
	
	public String toString() {
		return "EVENT: " + this.type + " AT: " + this.at + " SERVICE: " + this.service;
	}
}
