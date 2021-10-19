package SSQ;

public class Clock {
	private double time;
	
	public double get_time() {
		return this.time;
	}
	
	public void set_time(double t) {
		this.time = t;
	}
	
	public Clock() {
		this.time = 0.0;
	}
}
